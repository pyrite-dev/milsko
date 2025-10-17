/* $Id$ */
#ifndef __X11_H__
#define __X11_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/Xrender.h>

struct _MwLL {
	Display*     display;
	Window	     window;
	Pixmap	     pixmap;
	GC	     gc;
	Colormap     colormap;
	void*	     user;
	Atom	     wm_delete;
	int	     copy_buffer;
	XIM	     xim;
	XIC	     xic;
	XVisualInfo* visual;

	unsigned int width;
	unsigned int height;

	MwLLHandler handler;

	int grabbed;
};

struct _MwLLColor {
	unsigned long pixel;
	int	      red;
	int	      green;
	int	      blue;
};

struct _MwLLPixmap {
	int	       width;
	int	       height;
	unsigned char* data;

	int depth;

	int	 use_render;
	Display* display;
	XImage*	 image;
	XImage*	 mask;
};

#endif
