/* $Id$ */
/*!
 * @file Mw/LowLevel/X11.h
 * @brief X11 Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_X11_H__
#define __MW_LOWLEVEL_X11_H__

#include <Mw/MachDep.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#ifdef USE_XRENDER
#include <X11/extensions/Xrender.h>
#endif

struct _MwLLX11 {
	struct _MwLLCommon common;

	unsigned int width;
	unsigned int height;

	Display* display;
	Window	 window;
	Pixmap	 pixmap;
	GC	 gc;
	Colormap colormap;
	Atom	 wm_delete;
	XIM	 xim;
	XIC	 xic;

	int top;
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

struct _MwLLX11Color {
	struct _MwLLCommonColor common;

	unsigned long pixel;
};

struct _MwLLX11Pixmap {
	struct _MwLLCommonPixmap common;

	int	       depth;
	unsigned char* data;
	MwLL	       handle;
	int	       use_xrender;
	Display*       display;
	XImage*	       image;
	XImage*	       mask;
};

typedef XVisualInfo* (*MWglXChooseVisual)(Display* dpy, int screen, int* attribList);
typedef GLXContext (*MWglXCreateContext)(Display* dpy, XVisualInfo* vis, GLXContext shareList, Bool direct);
typedef void (*MWglXDestroyContext)(Display* dpy, GLXContext ctx);
typedef Bool (*MWglXMakeCurrent)(Display* dpy, GLXDrawable drawable, GLXContext ctx);
typedef void (*MWglXSwapBuffers)(Display* dpy, GLXDrawable drawable);
typedef void* (*MWglXGetProcAddress)(const GLubyte* procname);

typedef struct _MwLLX11GL {
	struct _MwLLCommonGL common;

	XVisualInfo* visual;
	GLXContext   gl;

	MWglXChooseVisual   glXChooseVisual;
	MWglXCreateContext  glXCreateContext;
	MWglXDestroyContext glXDestroyContext;
	MWglXMakeCurrent    glXMakeCurrent;
	MWglXSwapBuffers    glXSwapBuffers;
	MWglXGetProcAddress glXGetProcAddress;
} x11opengl_t;
#endif
