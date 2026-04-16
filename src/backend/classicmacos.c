#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;

	MwLLCreateCommon(r);

	if(!parent) {
		SetRect(&r->cmacos.winRect, 100, 100, width, height);
		r->cmacos.window = NewCWindow(nil, &r->cmacos.winRect, (ConstStr255Param) "\p", true,
					      documentProc, (WindowPtr)(-1), true, 0);
		SetPort(r->cmacos.window);
		//
	}

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	free(handle);
}

static void MwLLBeginDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int	   i;
	PolyHandle p = OpenPoly();
	RGBColor   col;

	col.red	  = color->common.red;
	col.green = color->common.green;
	col.blue  = color->common.blue;

	RGBBackColor(&col);
	RGBForeColor(&col);

	for(i = 0; i < points_count; i++) {
		if(i == 0) {
			MoveTo(points[i].x, points[i].y);
		} else {
			LineTo(points[i].x, points[i].y);
		}
	}

	ClosePoly();
	PaintPoly(p);
	KillPoly(p);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	RGBColor col;

	col.red	  = color->common.red;
	col.green = color->common.green;
	col.blue  = color->common.blue;

	RGBBackColor(&col);
	RGBForeColor(&col);

	MoveTo(points[0].x, points[0].y);
	LineTo(points[1].x, points[1].y);
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Rect bounds;
	GetWindowBounds(handle->cmacos.window, kWindowContentRgn, &bounds);
	*x = bounds.left;
	*y = bounds.top;
	*w = bounds.right;
	*h = bounds.bottom;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	MoveWindow(handle->cmacos.window, x, y, MwTRUE);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	SizeWindow(handle->cmacos.window, w, h, MwTRUE);
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static int MwLLPendingImpl(MwLL handle) {
	return GetNextEvent(everyEvent, &handle->cmacos.event);
}

static void MwLLNextEventImpl(MwLL handle) {
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	setwtitle(handle->cmacos.window, title);
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));

	r->common.raw = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	MwLLPixmapUpdate(r);
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
}

static void MwLLForceRenderImpl(MwLL handle) {
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
}

static void MwLLShowImpl(MwLL handle, int show) {
	if(show)
		ShowWindow(handle->cmacos.window);
	else
		HideWindow(handle->cmacos.window);
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
}

static void MwLLFocusImpl(MwLL handle) {
	HiliteWindow(handle->cmacos.window, MwTRUE);
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	/* TODO */

	(void)handle;
	(void)text;
}

static void MwLLGetClipboardImpl(MwLL handle) {
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static void MwLLSetDarkThemeImpl(MwLL handle, int toggle) {
	(void)handle;
	(void)toggle;
}

static int MwLLClassicMacOSCallInitImpl(void) {
	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();

	return 0;
}

#include "call.c"
CALL(ClassicMacOS);
