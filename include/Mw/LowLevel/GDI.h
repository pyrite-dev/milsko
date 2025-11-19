/* $Id$ */
/*!
 * @file Mw/LowLevel/GDI.h
 * @brief GDI Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_GDI_H__
#define __MW_LOWLEVEL_GDI_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

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

typedef HGLRC(WINAPI* MWwglCreateContext)(HDC);
typedef BOOL(WINAPI* MWwglMakeCurrent)(HDC, HGLRC);
typedef PROC(WINAPI* MWwglGetProcAddress)(LPCSTR);
typedef BOOL(WINAPI* MWwglDeleteContext)(HGLRC);

typedef struct _MwLLGDIGL {
	HDC   dc;
	HGLRC gl;

	void* lib;

	MWwglCreateContext  wglCreateContext;
	MWwglMakeCurrent    wglMakeCurrent;
	MWwglDeleteContext  wglDeleteContext;
	MWwglGetProcAddress wglGetProcAddress;
} gdiopengl_t;

#endif
