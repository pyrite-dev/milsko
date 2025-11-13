/* $Id$ */
/* This file gets included by backend */
#define CALL(NAME) \
	static int _MwLL##NAME##CallInit(void) { \
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
		MwLLMakePopup = MwLLMakePopupImpl; \
\
		MwLLSetSizeHints   = MwLLSetSizeHintsImpl; \
		MwLLMakeBorderless = MwLLMakeBorderlessImpl; \
\
		MwLLSetBackground = MwLLSetBackgroundImpl; \
\
		MwLLFocus	= MwLLFocusImpl; \
		MwLLGrabPointer = MwLLGrabPointerImpl; \
\
		MwLLSetClipboard = MwLLSetClipboardImpl; \
		MwLLGetClipboard = MwLLGetClipboardImpl; \
\
		MwLLMakeToolWindow = MwLLMakeToolWindowImpl; \
\
		return 0; \
	} \
	int (*MwLL##NAME##CallInit)(void) = _MwLL##NAME##CallInit;
