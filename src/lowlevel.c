#include <Mw/Milsko.h>

/* Older GCC doesn't like this part of the code because of common sections. 
	What are common sections? This question is so obscure and unknown
 	that newer GCC actually compiles with -fno-common by default because 
	not enough people ever figured this out. But when compiling under
	older Mac OS X. specifically the gcc that comes with tigerbrew (this
	has not yet been tried under XCode 10.5), this comes up. Furthermore,
	adding -fno-common to either the cflags or ldflags doesn't seem to fix this.
	But what does fix it is using GNU attributes. So...yeah, sure, we'll just do
	this.*/
#ifdef __APPLE__
#define NOCOMMON __attribute__((nocommon))
#else
#define NOCOMMON
#endif

NOCOMMON MwLL (*MwLLCreate)(MwLL parent, int x, int y, int width, int height);
NOCOMMON void (*MwLLDestroy)(MwLL handle);

NOCOMMON void (*MwLLPolygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color);
NOCOMMON void (*MwLLLine)(MwLL handle, MwPoint* points, MwLLColor color);

NOCOMMON void (*MwLLBeginDraw)(MwLL handle);
NOCOMMON void (*MwLLEndDraw)(MwLL handle);

NOCOMMON MwLLColor (*MwLLAllocColor)(MwLL handle, int r, int g, int b);
NOCOMMON void (*MwLLColorUpdate)(MwLL handle, MwLLColor c, int r, int g, int b);
NOCOMMON void (*MwLLFreeColor)(MwLLColor color);

NOCOMMON void (*MwLLGetXYWH)(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
NOCOMMON void (*MwLLSetXY)(MwLL handle, int x, int y);
NOCOMMON void (*MwLLSetWH)(MwLL handle, int w, int h);

NOCOMMON void (*MwLLSetTitle)(MwLL handle, const char* title);

NOCOMMON int (*MwLLPending)(MwLL handle);
NOCOMMON void (*MwLLNextEvent)(MwLL handle);

NOCOMMON MwLLPixmap (*MwLLCreatePixmap)(MwLL handle, unsigned char* data, int width, int height);
NOCOMMON void (*MwLLPixmapUpdate)(MwLLPixmap pixmap);
NOCOMMON void (*MwLLDestroyPixmap)(MwLLPixmap pixmap);
NOCOMMON void (*MwLLDrawPixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap);
NOCOMMON void (*MwLLSetIcon)(MwLL handle, MwLLPixmap pixmap);

NOCOMMON void (*MwLLForceRender)(MwLL handle);

NOCOMMON void (*MwLLSetCursor)(MwLL handle, MwCursor* image, MwCursor* mask);
NOCOMMON void (*MwLLDetach)(MwLL handle, MwPoint* point);
NOCOMMON void (*MwLLShow)(MwLL handle, int show);

NOCOMMON void (*MwLLSetSizeHints)(MwLL handle, int minx, int miny, int maxx, int maxy);
NOCOMMON void (*MwLLMakeBorderless)(MwLL handle, int toggle);
NOCOMMON void (*MwLLMakeToolWindow)(MwLL handle);
NOCOMMON void (*MwLLMakePopup)(MwLL handle, MwLL parent);

NOCOMMON void (*MwLLBeginStateChange)(MwLL handle);
NOCOMMON void (*MwLLEndStateChange)(MwLL handle);

NOCOMMON void (*MwLLFocus)(MwLL handle);
NOCOMMON void (*MwLLGrabPointer)(MwLL handle, int toggle);

NOCOMMON void (*MwLLSetClipboard)(MwLL handle, const char* text);
NOCOMMON void (*MwLLGetClipboard)(MwLL handle);

NOCOMMON void (*MwLLGetCursorCoord)(MwLL handle, MwPoint* point);
NOCOMMON void (*MwLLGetScreenSize)(MwLL handle, MwRect* rect);

void MwLLCreateCommon(MwLL handle) {
	handle->common.handler = malloc(sizeof(*handle->common.handler));
	memset(handle->common.handler, 0, sizeof(*handle->common.handler));
}

void MwLLDestroyCommon(MwLL handle) {
	free(handle->common.handler);
}
