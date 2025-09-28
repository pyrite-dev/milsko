/* $Id$ */
#ifndef __MILSKO_X11_H__
#define __MILSKO_X11_H__

typedef struct _MilskoLowLevel* MilskoLL;
typedef struct _MilskoColor*	MilskoLLColor;

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

#include <windows.h>

typedef struct _MilskoLowLevel {
	void* user;

	void (*draw)(MilskoLL handle);
}* MilskoLL;

typedef struct _MilskoColor {
	int	      red;
	int	      green;
	int	      blue;
}* MilskoLLColor;

#endif
