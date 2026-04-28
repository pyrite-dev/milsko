#include <Mw/Milsko.h>

MwView::MwView(MwApplication* app, BRect rc, uint32 resizing, uint32 flags) : BView(rc, NULL, resizing, flags) {
	app->PostMessage(new BMessage(B_PULSE));

	this->SetViewColor(rand() % 256, 0, 0);
	printf("?\n");
}

void MwApplication::ReadyToRun(){
	this->ready = 1;
}

void MwApplication::Pulse(){
	printf("!\n");
}

MwApplication::MwApplication(BRect rc, MwLL handle) : BApplication("application/milsko-generic") {
	BScreen* scr = new BScreen();
	float left = rc.left;
	float top = rc.top;

	if(rc.left == MwDEFAULT) left = (scr->Frame().Width() - rc.Width()) / 2;
	if(rc.top == MwDEFAULT) top = (scr->Frame().Height() - rc.Height()) / 2;
	rc = BRect(BPoint(left, top), BSize(rc.Width(), rc.Height()));

	this->window = new BWindow(rc, "Milsko", B_TITLED_WINDOW, 0);
	this->window->Show();

	handle->haiku.view = new MwView(this, this->window->Bounds(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);

	this->window->AddChild(handle->haiku.view);

	this->ready = 0;
	this->locker = new BLocker();

	delete scr;
}

MwApplication::~MwApplication(){
	delete this->window;
	delete this->locker;
}

MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r = (MwLL)malloc(sizeof(*r));
	BRect rc(BPoint(x, y), BSize(width, height));

	MwLLCreateCommon(r);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendHaiku;

	r->haiku.parent = parent;

	if(parent == NULL){
		r->haiku.app = new MwApplication(rc, r);

		//r->haiku.app->Run();
	}else{
		MwLL top = r;
		while(top->haiku.parent != NULL){
			top = top->haiku.parent;
		}

		r->haiku.app = NULL;
		r->haiku.view = new MwView(top->haiku.app, rc, B_FOLLOW_NONE, B_WILL_DRAW);

		parent->haiku.view->AddChild(r->haiku.view);
	}

	return r;
}

void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	free(handle);
}

void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	BPoint* p = (BPoint*)malloc(sizeof(*p) * points_count);
	int i;

	for(i = 0; i < points_count; i++){
		p[i] = BPoint(points[i].x, points[i].y);
	}

	handle->haiku.view->FillPolygon(p, points_count);

	free(p);
}

void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {}

void MwLLBeginDrawImpl(MwLL handle) {}

void MwLLEndDrawImpl(MwLL handle) {
	new BMessage(B_PULSE);
}

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

void MwLLFreeColorImpl(MwLLColor color) {}

void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {}

void MwLLSetXYImpl(MwLL handle, int x, int y) {}

void MwLLSetWHImpl(MwLL handle, int w, int h) {}

void MwLLSetTitleImpl(MwLL handle, const char* title) {}

int MwLLPendingImpl(MwLL handle) {
	return 0;
}

void MwLLNextEventImpl(MwLL handle) {}

MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = (MwLLPixmap)malloc(sizeof(*r));
	r->common.raw = (unsigned char*)malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	r->common.width	 = width;
	r->common.height = height;

	MwLLPixmapUpdate(r);
	return r;

	return r;
}

void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {}

void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap->common.raw);

	free(pixmap);
}

void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {}

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
