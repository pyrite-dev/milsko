/* $Id$ */
/* This file gets included by backend */
#define CALL(NAME) \
	int MwLL##NAME##CallInit(void) { \
		int st; \
\
		if((st = MwLL##NAME##CallInitImpl()) != 0) return st; \
\
		MwLLCreate  = MwLLCreateImpl; \
		MwLLDestroy = MwLLDestroyImpl; \
\
		MwLLPolygon = MwLLPolygonImpl; \
		MwLLLine    = MwLLLineImpl; \
\
		MwLLAllocColor	= MwLLAllocColorImpl; \
		MwLLColorUpdate = MwLLColorUpdateImpl; \
		MwLLFreeColor	= MwLLFreeColorImpl; \
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
		MwLLCreatePixmap  = MwLLCreatePixmapImpl; \
		MwLLPixmapUpdate  = MwLLPixmapUpdateImpl; \
		MwLLDestroyPixmap = MwLLDestroyPixmapImpl; \
		MwLLDrawPixmap	  = MwLLDrawPixmapImpl; \
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
		MwLLFocus	= MwLLFocusImpl; \
		MwLLGrabPointer = MwLLGrabPointerImpl; \
\
		MwLLSetClipboard = MwLLSetClipboardImpl; \
		MwLLGetClipboard = MwLLGetClipboardImpl; \
\
		MwLLGetCursorCoord = MwLLGetCursorCoordImpl; \
		MwLLGetScreenSize  = MwLLGetScreenSizeImpl; \
\
		return 0; \
	}
