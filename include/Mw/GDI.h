/* $Id$ */
#ifndef __MW_GDI_H__
#define __MW_GDI_H__

#include <Mw/MachDep.h>

typedef struct _MwLL *	   MwLL, MwLLRec;
typedef struct _MwLLColor *MwLLColor, MwLLColorRec;

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

#endif
