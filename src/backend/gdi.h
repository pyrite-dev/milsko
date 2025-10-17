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

struct _MwLL {
	HINSTANCE hInstance;
	HWND	  hWnd;
	HDC	  hDC;
	void*	  user;
	int	  copy_buffer;

	MwLLHandler handler;

	int grabbed;
};

struct _MwLLColor {
	HBRUSH brush;

	int red;
	int green;
	int blue;
};

struct _MwLLPixmap {
	int width;
	int height;

	HBITMAP hBitmap;
	HBITMAP hMask;
	HBITMAP hMask2;
};

#endif
