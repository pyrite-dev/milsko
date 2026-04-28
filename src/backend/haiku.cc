#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static void fix_point(BPoint* p, MwView* v){
	p->x = p->x * (v->handle->haiku.width + 1) / v->handle->haiku.width;
	p->y = p->y * (v->handle->haiku.height + 1) / v->handle->haiku.height;
}

MwApplication::MwApplication(BRect rc, MwLL handle) : BApplication("application/milsko-generic") {
	BScreen* scr  = new BScreen();
	float	 left = rc.left;
	float	 top  = rc.top;

	if(rc.left == MwDEFAULT) left = (scr->Frame().Width() - rc.Width()) / 2;
	if(rc.top == MwDEFAULT) top = (scr->Frame().Height() - rc.Height()) / 2;
	rc = BRect(BPoint(left, top), BSize(rc.Width(), rc.Height()));

	this->window = new MwWindow(rc, B_TITLED_WINDOW, 0);
	this->window->Show();

	handle->haiku.view = new MwView(handle, this->window->Bounds(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);

	this->window->AddChild(handle->haiku.view);

	delete scr;
}

MwApplication::~MwApplication() {
	delete this->window;
}

void MwApplication::MessageReceived(BMessage* message) {
	switch(message->what) {
	case BAPP_MW_DESTROY: {
		this->Quit();
		break;
	}
	default: {
		BLooper::MessageReceived(message);
		break;
	}
	}
}

MwView::MwView(MwLL handle, BRect frame, uint32 resizingMode, uint32 flags) : BView(frame, NULL, resizingMode, flags) {
	this->handle = handle;

	this->locker = new BLocker();

	this->SetDrawingMode(B_OP_OVER);

	this->SetViewColor(255, 0, 0);
}

MwView::~MwView(){
	delete this->locker;
}

void MwView::MessageReceived(BMessage* message) {
	switch(message->what) {
	case BVIEW_MW_RESIZE: {
		int width, height;

		if(message->FindInt32("view-width", &width) != B_OK) break;
		if(message->FindInt32("view-height", &height) != B_OK) break;

		this->ResizeTo(width, height);
		break;
	}
	case BVIEW_MW_MOVE: {
		int x, y;

		if(message->FindInt32("view-x", &x) != B_OK) break;
		if(message->FindInt32("view-y", &y) != B_OK) break;

		this->MoveTo(x, y);
		break;
	}
	case BVIEW_MW_SET_COLOR:
	{
		int32 rgb[3];
		int i;

		rgb[0] = rgb[1] = rgb[2] = 0;

		for(i = 0; i < 3 && message->FindInt32("view-color", i, &rgb[i]) == B_OK; i++);

		this->SetHighColor(rgb[0], rgb[1], rgb[2]);

		break;
	}
	case BVIEW_MW_POLYGON:
	{
		int i;
		BPoint p;
		BPoint points[128];

		for(i = 0; message->FindPoint("view-point", i, &p) == B_OK; i++){
			points[i] = p;
		}

		this->FillPolygon(points, i);
		break;
	}
	case BVIEW_MW_BITMAP:
	{
		BRect rc;
		BBitmap* bitmap;

		if(message->FindRect("view-rect", &rc) != B_OK) break;
		if(message->FindPointer("view-bitmap", (void**)&bitmap) != B_OK) break;

		this->DrawBitmap(bitmap, rc);
		break;
	}
	default: {
		BView::MessageReceived(message);
		break;
	}
	}
}

void MwView::PostMessage(BMessage* message){
	BMessage copy = *message;

	copy.AddPointer("window-view", this);

	this->Window()->PostMessage(&copy);
}

void MwView::Draw(BRect updateRect){
	MwLLHaikuEvent ev;

	(void)updateRect;

	ev.type = MwLLHAIKU_EVENT_DRAW;

	this->locker->Lock();
	arrput(this->handle->haiku.events, ev);
	this->locker->Unlock();
}

void MwView::SetColor(MwLLColor color){
	BMessage msg(BVIEW_MW_SET_COLOR);

	msg.AddInt32("view-color", color->common.red);
	msg.AddInt32("view-color", color->common.green);
	msg.AddInt32("view-color", color->common.blue);

	this->PostMessage(&msg);
}

MwWindow::MwWindow(BRect frame, window_type type, uint32 flags) : BWindow(frame, "Milsko", type, flags) {
}

void MwWindow::MessageReceived(BMessage* message) {
	void* ptr;

	if(message->FindPointer("window-view", &ptr) == B_OK){
		MwView* v = (MwView*)ptr;

		v->MessageReceived(message);

		return;
	}

	switch(message->what) {
	case BWIN_MW_SET_TITLE: {
		const char* title;

		if(message->FindString("window-title", &title) != B_OK) break;

		this->SetTitle(title);
		break;
	}
	default: {
		BWindow::MessageReceived(message);
		break;
	}
	}
}

struct app_param {
	BRect rc;
	MwLL  handle;
};

static int32 app_thread(void* data) {
	struct app_param* r = (struct app_param*)data;

	r->handle->haiku.app = new MwApplication(r->rc, r->handle);
	r->handle->haiku.app->Run();

	free(r);

	return 0;
}

MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL  r = (MwLL)malloc(sizeof(*r));
	BRect rc = BRect(BPoint(x, y), BSize(width, height));

	MwLLCreateCommon(r);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendHaiku;

	r->haiku.parent = parent;

	r->haiku.events = NULL;

	if(parent == NULL) {
		struct app_param* p = (struct app_param*)malloc(sizeof(*p));

		r->haiku.app = NULL;

		p->rc	  = rc;
		p->handle = r;

		r->haiku.app = NULL;
		r->haiku.view = NULL;

		r->haiku.app_thread = spawn_thread(app_thread, NULL, B_NORMAL_PRIORITY, p);
		resume_thread(r->haiku.app_thread);

		while(r->haiku.app == NULL) MwTimeSleep(10);
	} else {
		MwLL top = r;
		while(top->haiku.parent != NULL) {
			top = top->haiku.parent;
		}

		r->haiku.app  = NULL;
		r->haiku.view = new MwView(r, rc, B_FOLLOW_NONE, B_WILL_DRAW);

		parent->haiku.view->AddChild(r->haiku.view);
	}

	r->haiku.x = x;
	r->haiku.y = y;
	r->haiku.width = width;
	r->haiku.height = height;

	return r;
}

void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	if(handle->haiku.app != NULL) {
		status_t exit_value;

		handle->haiku.app->PostMessage(BAPP_MW_DESTROY);
		wait_for_thread(handle->haiku.app_thread, &exit_value);

		delete handle->haiku.app;
	}
	delete handle->haiku.view;

	free(handle);
}

void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int	i;
	BMessage msg(BVIEW_MW_POLYGON);

	for(i = 0; i < points_count; i++) {
		BPoint p(points[i].x, points[i].y);

		fix_point(&p, handle->haiku.view);

		msg.AddPoint("view-point", p);
	}

	handle->haiku.view->SetColor(color);
	handle->haiku.view->PostMessage(&msg);
}

void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {}

void MwLLBeginDrawImpl(MwLL handle) {}

void MwLLEndDrawImpl(MwLL handle) {}

MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = (MwLLColor)malloc(sizeof(*c));

	MwLLColorUpdate(handle, c, r, g, b);

	return c;
}

void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	BRect rc;
	int m = 0;

	if(handle->haiku.app == NULL){
		rc = BRect(BPoint(handle->haiku.x, handle->haiku.y), BSize(handle->haiku.width, handle->haiku.height));
	}else{
		rc = handle->haiku.app->window->Frame();
	}

	*x = rc.left;
	*y = rc.top;
	*w = rc.Width();
	*h = rc.Height();
}

void MwLLSetXYImpl(MwLL handle, int x, int y) {
	BMessage msg(BVIEW_MW_MOVE);

	handle->haiku.x = x;
	handle->haiku.y = y;

	msg.AddInt32("view-x", x);
	msg.AddInt32("view-y", y);

	handle->haiku.view->PostMessage(&msg);
}

void MwLLSetWHImpl(MwLL handle, int w, int h) {
	BMessage msg(BVIEW_MW_RESIZE);

	handle->haiku.width = w;
	handle->haiku.height = h;

	msg.AddInt32("view-width", w);
	msg.AddInt32("view-height", h);

	handle->haiku.view->PostMessage(&msg);
}

void MwLLSetTitleImpl(MwLL handle, const char* title) {
	BMessage msg(BWIN_MW_SET_TITLE);

	if(handle->haiku.app == NULL) return;

	msg.AddString("window-title", title);

	handle->haiku.app->window->PostMessage(&msg);
}

int MwLLPendingImpl(MwLL handle) {
	handle->haiku.view->locker->Lock();
	if(arrlen(handle->haiku.events) > 0){
		handle->haiku.view->locker->Unlock();
		return 1;
	}
	handle->haiku.view->locker->Unlock();

	return 0;
}

void MwLLNextEventImpl(MwLL handle) {
	handle->haiku.view->locker->Lock();
	while(arrlen(handle->haiku.events) > 0){
		MwLLHaikuEvent* ev = &handle->haiku.events[0];

		if(ev->type == MwLLHAIKU_EVENT_DRAW){
			MwLLDispatch(handle, draw, NULL);
		}

		arrdel(handle->haiku.events, 0);
	}
	handle->haiku.view->locker->Unlock();
}

MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r  = (MwLLPixmap)malloc(sizeof(*r));
	r->common.raw = (unsigned char*)malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	r->common.width	 = width;
	r->common.height = height;

	r->haiku.bitmap = NULL;

	MwLLPixmapUpdate(r);
	return r;

	return r;
}

void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {
	uint32* px = (uint32*)malloc(4 * pixmap->common.width * pixmap->common.height);
	int i;

	for(i = 0; i < pixmap->common.width * pixmap->common.height; i++){
		unsigned char* ipx = &pixmap->common.raw[4 * i];

		px[i] = (ipx[3] << 24) | (ipx[0] << 16) | (ipx[1] << 8) | (ipx[2] << 0);
	}

	if(pixmap->haiku.bitmap != NULL) delete pixmap->haiku.bitmap;

	pixmap->haiku.bitmap = new BBitmap(BRect(0, 0, pixmap->common.width - 1, pixmap->common.height - 1), B_RGBA32);

	pixmap->haiku.bitmap->SetBits(px, pixmap->common.width * pixmap->common.height * 4, 0, B_RGBA32);

	free(px);
}

void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap->common.raw);

	free(pixmap);
}

void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	BPoint p;
	BRect rc;
	BMessage msg(BVIEW_MW_BITMAP);

	p = BPoint(rect->x, rect->y);
	fix_point(&p, handle->haiku.view);
	rc = BRect(p, BSize(rect->width, rect->height));

	msg.AddRect("view-rect", rc);
	msg.AddPointer("view-bitmap", pixmap->haiku.bitmap);

	handle->haiku.view->PostMessage(&msg);
}

void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {}

void MwLLForceRenderImpl(MwLL handle) {}

void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {}

void MwLLDetachImpl(MwLL handle, MwPoint* point) {}

void MwLLShowImpl(MwLL handle, int show) {}

void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {}

void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {}

void MwLLMakeToolWindowImpl(MwLL handle) {}

void MwLLMakePopupImpl(MwLL handle, MwLL parent) {}

void MwLLBeginStateChangeImpl(MwLL handle) {}

void MwLLEndStateChangeImpl(MwLL handle) {}

void MwLLFocusImpl(MwLL handle) {}

void MwLLGrabPointerImpl(MwLL handle, int toggle) {}

void MwLLSetClipboardImpl(MwLL handle, const char* text, int clipboard_type) {}

void MwLLGetClipboardImpl(MwLL handle, int clipboard_type) {}

void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {}

void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {}

void MwLLSetDarkThemeImpl(MwLL handle, int toggle) {
	(void)handle;
	(void)toggle;
}

MwBool MwLLDoModernImpl(MwLL handle) {
	(void)handle;
	return MwTRUE;
}

int MwLLHaikuCallInitImpl() {
	return 0;
}

#include "call.c"
CALL(Haiku);
