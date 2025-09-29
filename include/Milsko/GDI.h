/* $Id$ */
#ifndef __MILSKO_GDI_H__
#define __MILSKO_GDI_H__

#include <Mw/MachDep.h>

typedef struct _MwLL *     MwLL, MwLLRec;
typedef struct _MwLLColor *MwLLColor, MwLLColorRec;

#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <windows.h>

struct _MwLL {
	void* user;

	MwLLHandler handler;
};

struct _MwColor {
	int red;
	int green;
	int blue;
};

#endif
