/* $Id$ */
#ifndef __MILSKO_GDI_H__
#define __MILSKO_GDI_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoLL *     MilskoLL, MilskoLLRec;
typedef struct _MilskoLLColor *MilskoLLColor, MilskoLLColorRec;

#include <Milsko/TypeDefs.h>
#include <Milsko/LowLevel.h>

#include <windows.h>

struct _MilskoLL {
	void* user;

	void (*draw)(MilskoLL handle);
};

struct _MilskoColor {
	int red;
	int green;
	int blue;
};

#endif
