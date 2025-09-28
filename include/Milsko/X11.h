/* $Id$ */
#ifndef __MILSKO_X11_H__
#define __MILSKO_X11_H__

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
}* HMILSKOLL;

typedef struct _MilskoColor {
	unsigned long pixel;
}* HMILSKOCOLOR;

#endif
