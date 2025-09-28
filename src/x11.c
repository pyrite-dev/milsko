/* $Id$ */
#include <Milsko/Milsko.h>

HMILSKO MilskoLLCreate(HMILSKO parent, int x, int y, int width, int height){
	HMILSKO r;
	Window p;

	r = malloc(sizeof(*r));
	if(parent == NULL){
		r->display = XOpenDisplay(NULL);
		p = XRootWindow(r->display, XDefaultScreen(r->display));
	}else{
		r->display = parent->display;
		p = parent->window;
	}
	r->window = XCreateSimpleWindow(r->display, p, x, y, width, height, 0, 0, WhitePixel(r->display, XDefaultScreen(r->display)));
	r->colormap = DefaultColormap(r->display, XDefaultScreen(r->display));
	r->gc = XCreateGC(r->display, r->window, 0, 0);

	XMapWindow(r->display, r->window);

	return r;
}

void MilskoLLPolygon(HMILSKO handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color){
}

HMILSKOCOLOR MilskoLLAllocColor(HMILSKO handle, int r, int g, int b){
	HMILSKOCOLOR c = malloc(sizeof(*c));
	XColor xc;
	xc.red = 256 * r;
	xc.green = 256 * g;
	xc.blue = 256 * b;
	XAllocColor(handle->display, handle->colormap, &xc);

	c->pixel = xc.pixel;
	return c;
}

void MilskoLLFreeColor(HMILSKOCOLOR color){
	free(color);
}
