/* $Id$ */
#ifndef __MILSKO_X11_H__
#define __MILSKO_X11_H__

#include <X11/X.h>
#include <X11/Xutil.h>

typedef struct __Milsko {
	Display* display;
	Window window;
	GC gc;
	Colormap colormap;
}* HMILSKO;

typedef struct __MilskoColor {
	unsigned long pixel;
}* HMILSKOCOLOR;

#endif
