/*!
 * @file Mw/LowLevel.h
 * @brief Low-level drawing API
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_H__
#define __MW_LOWLEVEL_H__

#include <Mw/MachDep.h>

typedef struct _MwLLHandler*	  MwLLHandler;
typedef struct _MwLLMouse	  MwLLMouse;
typedef struct _MwLLCommon*	  MwLLCommon;
typedef struct _MwLLCommonColor*  MwLLCommonColor;
typedef struct _MwLLCommonPixmap* MwLLCommonPixmap;
#ifdef _MILSKO
typedef union _MwLL*	   MwLL;
typedef union _MwLLColor*  MwLLColor;
typedef union _MwLLPixmap* MwLLPixmap;
#else
typedef void* MwLL;
typedef void* MwLLColor;
typedef void* MwLLPixmap;
#endif

enum MwLLBackends {
	MwLLBackendX11 = 0,
	MwLLBackendGDI,
	MwLLBackendWayland,
	MwLLBackendCocoa,
};

struct _MwLLCommon {
	void* user;
	int   copy_buffer;
	int   type;
	int   coordinate_type;

	MwLLHandler handler;
};

struct _MwLLCommonColor {
	int red;
	int green;
	int blue;
};

struct _MwLLCommonPixmap {
	int	       width;
	int	       height;
	unsigned char* raw;
	void*	       user;
};

#ifdef _MILSKO
#ifdef USE_X11
#include <Mw/LowLevel/X11.h>
#endif
#ifdef USE_GDI
#include <Mw/LowLevel/GDI.h>
#endif
#ifdef USE_WAYLAND
#include <Mw/LowLevel/Wayland.h>
#endif
#ifdef USE_COCOA
#include <Mw/LowLevel/Cocoa.h>
#endif

union _MwLL {
	struct _MwLLCommon common;
#ifdef USE_X11
	struct _MwLLX11 x11;
#endif
#ifdef USE_GDI
	struct _MwLLGDI gdi;
#endif
#ifdef USE_WAYLAND
	struct _MwLLWayland wayland;
#endif
#ifdef USE_COCOA
	struct _MwLLCocoa cocoa;
#endif
};

union _MwLLColor {
	struct _MwLLCommonColor common;
#ifdef USE_X11
	struct _MwLLX11Color x11;
#endif
#ifdef USE_GDI
	struct _MwLLGDIColor gdi;
#endif
#ifdef USE_WAYLAND
	struct _MwLLWaylandColor wayland;
#endif
#ifdef USE_COCOA
	struct _MwLLCocoaColor cocoa;
#endif
};

union _MwLLPixmap {
	struct _MwLLCommonPixmap common;
#ifdef USE_X11
	struct _MwLLX11Pixmap x11;
#endif
#ifdef USE_GDI
	struct _MwLLGDIPixmap gdi;
#endif
#ifdef USE_WAYLAND
	struct _MwLLWaylandPixmap wayland;
#endif
#ifdef USE_COCOA
	struct _MwLLCocoaPixmap cocoa;
#endif
};
#endif
#include <Mw/TypeDefs.h>

#define MwLLDispatch(x, y, z) \
	if(x->common.handler != NULL && x->common.handler->y != NULL) x->common.handler->y(x, z)

#define MwLLKeyMask (1 << 31)
enum MwLLKeyEnum {
	MwLLKeyBackSpace = MwLLKeyMask | 1,
	MwLLKeyLeft,
	MwLLKeyRight,
	MwLLKeyUp,
	MwLLKeyDown,
	MwLLKeyEnter,
	MwLLKeyEscape,
	MwLLKeyLeftShift,
	MwLLKeyRightShift,
	MwLLKeyAlt,
	MwLLKeyControl
};

#define MwLLControlMask MwLLKeyMask | (1 << 30)
#define MwLLAltMask MwLLKeyMask | (1 << 29)

enum MwLLMouseEnum {
	MwLLMouseLeft = 1,
	MwLLMouseMiddle,
	MwLLMouseRight,
	MwLLMouseWheelUp,
	MwLLMouseWheelDown
};

struct _MwLLMouse {
	MwPoint point;
	int	button;
};

struct _MwLLHandler {
	void (*draw)(MwLL handle, void* data);
	void (*up)(MwLL handle, void* data);
	void (*down)(MwLL handle, void* data);
	void (*resize)(MwLL handle, void* data);
	void (*close)(MwLL handle, void* data);
	void (*move)(MwLL handle, void* data);
	void (*key)(MwLL handle, void* data);
	void (*key_released)(MwLL handle, void* data);
	void (*focus_in)(MwLL handle, void* data);
	void (*focus_out)(MwLL handle, void* data);
	void (*clipboard)(MwLL handle, void* data);
};

#ifdef __cplusplus
extern "C" {
#endif

/* lowlevel.c, common part */
MWDECL void MwLLCreateCommon(MwLL handle);
MWDECL void MwLLDestroyCommon(MwLL handle);

/* driver-specific, these get assigned by backend */
MWDECL MwLL (*MwLLCreate)(MwLL parent, int x, int y, int width, int height);
MWDECL void (*MwLLDestroy)(MwLL handle);

MWDECL void (*MwLLPolygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color);
MWDECL void (*MwLLLine)(MwLL handle, MwPoint* points, MwLLColor color);
MWDECL void (*MwLLBeginDraw)(MwLL handle);
MWDECL void (*MwLLEndDraw)(MwLL handle);

MWDECL MwLLColor (*MwLLAllocColor)(MwLL handle, int r, int g, int b);
MWDECL void (*MwLLColorUpdate)(MwLL handle, MwLLColor c, int r, int g, int b);
MWDECL void (*MwLLFreeColor)(MwLLColor color);

MWDECL void (*MwLLGetXYWH)(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
MWDECL void (*MwLLSetXY)(MwLL handle, int x, int y);
MWDECL void (*MwLLSetWH)(MwLL handle, int w, int h);

MWDECL void (*MwLLSetTitle)(MwLL handle, const char* title);

MWDECL int (*MwLLPending)(MwLL handle);
MWDECL void (*MwLLNextEvent)(MwLL handle);

MWDECL MwLLPixmap (*MwLLCreatePixmap)(MwLL handle, unsigned char* data, int width, int height);
MWDECL void (*MwLLPixmapUpdate)(MwLLPixmap pixmap);
MWDECL void (*MwLLDestroyPixmap)(MwLLPixmap pixmap);
MWDECL void (*MwLLDrawPixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap);
MWDECL void (*MwLLSetIcon)(MwLL handle, MwLLPixmap pixmap);

MWDECL void (*MwLLForceRender)(MwLL handle);

MWDECL void (*MwLLSetCursor)(MwLL handle, MwCursor* image, MwCursor* mask);
MWDECL void (*MwLLDetach)(MwLL handle, MwPoint* point);
MWDECL void (*MwLLShow)(MwLL handle, int show);

MWDECL void (*MwLLSetSizeHints)(MwLL handle, int minx, int miny, int maxx, int maxy);
MWDECL void (*MwLLMakeBorderless)(MwLL handle, int toggle);
MWDECL void (*MwLLMakeToolWindow)(MwLL handle);
MWDECL void (*MwLLMakePopup)(MwLL handle, MwLL parent);

MWDECL void (*MwLLBeginStateChange)(MwLL handle);
MWDECL void (*MwLLEndStateChange)(MwLL handle);

MWDECL void (*MwLLFocus)(MwLL handle);
MWDECL void (*MwLLGrabPointer)(MwLL handle, int toggle);

MWDECL void (*MwLLSetClipboard)(MwLL handle, const char* text);
MWDECL void (*MwLLGetClipboard)(MwLL handle);

MWDECL void (*MwLLGetCursorCoord)(MwLL handle, MwPoint* point);
MWDECL void (*MwLLGetScreenSize)(MwLL handle, MwRect* rect);

#ifdef __cplusplus
}
#endif

#endif
