/* $Id$ */
/* This file gets included by backend */

#include "Mw/Abstract/Dynamic.h"
#include "Mw/BaseTypes.h"
#include "Mw/LowLevel.h"
#include <X11/Xlib.h>
#ifdef USE_X11
#include "render/x11.c"
#endif
#ifdef USE_GDI
#include "render/gdi.c"
#endif
#ifdef BUILD_OPENGL
#include "render/gl.c"
MwBool _MwLLHasOpenGL = MwFALSE;
#endif

static inline void InitOpenGLFuncs(void) {
#ifdef BUILD_OPENGL
	MwLLGLLibGet	     = MwLLGLLibGetImpl;
	MwLLGLCreate	     = MwLLGLCreateImpl;
	MwLLGLDestroy	     = MwLLGLDestroyImpl;
	MwLLGLMakeCurrent    = MwLLGLMakeCurrentImpl;
	MwLLGLSwapBuffer     = MwLLGLSwapBufferImpl;
	MwLLGLGetProcAddress = MwLLGLGetProcAddressImpl;
#endif
}

static void InitDrawFuncs(void) {
#ifdef BUILD_OPENGL
	void* lib = MwLLGLLibGet();
	if(lib != NULL) {
		MwDynamicClose(lib);
		_MwLLHasOpenGL	  = MwTRUE;
		MwLLAllocColor	  = MwLLAllocColorImplGL;
		MwLLColorUpdate	  = MwLLColorUpdateImplGL;
		MwLLFreeColor	  = MwLLFreeColorImplGL;
		MwLLCreatePixmap  = MwLLCreatePixmapImplGL;
		MwLLPixmapUpdate  = MwLLPixmapUpdateImplGL;
		MwLLDestroyPixmap = MwLLDestroyPixmapImplGL;
		MwLLDrawPixmap	  = MwLLDrawPixmapImplGL;
		MwLLPolygon	  = MwLLPolygonImplGL;
		MwLLLine	  = MwLLLineImplGL;
	} else {
#endif
		MwLLAllocColor	  = MwLLAllocColorImplNative;
		MwLLColorUpdate	  = MwLLColorUpdateImplNative;
		MwLLFreeColor	  = MwLLFreeColorImplNative;
		MwLLCreatePixmap  = MwLLCreatePixmapImplNative;
		MwLLPixmapUpdate  = MwLLPixmapUpdateImplNative;
		MwLLDestroyPixmap = MwLLDestroyPixmapImplNative;
		MwLLDrawPixmap	  = MwLLDrawPixmapImplNative;
		MwLLPolygon	  = MwLLPolygonImplNative;
		MwLLLine	  = MwLLLineImplNative;
#ifdef BUILD_OPENGL
	}
#endif
}

#define CALL(NAME) \
	int MwLL##NAME##CallInit(void) { \
		int st; \
\
		if((st = MwLL##NAME##CallInitImpl()) != 0) return st; \
\
		MwLLCreate  = MwLLCreateImpl; \
		MwLLDestroy = MwLLDestroyImpl; \
\
		MwLLGetXYWH = MwLLGetXYWHImpl; \
		MwLLSetXY   = MwLLSetXYImpl; \
		MwLLSetWH   = MwLLSetWHImpl; \
\
		MwLLSetTitle = MwLLSetTitleImpl; \
\
		MwLLPending   = MwLLPendingImpl; \
		MwLLNextEvent = MwLLNextEventImpl; \
\
		MwLLSetIcon	= MwLLSetIconImpl; \
		MwLLForceRender = MwLLForceRenderImpl; \
\
		MwLLSetCursor = MwLLSetCursorImpl; \
		MwLLDetach    = MwLLDetachImpl; \
		MwLLShow      = MwLLShowImpl; \
\
		MwLLSetSizeHints   = MwLLSetSizeHintsImpl; \
		MwLLMakeBorderless = MwLLMakeBorderlessImpl; \
		MwLLMakeToolWindow = MwLLMakeToolWindowImpl; \
		MwLLMakePopup	   = MwLLMakePopupImpl; \
\
		MwLLBeginStateChange = MwLLBeginStateChangeImpl; \
		MwLLEndStateChange   = MwLLEndStateChangeImpl; \
\
		MwLLSetBackground = MwLLSetBackgroundImpl; \
\
		MwLLFocus	= MwLLFocusImpl; \
		MwLLGrabPointer = MwLLGrabPointerImpl; \
\
		MwLLSetClipboard = MwLLSetClipboardImpl; \
		MwLLGetClipboard = MwLLGetClipboardImpl; \
\
		InitOpenGLFuncs(); \
		InitDrawFuncs(); \
\
		return 0; \
	}
