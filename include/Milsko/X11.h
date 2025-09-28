/* $Id$ */
#ifndef __MILSKO_X11_H__
#define __MILSKO_X11_H__

typedef struct _MilskoLowLevel* MilskoLL;
typedef struct _MilskoColor*	MilskoLLColor;

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

#include <X11/X.h>
#include <X11/Xutil.h>

typedef struct _MilskoLowLevel {
	Display*     display;
	Window	     window;
	GC	     gc;
	Colormap     colormap;
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
	void*	     user;

	void (*draw)(MilskoLL handle);
}* MilskoLL;

typedef struct _MilskoColor {
	unsigned long pixel;
}* MilskoLLColor;

#endif
