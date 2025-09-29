/* $Id$ */
#ifndef __MW_X11_H__
#define __MW_X11_H__

#include <Mw/MachDep.h>

typedef struct _MwLL *	   MwLL, MwLLRec;
typedef struct _MwLLColor *MwLLColor, MwLLColorRec;

#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <X11/X.h>
#include <X11/Xutil.h>

struct _MwLL {
	Display* display;
	Window	 window;
	GC	 gc;
	Colormap colormap;
	void*	 user;
	Atom	 wm_delete;

	MwLLHandler handler;
};

struct _MwLLColor {
	unsigned long pixel;
	int	      red;
	int	      green;
	int	      blue;
};

#endif
