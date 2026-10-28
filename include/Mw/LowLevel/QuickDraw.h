/*!
 * @file Mw/LowLevel/QuickDraw.h
 * @brief QuickDraw (classic mac os) Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_QUICKDRAW_H__
#define __MW_LOWLEVEL_QUICKDRAW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Classic Mac OS has no child windows, so only toplevel handles own a WindowPtr.
 * Children are rectangles inside their toplevel, drawn into the toplevel's
 * offscreen GWorld and clipped like WS_CLIPCHILDREN | WS_CLIPSIBLINGS would. */
struct _MwLLQuickdraw {
	struct _MwLLCommon common;

	MwLL  parent;
	MwLL* children;

	WindowPtr window;
	long	  window_serial;
	GWorldPtr gworld;
	Str255	  title;

	/* relative to parent for children, global content origin for toplevels */
	int x;
	int y;
	int width;
	int height;

	int shown;
	int drawing;

	int borderless;
	int tool;
	int min_w, min_h, max_w, max_h;

	int    has_clip;
	MwRect clip;

	int    has_cursor;
	Cursor cursor;
	Cursor cursor_drag;

	int grabbed;
	int accepts_dnd;
	int setting_drag_cursor;
	int do_drag_cursor;

	/* proxy icon */
	IconRef icon;
	OSType	icon_creator;
	OSType	icon_type;
};

struct _MwLLQuickdrawColor {
	struct _MwLLCommonColor common;

	RGBColor rgb;
};

struct _MwLLQuickdrawPixmap {
	struct _MwLLCommonPixmap common;

	PixMap	       pixmap;
	BitMap	       mask;
	int	       opaque;
	unsigned char* pixels;
};

MWDECL int	 MwLLQuickdrawCallInit(void);
MWDECL WindowPtr MwLLQuickdrawGetWindow(MwLL handle, MwRect* rect, long* serial);
MWDECL void	 MwLLQuickdrawGetVisibleRegion(MwLL handle, RgnHandle rgn);
MWDECL void	 MwLLQuickdrawAlert(const char* text);
MWDECL int	 MwLLQuickdrawBeginText(MwLL handle);
MWDECL void	 MwLLQuickdrawEndText(MwLL handle);
MWDECL void*	 MwLLQuickdrawAllocBig(size_t size);
MWDECL void	 MwLLQuickdrawFreeBig(void* ptr);

MWDECL int MwMacRomanToUTF32(unsigned char c);
MWDECL int MwUTF32ToMacRoman(int cp);

#ifdef __cplusplus
}
#endif

#endif
