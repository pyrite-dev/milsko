/* $Id$ */
#ifndef __MW_GDI_H__
#define __MW_GDI_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <windows.h>

struct _MwLL {
	HWND  hWnd;
	HDC   hDC;
	void* user;

	MwLLHandler handler;
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
};

#endif
