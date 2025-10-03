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

			if(menu->sub[i]->wsub != NULL){
				r.x = 0;
				r.y = p.y - 3;
				r.width = tw + 15 + 5 * 2;
				r.height = th + 3 * 2;
				MwDrawFrame(handle, &r, base, 0);
			}

			p.x = 5 + tw / 2;

			p.y += th / 2;
			MwDrawText(handle, &p, menu->sub[i]->name, 1, text);

			if(arrlen(menu->sub[i]->sub) > 0){
				MwPoint pl[3];

				p.x = 5 + tw + 10;

				pl[0].x = p.x - 5;
				pl[0].y = p.y - th / 2;

				pl[1].x = p.x - 5;
				pl[1].y = p.y + th / 2;

				pl[2].x = p.x + 5;
				pl[2].y = p.y;

				MwLLPolygon(handle->lowlevel, pl, 3, text);
			}

			p.y += th / 2 + 5;
		}
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwWidget w = handle;
	jmp_buf	 jmp;
	MwMenu	 menu = handle->internal;

	if(arrlen(menu->sub) > 0) {
		int    ww = 0, i;
		MwRect rc;

		rc.x = 5;
		rc.y = 5;
		for(i = 0; i < arrlen(menu->sub); i++) {
			int tw = MwTextWidth(handle, menu->sub[i]->name);
			if(tw > ww) ww = tw;
		}

		rc.width = ww + 15;
		for(i = 0; i < arrlen(menu->sub); i++) {
			int th	  = MwTextHeight(handle, menu->sub[i]->name);
			rc.height = th;

			if(rc.x <= handle->mouse_point.x && rc.y <= handle->mouse_point.y && handle->mouse_point.x <= (int)(rc.x + rc.width) && handle->mouse_point.y <= (int)(rc.y + rc.height)) {
				if(menu->sub[i]->wsub == NULL && arrlen(menu->sub[i]->sub) > 0) {
					MwPoint p;
					int j;

					for(j = 0; j < arrlen(menu->sub); j++){
						if(menu->sub[j]->wsub != NULL) MwDestroyWidget(menu->sub[j]->wsub);
						menu->sub[j]->wsub = NULL;
					}

					p.x = rc.x + rc.width + 5;
					p.y = rc.y - 5;

					menu->sub[i]->wsub = MwCreateWidget(MwSubMenuClass, "submenu", handle, 0, 0, 0, 0);
					MwSubMenuAppear(menu->sub[i]->wsub, menu->sub[i], &p);
					i = -1;
				}else if(arrlen(menu->sub[i]->sub) == 0){
					while(w->parent->widget_class != MwMenuClass) w = w->parent;
					MwGetBeforeStep(w, &jmp);
			
					MwDestroyWidget(w);

					MwLLForceRender(w->parent->lowlevel);

					MwDispatchUserHandler(w->parent, MwNmenuHandler, menu->sub[i]);
			
					longjmp(jmp, 1);

					break;
				}
			}

			rc.y += rc.height + 5;
		}
	}
}

MwClassRec MwSubMenuClassRec = {
    create,  /* create */
    destroy, /* destroy */
    draw,    /* draw */
    click,   /* click */
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
		h += MwTextHeight(handle, menu->sub[i]->name) + 5;
		if(tw > w) {
			w = tw;
		}
	}

	w += 10 + 15;
	h += 5;

	MwVaApply(handle,
		  MwNwidth, w,
		  MwNheight, h,
		  NULL);
}
