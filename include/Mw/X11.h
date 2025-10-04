/* $Id$ */
/*!
 * %file Mw/X11.h
 * %brief X11 backend
 * %warning This is used internally
 */
#ifndef __MW_X11_H__
#define __MW_X11_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xrender.h>

struct _MwLL {
	Display* display;
	Window	 window;
	Pixmap	 pixmap;
	GC	 gc;
	Colormap colormap;
	void*	 user;
	Atom	 wm_delete;
	int	 copy_buffer;

	unsigned int width;
	unsigned int height;

	MwLLHandler handler;
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

	int		use_shm;
	XShmSegmentInfo shm;
	Display*	display;
	XImage*		image;
};

#endif
