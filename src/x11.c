/* $Id$ */
#include <Mw/Milsko.h>

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask;

MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height) {
	MwLL   r;
	Window p;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

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

void MwLLDestroy(MwLL handle) {
	MwLLDestroyCommon(handle);

	XFreeGC(handle->display, handle->gc);
	XDestroyWindow(handle->display, handle->window);
	free(handle);
}

void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
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

MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	XColor	  xc;

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	xc.red	 = 256 * r;
	xc.green = 256 * g;
	xc.blue	 = 256 * b;
	XAllocColor(handle->display, handle->colormap, &xc);

	c->pixel = xc.pixel;
	c->red	 = r;
	c->green = g;
	c->blue	 = b;
	return c;
}

void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Window	     root;
	unsigned int border, depth;

	XGetGeometry(handle->display, handle->window, &root, x, y, w, h, &border, &depth);
}

void MwLLSetXY(MwLL handle, int x, int y) {
	XMoveWindow(handle->display, handle->window, x, y);
}

void MwLLSetWH(MwLL handle, int w, int h) {
	XResizeWindow(handle->display, handle->window, w, h);
}

void MwLLFreeColor(MwLLColor color) {
	free(color);
}

int MwLLPending(MwLL handle) {
	XEvent ev;
	if(XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		XPutBackEvent(handle->display, &ev);
		return 1;
	}
	return 0;
}

void MwLLNextEvent(MwLL handle) {
	XEvent ev;
	if(XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		if(ev.type == Expose) {
			MwLLDispatch(handle, draw);
		} else if(ev.type == ButtonPress) {
			if(ev.xbutton.button == Button1) {
				MwLLDispatch(handle, down);
				MwLLDispatch(handle, draw);
			}
		} else if(ev.type == ButtonRelease) {
			if(ev.xbutton.button == Button1) {
				MwLLDispatch(handle, up);
				MwLLDispatch(handle, draw);
			}
		}
	}
}

void MwLLSleep(int ms) {
	usleep(ms * 1000);
}

void MwLLSetTitle(MwLL handle, const char* title) {
	XSetStandardProperties(handle->display, handle->window, title, "Mw Widget Toolkit", None, (char**)NULL, 0, NULL);
}
