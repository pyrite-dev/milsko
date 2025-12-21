#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int create(MwWidget handle) {
	MwLLBeginStateChange(handle->lowlevel);

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

	menu->wsub = NULL;
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

	MwDrawWidgetBack(handle, &r, base, 0, MwTRUE);

	if(menu != NULL) {
		MwPoint p;
		int	i;

		p.x = 0;
		p.y = 3;

		for(i = 0; i < arrlen(menu->sub); i++) {
			if(strcmp(menu->sub[i]->name, "----") == 0) {
				MwRect rc;

				p.y += 1;

				rc.x	  = MwDefaultBorderWidth(handle) * 2;
				rc.y	  = p.y;
				rc.width  = r.width - (rc.x * 2);
				rc.height = MwDefaultBorderWidth(handle) * 2;

				MwDrawFrame(handle, &rc, base, 1);

				p.y += MwDefaultBorderWidth(handle) * 2 + 1;
			} else {
				int tw = MwTextWidth(handle, menu->sub[i]->name);
				int th = MwTextHeight(handle, menu->sub[i]->name);

				if(menu->sub[i]->wsub != NULL) {
					r.x	 = 0;
					r.y	 = p.y - 3;
					r.width	 = tw + 15 + 5 * 2;
					r.height = th + 3 * 2;
					MwDrawWidgetBack(handle, &r, base, 0, MwTRUE);
				}

				p.x = 5 + tw / 2;

				p.y += th / 2;
				MwDrawText(handle, &p, menu->sub[i]->name, menu->sub[i]->wsub != NULL ? 1 : 0, MwALIGNMENT_CENTER, text);

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
					MwSubMenuAppear(menu->sub[i]->wsub, menu->sub[i], &p, 0);
					i = -1;
				} else if(menu->sub[i]->wsub != NULL && arrlen(menu->sub[i]->sub) > 0) {
					while(w->parent->widget_class == MwSubMenuClass) w = w->parent;

					MwDestroyWidget(menu->sub[i]->wsub);
					menu->sub[i]->wsub = NULL;

					MwForceRender(w->parent);

					MwForceRender(handle);
				} else if(strcmp(menu->sub[i]->name, "----") != 0 && arrlen(menu->sub[i]->sub) == 0) {
					while(w->parent->widget_class == MwSubMenuClass) w = w->parent;
					MwGetBeforeStep(w, &jmp);

					MwDestroyWidget(w);
					((MwMenu)w->internal)->wsub = NULL;

					MwForceRender(w->parent);

					MwDispatchUserHandler(w->parent, MwNmenuHandler, menu->sub[i]);
				}
			}

			rc.y += rc.height + 3;
		}
	}
}

static void mwSubMenuAppearImpl(MwWidget handle, MwMenu menu, MwPoint* point, int diff_calc) {
	int	i, w = 0, h = 0;
	MwRect	rc;
	MwPoint p = *point;

	MwGetScreenSize(handle, &rc);

	handle->internal = menu;

	for(i = 0; i < arrlen(menu->sub); i++) {
		if(strcmp(menu->sub[i]->name, "----") == 0) {
			h += MwDefaultBorderWidth(handle) * 2 + 2;
		} else {
			int tw = MwTextWidth(handle, menu->sub[i]->name);
			h += MwTextHeight(handle, menu->sub[i]->name) + 3;
			if(tw > w) {
				w = tw;
			}
		}
	}

	w += 10 + 15;
	h += 3;

	if(diff_calc) {
		p.y = rc.height - p.y - h;
	}

	MwLLMakeToolWindow(handle->lowlevel);
	MwLLDetach(handle->lowlevel, &p);

	if(MwGetInteger(handle, MwNy) + h > rc.height) {
		MwVaApply(handle,
			  MwNy, rc.height - h,
			  NULL);
	}
	MwLLEndStateChange(handle->lowlevel);

	MwVaApply(handle,
		  MwNwidth, w,
		  MwNheight, h,
		  NULL);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	(void)out;

	if(strcmp(name, "mwSubMenuAppear") == 0) {
		MwMenu	 menu	   = va_arg(va, MwMenu);
		MwPoint* point	   = va_arg(va, MwPoint*);
		int	 diff_calc = va_arg(va, int);
		mwSubMenuAppearImpl(handle, menu, point, diff_calc);
	}
}

MwClassRec MwSubMenuClassRec = {
    create,	    /* create */
    destroy,	    /* destroy */
    draw,	    /* draw */
    click,	    /* click */
    NULL,	    /* parent_resize */
    NULL,	    /* prop_change */
    NULL,	    /* mouse_move */
    MwForceRender2, /* mouse_up */
    MwForceRender2, /* mouse_down */
    NULL,	    /* key */
    func_handler,   /* execute */
    NULL,	    /* tick */
    NULL,	    /* resize */
    NULL,	    /* children_update */
    NULL,	    /* children_prop_change */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwSubMenuClass = &MwSubMenuClassRec;
