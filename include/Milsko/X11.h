/* $Id$ */
#ifndef __MILSKO_X11_H__
#define __MILSKO_X11_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoLL *     MilskoLL, MilskoLLRec;
typedef struct _MilskoLLColor *MilskoLLColor, MilskoLLColorRec;

#include <Milsko/TypeDefs.h>
#include <Milsko/LowLevel.h>

#include <X11/X.h>
#include <X11/Xutil.h>

struct _MilskoLL {
	Display*     display;
	Window	     window;
	GC	     gc;
	Colormap     colormap;
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
	void*	     user;

	MilskoLLCallback callback;
};

struct _MilskoLLColor {
	unsigned long pixel;
	int	      red;
	int	      green;
	int	      blue;
};

#endif
