/* $Id$ */
#include <Mw/Milsko.h>

static void MwLLPolygonImplGL(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
}

static void MwLLLineImplGL(MwLL handle, MwPoint* points, MwLLColor color) {
}

static MwLLColor MwLLAllocColorImplGL(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	MwLLColorUpdate(handle, c, r, g, b);

	return c;
}

static void MwLLColorUpdateImplGL(MwLL handle, MwLLColor c, int r, int g, int b) {
}

static void MwLLFreeColorImplGL(MwLLColor color) {
	free(color);
}

static void MwLLSetBackgroundImplGL(MwLL handle, MwLLColor color) {
	(void)handle;
	(void)color;
}

static MwLLPixmap MwLLCreatePixmapImplGL(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));

	return r;
}

static void MwLLPixmapUpdateImplGL(MwLLPixmap r) {
	int y, x;
	int w = (r->common.width + (16 - (r->common.width % 16))) / 8;
}

static void MwLLDestroyPixmapImplGL(MwLLPixmap pixmap) {
}

static void MwLLDrawPixmapImplGL(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
}
