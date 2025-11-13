/* $Id$ */
#ifndef __GDI_H__
#define __GDI_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <windows.h>
#ifndef GWLP_USERDATA
#define GWLP_USERDATA GWL_USERDATA
#define GWLP_WNDPROC GWL_WNDPROC
#define GCLP_HICON GCL_HICON
#define GCLP_HCURSOR GCL_HCURSOR
#define SetWindowLongPtr SetWindowLong
#define GetWindowLongPtr GetWindowLong
#define SetClassLongPtr SetClassLong
#define GetClassLongPtr GetClassLong
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020a
#define GET_WHEEL_DELTA_WPARAM(x) ((short)HIWORD(x))
#endif

struct _MwLLGDI {
	struct _MwLLCommon common;

	HINSTANCE hInstance;
	HWND	  hWnd;
	HDC	  hDC;
	HCURSOR	  cursor;

	int grabbed;
};

struct _MwLLGDIColor {
	struct _MwLLCommonColor common;

	HBRUSH brush;
};

struct _MwLLGDIPixmap {
	struct _MwLLCommonPixmap common;

	RGBQUAD* quad;
	HBITMAP	 hBitmap;
	HBITMAP	 hMask;
	HBITMAP	 hMask2;
};

#endif
