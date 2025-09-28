/* $Id$ */
#include <Milsko/Milsko.h>

#include <stdlib.h>

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask;

HMILSKO MilskoLLCreate(HMILSKO parent, int x, int y, int width, int height) {
	HMILSKO r;
	Window	p;

	r	  = malloc(sizeof(*r));
	r->x	  = x;
	r->y	  = y;
	r->width  = width;
	r->height = height;

	if(parent == NULL) {
		r->display = XOpenDisplay(NULL);
		p	   = XRootWindow(r->display, XDefaultScreen(r->display));
	} else {
		r->display = parent->display;
		p	   = parent->window;
	}
	r->window   = XCreateSimpleWindow(r->display, p, x, y, width, height, 0, 0, WhitePixel(r->display, XDefaultScreen(r->display)));
	r->colormap = DefaultColormap(r->display, XDefaultScreen(r->display));
	r->gc	    = XCreateGC(r->display, r->window, 0, 0);

	XSelectInput(r->display, r->window, mask);
	XMapWindow(r->display, r->window);

	return r;
}

void MilskoLLDestroy(HMILSKO handle) {
	XFreeGC(handle->display, handle->gc);
	XDestroyWindow(handle->display, handle->window);
	free(handle);
}

void MilskoLLPolygon(HMILSKO handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color) {
	int	i;
	XPoint* p = malloc(sizeof(*p) * points_count);

	XSetForeground(handle->display, handle->gc, color->pixel);

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	XFillPolygon(handle->display, handle->window, handle->gc, p, points_count, Convex, CoordModeOrigin);

	free(p);
}

HMILSKOCOLOR MilskoLLAllocColor(HMILSKO handle, int r, int g, int b) {
	HMILSKOCOLOR c = malloc(sizeof(*c));
	XColor	     xc;
	xc.red	 = 256 * r;
	xc.green = 256 * g;
	xc.blue	 = 256 * b;
	XAllocColor(handle->display, handle->colormap, &xc);

	c->pixel = xc.pixel;
	return c;
}

void MilskoLLGetXYWH(HMILSKO handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	*x = handle->x;
	*y = handle->y;
	*w = handle->width;
	*h = handle->height;
}

void MilskoLLFreeColor(HMILSKOCOLOR color) {
	free(color);
}

int MilskoLLPending(HMILSKO handle) {
	XEvent ev;
	if(XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		XPutBackEvent(handle->display, &ev);
		return 1;
	}
	return 0;
}
