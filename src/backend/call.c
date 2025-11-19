/* $Id$ */
/* This file gets included by backend */

#ifdef USE_X11
#include "render/x11.c"
#endif
#ifdef USE_GDI
#include "render/gdi.c"
#endif

#ifdef BUILD_OPENGL
#define GL_FUNCS \
	MwLLGLCreate	     = MwLLGLCreateImpl; \
	MwLLGLDestroy	     = MwLLGLDestroyImpl; \
	MwLLGLMakeCurrent    = MwLLGLMakeCurrentImpl; \
	MwLLGLSwapBuffer     = MwLLGLSwapBufferImpl; \
	MwLLGLGetProcAddress = MwLLGLGetProcAddressImpl;
#else
#define GL_FUNCS
#endif

#define CALL(NAME) \
	int MwLL##NAME##CallInit(void) { \
		int st; \
\
		if((st = MwLL##NAME##CallInitImpl()) != 0) return st; \
\
		MwLLCreate  = MwLLCreateImpl; \
		MwLLDestroy = MwLLDestroyImpl; \
\
		MwLLPolygon = MwLLPolygonImplNative; \
		MwLLLine    = MwLLLineImplNative; \
\
		MwLLAllocColor	= MwLLAllocColorImplNative; \
		MwLLColorUpdate = MwLLColorUpdateImplNative; \
		MwLLFreeColor	= MwLLFreeColorImplNative; \
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
		MwLLCreatePixmap  = MwLLCreatePixmapImplNative; \
		MwLLPixmapUpdate  = MwLLPixmapUpdateImplNative; \
		MwLLDestroyPixmap = MwLLDestroyPixmapImplNative; \
		MwLLDrawPixmap	  = MwLLDrawPixmapImplNative; \
		MwLLSetIcon	  = MwLLSetIconImpl; \
\
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
		GL_FUNCS \
\
		return 0; \
	}
