#include <Mw/Milsko.h>

MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {}

void MwLLDestroyImpl(MwLL handle) {}

void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {}

void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {}

WDECL void MwLLBeginDrawImpl(MwLL handle) {}

WDECL void MwLLEndDrawImpl(MwLL handle) {}

MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {}

void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {}

void MwLLFreeColorImpl(MwLLColor color) {}

void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {}

void MwLLSetXYImpl(MwLL handle, int x, int y) {}

void MwLLSetWHImpl(MwLL handle, int w, int h) {}

void MwLLSetTitleImpl(MwLL handle, const char* title) {}

int MwLLPendingImpl(MwLL handle) {}

void MwLLNextEventImpl(MwLL handle) {}

MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {}

void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {}

void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {}

void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {}

void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {}

void MwLLForceRenderImpl(MwLL handle) {}

void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {}

void MwLLDetachImpl(MwLL handle, MwPoint* point) {}

void MwLLShowImpl(MwLL handle, int show) {}

void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {}

void MwLLMakeBorderlessImpl(MwLL handle, int toggle){}

void MwLLMakeToolWindowImpl(MwLL handle){}

void MwLLMakePopupImpl(MwLL handle, MwLL parent){}

void MwLLBeginStateChangeImpl(MwLL handle){}

void MwLLEndStateChangeImpl(MwLL handle){}

void MwLLFocusImpl(MwLL handle){}

void MwLLGrabPointerImpl(MwLL handle, int toggle){}

void MwLLSetClipboardImpl(MwLL handle, const char* text, int clipboard_type){}

void MwLLGetClipboardImpl(MwLL handle, int clipboard_type){}

void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point){}

void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect){}

void MwLLSetDarkThemeImpl(MwLL handle, int toggle){}

MwBool MwLLDoModernImpl(MwLL handle){}

#include "call.c"
CALL(Haiku);
