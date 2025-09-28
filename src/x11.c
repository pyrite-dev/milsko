/* $Id$ */
#include <Milsko/Milsko.h>

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask;

HMILSKOLL MilskoLLCreate(HMILSKOLL parent, int x, int y, int width, int height) {
	HMILSKOLL    r;
	Window	     p;
	Window	     root;
	unsigned int border, depth;

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

	r->gc = XCreateGC(r->display, r->window, 0, 0);

	XSelectInput(r->display, r->window, mask);
	XMapWindow(r->display, r->window);

	return r;
}

void MilskoLLDestroy(HMILSKOLL handle) {
	XFreeGC(handle->display, handle->gc);
	XDestroyWindow(handle->display, handle->window);
	free(handle);
}

void MilskoLLPolygon(HMILSKOLL handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color) {
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

HMILSKOCOLOR MilskoLLAllocColor(HMILSKOLL handle, int r, int g, int b) {
	HMILSKOCOLOR c = malloc(sizeof(*c));
	XColor	     xc;
	xc.red	 = 256 * r;
	xc.green = 256 * g;
	xc.blue	 = 256 * b;
	XAllocColor(handle->display, handle->colormap, &xc);

	c->pixel = xc.pixel;
	return c;
}

void MilskoLLGetXYWH(HMILSKOLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Window	     root;
	unsigned int border, depth;

	XGetGeometry(handle->display, handle->window, &root, x, y, w, h, &border, &depth);

	handle->x      = *x;
	handle->y      = *y;
	handle->width  = *w;
	handle->height = *h;
}

void MilskoLLSetXY(HMILSKOLL handle, int x, int y) {
	XMoveWindow(handle->display, handle->window, x, y);
}

void MilskoLLSetWH(HMILSKOLL handle, int w, int h) {
	XResizeWindow(handle->display, handle->window, w, h);
}

void MilskoLLFreeColor(HMILSKOCOLOR color) {
	free(color);
}

int MilskoLLPending(HMILSKOLL handle) {
	XEvent ev;
	if(XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		XPutBackEvent(handle->display, &ev);
		return 1;
	}
	return 0;
}

void MilskoLLNextEvent(HMILSKOLL handle) {
	XEvent ev;
	if(XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
	}
}

void MilskoLLSleep(int ms) {
	usleep(ms * 1000);
}

void MilskoLLSetTitle(HMILSKOLL handle, const char* title) {
	XSetStandardProperties(handle->display, handle->window, title, "Milsko Widget Toolkit", None, (char**)NULL, 0, NULL);
}
