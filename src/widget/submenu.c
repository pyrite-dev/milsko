/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

static void create(MwWidget handle) {
#ifdef _WIN32
#else
	XUnmapWindow(handle->lowlevel->display, handle->lowlevel->window);
#endif

	MwSetDefault(handle);
}

static void null_all(MwMenu menu) {
	int i;
	for(i = 0; i < arrlen(menu->sub); i++) {
		null_all(menu->sub[i]);
	}
	menu->wsub = NULL;
}

static void destroy(MwWidget handle) {
	MwMenu menu = handle->internal;

	null_all(menu);
}

static void draw(MwWidget handle) {
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	  r;
	MwMenu	  menu = handle->internal;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, 0);
	MwDrawRect(handle, &r, base);

	if(menu != NULL) {
		MwPoint p;
		int	i;

		p.x = 0;
		p.y = 5;

		for(i = 0; i < arrlen(menu->sub); i++) {
			int tw = MwTextWidth(handle, menu->sub[i]->name);
			int th = MwTextHeight(handle, menu->sub[i]->name);

			p.x = 5 + tw / 2;

			if(menu->sub[i]->wsub == NULL && arrlen(menu->sub[i]->sub) > 0) {
				MwPoint p2 = p;
				p2.x += tw / 2 + 5;
				p2.y -= 5;

				menu->sub[i]->wsub = MwCreateWidget(MwSubMenuClass, "submenu", handle, 0, 0, 0, 0);
				MwSubMenuAppear(menu->sub[i]->wsub, menu->sub[i], &p2);
			}

			p.y += th / 2;
			MwDrawText(handle, &p, menu->sub[i]->name, 1, text);
			p.y += th / 2;
		}
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

MwClassRec MwSubMenuClassRec = {
    create,  /* create */
    destroy, /* destroy */
    draw,    /* draw */
    NULL,    /* click */
    NULL     /* parent_resize */
};
MwClass MwSubMenuClass = &MwSubMenuClassRec;

void MwSubMenuAppear(MwWidget handle, MwMenu menu, MwPoint* point) {
	int i, w = 0, h = 0;
#ifdef _WIN32
#else
	Atom		     wndtype = XInternAtom(handle->lowlevel->display, "_NET_WM_WINDOW_TYPE", False);
	Atom		     wndmenu = XInternAtom(handle->lowlevel->display, "_NET_WM_WINDOW_TYPE_MENU", False);
	int		     x = 0, y = 0;
	Window		     child;
	XSetWindowAttributes xswa;

	xswa.override_redirect = True;

	XTranslateCoordinates(handle->parent->lowlevel->display, handle->parent->lowlevel->window, RootWindow(handle->parent->lowlevel->display, DefaultScreen(handle->parent->lowlevel->display)), 0, 0, &x, &y, &child);

	XReparentWindow(handle->lowlevel->display, handle->lowlevel->window, RootWindow(handle->lowlevel->display, DefaultScreen(handle->lowlevel->display)), x + point->x, y + point->y);

	XChangeWindowAttributes(handle->lowlevel->display, handle->lowlevel->window, CWOverrideRedirect, &xswa);
	XChangeProperty(handle->lowlevel->display, handle->lowlevel->window, wndtype, 4, 32, PropModeReplace, (unsigned char*)&wndmenu, 1);

	XMapWindow(handle->lowlevel->display, handle->lowlevel->window);
	XSetInputFocus(handle->lowlevel->display, handle->lowlevel->window, RevertToNone, CurrentTime);
#endif

	handle->internal = menu;

	for(i = 0; i < arrlen(menu->sub); i++) {
		int tw = MwTextWidth(handle, menu->sub[i]->name);
		h += MwTextHeight(handle, menu->sub[i]->name);
		if(tw > w) {
			w = tw;
		}
	}

	w += 10;
	h += 10;

	MwVaApply(handle,
		  MwNwidth, w,
		  MwNheight, h,
		  NULL);
}
