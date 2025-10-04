/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

static int create(MwWidget handle) {
#ifdef _WIN32
#else
	XUnmapWindow(handle->lowlevel->display, handle->lowlevel->window);
#endif

	MwSetDefault(handle);

	return 0;
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
		p.y = 3;

		for(i = 0; i < arrlen(menu->sub); i++) {
			int tw = MwTextWidth(handle, menu->sub[i]->name);
			int th = MwTextHeight(handle, menu->sub[i]->name);

			if(menu->sub[i]->wsub != NULL) {
				r.x	 = 0;
				r.y	 = p.y - 3;
				r.width	 = tw + 15 + 5 * 2;
				r.height = th + 3 * 2;
				MwDrawFrame(handle, &r, base, 0);
			}

			p.x = 5 + tw / 2;

			p.y += th / 2;
			MwDrawText(handle, &p, menu->sub[i]->name, 1, text);

			if(arrlen(menu->sub[i]->sub) > 0) {
				MwRect tr;

				tr.x	 = p.x + tw / 2 + 5;
				tr.y	 = p.y - th / 2 + 2;
				tr.width = tr.height = 11;

				MwDrawTriangle(handle, &tr, base, menu->sub[i]->wsub != NULL ? 1 : 0, MwEAST);
			}

			p.y += th / 2 + 3;
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
		rc.y = 3;
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
					int	j;

					for(j = 0; j < arrlen(menu->sub); j++) {
						if(menu->sub[j]->wsub != NULL) MwDestroyWidget(menu->sub[j]->wsub);
						menu->sub[j]->wsub = NULL;
					}

					p.x = rc.x + rc.width + 3;
					p.y = rc.y - 3;

					menu->sub[i]->wsub = MwCreateWidget(MwSubMenuClass, "submenu", handle, 0, 0, 0, 0);
					MwSubMenuAppear(menu->sub[i]->wsub, menu->sub[i], &p);
					i = -1;
				} else if(menu->sub[i]->wsub != NULL && arrlen(menu->sub[i]->sub) > 0) {
					MwDestroyWidget(menu->sub[i]->wsub);
					menu->sub[i]->wsub = NULL;

					MwForceRender(handle);
				} else if(arrlen(menu->sub[i]->sub) == 0) {
					MwWidget p;

					while(w->parent->widget_class != MwMenuClass) w = w->parent;
					MwGetBeforeStep(w, &jmp);

					p = w->parent;

					MwDestroyWidget(w);

					MwForceRender(p);

					MwDispatchUserHandler(p, MwNmenuHandler, menu->sub[i]);

					longjmp(jmp, 1);

					break;
				}
			}

			rc.y += rc.height + 3;
		}
	}
}

MwClassRec MwSubMenuClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    click,	   /* click */
    NULL,	   /* parent_resize */
    NULL,	   /* mouse_move */
    MwForceRender, /* mouse_up */
    MwForceRender  /* mouse_down */
};
MwClass MwSubMenuClass = &MwSubMenuClassRec;

void MwSubMenuAppear(MwWidget handle, MwMenu menu, MwPoint* point) {
	int i, w = 0, h = 0;
#ifdef _WIN32
	RECT rc;

	SetWindowLongPtr(handle->lowlevel->hWnd, GWL_STYLE, (LONG_PTR)WS_POPUP);

	GetWindowRect(handle->parent->lowlevel->hWnd, &rc);

	SetParent(handle->lowlevel->hWnd, NULL);

	rc.left += point->x;
	rc.top += point->y;

	SetWindowPos(handle->lowlevel->hWnd, NULL, rc.left, rc.top, 1, 1, SWP_NOREDRAW);

	ShowWindow(handle->lowlevel->hWnd, SW_NORMAL);
	UpdateWindow(handle->lowlevel->hWnd);

	SetFocus(handle->lowlevel->hWnd);
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
		h += MwTextHeight(handle, menu->sub[i]->name) + 3;
		if(tw > w) {
			w = tw;
		}
	}

	w += 10 + 15;
	h += 3;

	MwVaApply(handle,
		  MwNwidth, w,
		  MwNheight, h,
		  NULL);
}
