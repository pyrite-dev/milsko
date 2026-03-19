#include <Mw/Milsko.h>

MwLL (*MwLLCreate)(MwLL parent, int x, int y, int width, int height) = NULL;
void (*MwLLDestroy)(MwLL handle)				     = NULL;

void (*MwLLPolygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color) = NULL;
void (*MwLLLine)(MwLL handle, MwPoint* points, MwLLColor color)			     = NULL;

void (*MwLLBeginDraw)(MwLL handle) = NULL;
void (*MwLLEndDraw)(MwLL handle)   = NULL;

MwLLColor (*MwLLAllocColor)(MwLL handle, int r, int g, int b)	       = NULL;
void (*MwLLColorUpdate)(MwLL handle, MwLLColor c, int r, int g, int b) = NULL;
void (*MwLLFreeColor)(MwLLColor color)				       = NULL;

void (*MwLLGetXYWH)(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) = NULL;
void (*MwLLSetXY)(MwLL handle, int x, int y)					   = NULL;
void (*MwLLSetWH)(MwLL handle, int w, int h)					   = NULL;

void (*MwLLSetTitle)(MwLL handle, const char* title) = NULL;

int (*MwLLPending)(MwLL handle)	   = NULL;
void (*MwLLNextEvent)(MwLL handle) = NULL;

MwLLPixmap (*MwLLCreatePixmap)(MwLL handle, unsigned char* data, int width, int height) = NULL;
void (*MwLLPixmapUpdate)(MwLLPixmap pixmap)						= NULL;
void (*MwLLDestroyPixmap)(MwLLPixmap pixmap)						= NULL;
void (*MwLLDrawPixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap)			= NULL;
void (*MwLLSetIcon)(MwLL handle, MwLLPixmap pixmap)					= NULL;

void (*MwLLForceRender)(MwLL handle) = NULL;

void (*MwLLSetCursor)(MwLL handle, MwCursor* image, MwCursor* mask) = NULL;
void (*MwLLDetach)(MwLL handle, MwPoint* point)			    = NULL;
void (*MwLLShow)(MwLL handle, int show)				    = NULL;

void (*MwLLSetSizeHints)(MwLL handle, int minx, int miny, int maxx, int maxy) = NULL;
void (*MwLLMakeBorderless)(MwLL handle, int toggle)			      = NULL;
void (*MwLLMakeToolWindow)(MwLL handle)					      = NULL;
void (*MwLLMakePopup)(MwLL handle, MwLL parent)				      = NULL;

void (*MwLLBeginStateChange)(MwLL handle) = NULL;
void (*MwLLEndStateChange)(MwLL handle)	  = NULL;

void (*MwLLFocus)(MwLL handle)			 = NULL;
void (*MwLLGrabPointer)(MwLL handle, int toggle) = NULL;

void (*MwLLSetClipboard)(MwLL handle, const char* text) = NULL;
void (*MwLLGetClipboard)(MwLL handle)			= NULL;

void (*MwLLGetCursorCoord)(MwLL handle, MwPoint* point) = NULL;
void (*MwLLGetScreenSize)(MwLL handle, MwRect* rect)	= NULL;

void MwLLCreateCommon(MwLL handle) {
	handle->common.handler = malloc(sizeof(*handle->common.handler));
	memset(handle->common.handler, 0, sizeof(*handle->common.handler));
}

void MwLLDestroyCommon(MwLL handle) {
	free(handle->common.handler);
}
