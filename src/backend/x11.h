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
	Display* display;
	Window	 window;
	Pixmap	 pixmap;
	GC	 gc;
	Colormap colormap;
	void*	 user;
	Atom	 wm_delete;
	int	 copy_buffer;
	XIM	 xim;
	XIC	 xic;

	unsigned int width;
	unsigned int height;

	MwLLHandler handler;

	int grabbed;

	unsigned long red_mask;
	unsigned long red_max;
	unsigned long red_shift;

	unsigned long green_mask;
	unsigned long green_max;
	unsigned long green_shift;

	unsigned long blue_mask;
	unsigned long blue_max;
	unsigned long blue_shift;
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

#ifdef HAS_FREETYPE
void print_ft_error(void* ftLib, FT_Error err);
#define MwFreeTypeFontSize 181388
extern FT_Byte MwFreeTypeFontData[MwFreeTypeFontSize];

#define FT_WITH_FUNC(handle, func, block) \
	{ \
		_##func = dlsym(handle->ftLib, #func); \
		if(_##func != NULL) { \
			block \
		} else \
			printf("[WARNING] Unable to resolve function " #func ".\n"); \
	}
#endif

#endif
