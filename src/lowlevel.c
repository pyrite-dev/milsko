/* $Id$ */
#include <Mw/Milsko.h>

MwLL (*MwLLCreate)(MwLL parent, int x, int y, int width, int height);
void (*MwLLDestroy)(MwLL handle);

void (*MwLLPolygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color);
void (*MwLLLine)(MwLL handle, MwPoint* points, MwLLColor color);

MwLLColor (*MwLLAllocColor)(MwLL handle, int r, int g, int b);
void (*MwLLColorUpdate)(MwLL handle, MwLLColor c, int r, int g, int b);
void (*MwLLFreeColor)(MwLLColor color);

void (*MwLLGetXYWH)(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
void (*MwLLSetXY)(MwLL handle, int x, int y);
void (*MwLLSetWH)(MwLL handle, int w, int h);

void (*MwLLSetTitle)(MwLL handle, const char* title);

int (*MwLLPending)(MwLL handle);
void (*MwLLNextEvent)(MwLL handle);

MwLLPixmap (*MwLLCreatePixmap)(MwLL handle, unsigned char* data, int width, int height);
void (*MwLLPixmapUpdate)(MwLLPixmap pixmap);
void (*MwLLDestroyPixmap)(MwLLPixmap pixmap);
void (*MwLLDrawPixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap);
void (*MwLLSetIcon)(MwLL handle, MwLLPixmap pixmap);

void (*MwLLForceRender)(MwLL handle);

void (*MwLLSetCursor)(MwLL handle, MwCursor* image, MwCursor* mask);
void (*MwLLDetach)(MwLL handle, MwPoint* point);
void (*MwLLShow)(MwLL handle, int show);

void (*MwLLMakePopup)(MwLL handle, MwLL parent);

void (*MwLLSetSizeHints)(MwLL handle, int minx, int miny, int maxx, int maxy);
void (*MwLLMakeBorderless)(MwLL handle, int toggle);

void (*MwLLSetBackground)(MwLL handle, MwLLColor color);

void (*MwLLFocus)(MwLL handle);
void (*MwLLGrabPointer)(MwLL handle, int toggle);

void (*MwLLSetClipboard)(MwLL handle, const char* text);
char* (*MwLLGetClipboard)(MwLL handle);

void (*MwLLMakeToolWindow)(MwLL handle);

void MwLLCreateCommon(MwLL handle) {
	handle->handler = malloc(sizeof(*handle->handler));
	memset(handle->handler, 0, sizeof(*handle->handler));
}

void MwLLDestroyCommon(MwLL handle) {
	free(handle->handler);
}
