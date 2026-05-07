#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static void resize(MwWidget handle);

static int wcreate(MwWidget handle) {
	MwTab t = malloc(sizeof(*t));
	memset(t, 0, sizeof(*t));

	handle->internal = t;

	MwSetDefault(handle);

	MwSetInteger(handle, MwNvalue, -1);

	return 0;
}

static void destroy(MwWidget handle) {
	MwTab t = handle->internal;
	int   i;

	for(i = 0; i < arrlen(t->names); i++) {
		free(t->names[i]);
	}

	arrfree(t->names);
	arrfree(t->frames);

	free(handle->internal);
}

static int tab_width(MwWidget handle, const char* text) {
	return MwTextWidth(handle, NULL, text) + 20;
}

static int tab_height(MwWidget handle) {
	return MwTextHeight(handle, NULL, "M") + 10;
}

static void draw(MwWidget handle) {
	MwTab	  t  = handle->internal;
	MwLLColor c  = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor ct = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	  r, r2;
	int	  h = tab_height(handle);
	int	  i;
	int	  n;
	int	  x = 0;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, c);

	for(n = 0; n < 2; n++) {
		x = 0;
		for(i = 0; i < arrlen(t->names); i++) {
			int render = 0;

			if(i == MwGetInteger(handle, MwNvalue) && n == 1) {
				r.y += h;
				r.height -= h;
				MwDrawFrame(handle, &r, c, 0);

				render = 1;
			} else if(n == 0) {
				render = 1;
			}

			r2.x	  = x;
			r2.y	  = 0;
			r2.width  = tab_width(handle, t->names[i]);
			r2.height = tab_height(handle) + MwDefaultBorderWidth(handle);

			x += r2.width;

			if(render) {
				MwPoint p;

				if(n == 1 && i > 0) {
					MwRect r3 = r2;

					r3.y	 = tab_height(handle);
					r3.width = r3.height = MwDefaultBorderWidth(handle);

					MwLLClip(handle->lowlevel, &r3);

					r3.x -= r3.width;
					r3.y -= r3.height;
					r3.width *= 2;
					r3.height *= 2;
					MwDrawFrame(handle, &r3, c, 1);

					MwLLClip(handle->lowlevel, NULL);
				}

				if(n == 1 && i < (arrlen(t->names) - 1)) {
					MwRect r3 = r2;

					r3.x += r2.width - MwDefaultBorderWidth(handle);
					r3.y	 = tab_height(handle);
					r3.width = r3.height = MwDefaultBorderWidth(handle);

					MwLLClip(handle->lowlevel, &r3);

					r3.y -= r3.height;
					r3.width *= 2;
					r3.height *= 2;
					MwDrawFrame(handle, &r3, c, 1);

					MwLLClip(handle->lowlevel, NULL);
				}

				if(n == 1) {
					MwRect r3 = r2;

					r3.height -= MwDefaultBorderWidth(handle);

					MwLLClip(handle->lowlevel, &r3);
				}

				MwDrawFrame(handle, &r2, c, 0);

				MwLLClip(handle->lowlevel, NULL);

				if(n == 1) {
					MwRect r3 = r2;

					r3.y += r3.height;
					r3.height = MwDefaultBorderWidth(handle);

					MwDrawRect(handle, &r3, c);
				}

				p.x = r2.x + r2.width / 2;
				p.y = r2.y + r2.height / 2;
				MwDrawText(handle, NULL, &p, t->names[i], MwALIGNMENT_CENTER, ct);
			}
		}
	}

	MwLLFreeColor(ct);
	MwLLFreeColor(c);
}

static void show_frame(MwWidget handle) {
	MwTab t = handle->internal;
	int   n = MwGetInteger(handle, MwNvalue);
	int   i;

	for(i = 0; i < arrlen(t->frames); i++) {
		if(i != n) MwLLShow(t->frames[i]->lowlevel, 0);
	}

	resize(handle);

	MwLLShow(t->frames[n]->lowlevel, 1);

	MwForceRender(handle);

	MwDispatchUserHandler(handle, MwNchangedHandler, NULL);
}

static void click(MwWidget handle) {
	MwTab t = handle->internal;
	int   i;
	int   x = 0;
	int   h = tab_height(handle);

	for(i = 0; i < arrlen(t->names); i++) {
		int w = tab_width(handle, t->names[i]);

		if(MwGetInteger(handle, MwNvalue) != i && x <= handle->mouse_point.x && handle->mouse_point.x <= (x + w) && 0 <= handle->mouse_point.y && handle->mouse_point.y <= h) {
			MwSetInteger(handle, MwNvalue, i);
			show_frame(handle);
		}

		x += w;
	}
}

static MwWidget mwTabAddImpl(MwWidget handle, const char* name) {
	MwTab	 t = handle->internal;
	MwWidget f = MwCreateWidget(MwFrameClass, "tabframe", handle, 0, MwGetInteger(handle, MwNheight), 0, 0);
	char*	 n = MwStringDuplicate(name);

	arrput(t->frames, f);
	arrput(t->names, n);

	MwLLShow(f->lowlevel, 0);

	MwForceRender(handle);

	if(MwGetInteger(handle, MwNvalue) == -1) {
		MwSetInteger(handle, MwNvalue, arrlen(t->frames) - 1);
		show_frame(handle);
	}

	return f;
}

static MwWidget mwTabGetFrameImpl(MwWidget handle, const char* name) {
	MwTab t = handle->internal;
	int   i;

	for(i = 0; i < arrlen(t->names); i++) {
		if(strcmp(t->names[i], name) == 0) return t->frames[i];
	}

	return NULL;
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwTabAdd") == 0) {
		const char* name = va_arg(va, const char*);

		*(MwWidget*)out = mwTabAddImpl(handle, name);
	} else if(strcmp(name, "mwTabGetFrame") == 0) {
		const char* name = va_arg(va, const char*);

		*(MwWidget*)out = mwTabGetFrameImpl(handle, name);
	}
}

static void resize(MwWidget handle) {
	MwTab t = handle->internal;
	int   v = MwGetInteger(handle, MwNvalue);

	if(v == -1) return;

	MwVaApply(t->frames[v],
		  MwNx, MwDefaultBorderWidth(handle),
		  MwNy, tab_height(handle) + MwDefaultBorderWidth(handle),
		  MwNwidth, MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) * 2,
		  MwNheight, MwGetInteger(handle, MwNheight) - tab_height(handle) - MwDefaultBorderWidth(handle) * 2,
		  NULL);
}

MwClassRec MwTabClassRec = {
    wcreate,	  /* create */
    destroy,	  /* destroy */
    draw,	  /* draw */
    click,	  /* click */
    NULL,	  /* parent_resize */
    NULL,	  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* execute */
    NULL,	  /* tick */
    resize,	  /* resize */
    NULL,	  /* children_update */
    NULL,	  /* children_prop_change */
    NULL,	  /* clipboard */
    NULL,	  /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwTabClass = &MwTabClassRec;
