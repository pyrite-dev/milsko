#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL	      r;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	
	r = malloc(sizeof(*r));
	

	MwLLCreateCommon(r);

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
	(void)points;
	(void)points_count;
	(void)color;
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	(void)handle;
	(void)points;
	(void)color;
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	(void)handle;
	
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	*x = 0;
	*y = 0;
	*w = 0;
	*h = 0;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	(void)handle;
	(void)x;
	(void)y;
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	(void)handle;
	(void)w;
	(void)h;
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static int MwLLPendingImpl(MwLL handle) {
	(void)handle;
	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	(void)handle;
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	(void)title;
	(void)handle;
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	(void)handle;
	
	MwLLPixmap	  r = malloc(sizeof(*r));

	r->common.raw = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	r->common.width	 = width;
	r->common.height = height;

	MwLLPixmapUpdate(r);
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	(void)r;
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	(void)handle;
	(void)rect;
	(void)pixmap;
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	(void)handle;
	(void)pixmap;
}

static void MwLLForceRenderImpl(MwLL handle) {
	(void)handle;
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	(void)handle;
	(void)image;
	(void)mask;
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	(void)handle;
	(void)point;
}

static void MwLLShowImpl(MwLL handle, int show) {
	(void)handle;
	(void)show;
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	(void)handle;
	(void)parent;
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	(void)handle;
	(void)minx;
	(void)miny;
	(void)maxx;
	(void)maxy;
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
    (void)handle;
	(void)toggle;
}

static void MwLLFocusImpl(MwLL handle) {
    (void)handle;
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
    (void)handle;
    (void)toggle;
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	(void)handle;
	(void)text;
}

static void MwLLGetClipboardImpl(MwLL handle) {
	(void)handle;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
    (void)handle;
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
    (void)handle;
	(void)point;
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	(void)handle;
	(void)rect;
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static int AppKit(void) {
	return 0;
}

#include "call.c"
CALL(AppKit);
