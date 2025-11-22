/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"
#include "Mw/TypeDefs.h"

static void set_xywh(MwWidget handle) {
	int height = 0;
	int diff   = MwDefaultBorderWidth(handle);
	diff	   = 0;

	height = MwTextHeight(handle, "M") + 10;

	MwVaApply(handle,
		  MwNx, -diff,
		  MwNy, -diff,
		  MwNwidth, MwGetInteger(handle->parent, MwNwidth) + diff * 2,
		  MwNheight, height + diff,
		  NULL);
}

static int create(MwWidget handle) {
	MwMenu m = malloc(sizeof(*m));

	m->name		 = NULL;
	m->wsub		 = NULL;
	m->sub		 = NULL;
	handle->internal = m;

	MwSetDefault(handle);

	set_xywh(handle);

	return 0;
}

static void recursive_free(MwMenu m) {
	int i;

	for(i = 0; i < arrlen(m->sub); i++) {
		recursive_free(m->sub[i]);
	}

	if(m->sub != NULL) arrfree(m->sub);
	if(m->name != NULL) free(m->name);
	free(m);
}

static void destroy(MwWidget handle) {
	MwMenu m = handle->internal;

	recursive_free(m);
}

#define MENU_LOOP_DECL \
	int	i; \
	MwMenu	m = handle->internal; \
	MwPoint p; \
	MwRect	r; \
	int	rx; \
\
	p.x = 5; \
	p.y = (MwGetInteger(handle, MwNheight) - MwDefaultBorderWidth(handle)) / 2 + MwDefaultBorderWidth(handle); \
\
	r.x	 = 0; \
	r.y	 = 0; \
	r.width	 = MwGetInteger(handle, MwNwidth); \
	r.height = MwGetInteger(handle, MwNheight); \
\
	rx = r.width - 5;

#define BEGIN_MENU_LOOP \
	for(i = 0; i < arrlen(m->sub); i++) { \
		int incr = m->sub[i]->name[0] == '?' ? 1 : 0; \
		int tw	 = MwTextWidth(handle, m->sub[i]->name + incr); \
		int th	 = MwTextHeight(handle, m->sub[i]->name + incr); \
		int in_area; \
\
		if(incr) { \
			p.x = rx -= tw; \
			rx -= 10; \
		} \
\
		r.x	 = p.x - 5; \
		r.y	 = p.y - th / 2 - 5; \
		r.width	 = tw + 10; \
		r.height = th + 10; \
\
		in_area = (r.x <= handle->mouse_point.x && r.y <= handle->mouse_point.y && handle->mouse_point.x <= (int)(r.x + r.width) && handle->mouse_point.y <= (int)(r.y + r.height)) ? 1 : 0;

#define END_MENU_LOOP \
	p.x += tw + 20; \
	}

static void draw(MwWidget handle) {
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MENU_LOOP_DECL;

	MwDrawWidgetBack(handle, &r, base, 0, MwTRUE);

	BEGIN_MENU_LOOP;
	if(m->sub[i]->wsub != NULL) {
		MwDrawFrame(handle, &r, base, MwFALSE);
		MwDrawWidgetBack(handle, &r, base, 0, MwFALSE);
	} else if(in_area && handle->pressed) {
		MwDrawFrame(handle, &r, base, MwFALSE);
		MwDrawWidgetBack(handle, &r, base, 0, MwFALSE);
	}

	MwDrawText(handle, &p, m->sub[i]->name + incr, m->sub[i]->wsub != NULL || (in_area && handle->pressed) ? 1 : 0, MwALIGNMENT_BEGINNING, text);
	END_MENU_LOOP;

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void parent_resize(MwWidget handle) {
	set_xywh(handle);
}

static void mouse_down(MwWidget handle, void* ptr) {
	MENU_LOOP_DECL;

	if(((MwLLMouse*)ptr)->button != MwLLMouseLeft) return;

	BEGIN_MENU_LOOP;
	if(in_area) {
		if(m->sub[i]->wsub == NULL && arrlen(m->sub[i]->sub) > 0) {
			MwPoint p2;

			p2.x = p.x - 5;
			p2.y = p.y + th / 2 + 5;

			m->sub[i]->wsub = MwCreateWidget(MwSubMenuClass, "submenu", handle, 0, 0, 0, 0);
			MwSubMenuAppear(m->sub[i]->wsub, m->sub[i], &p2);
		} else if(m->sub[i]->wsub != NULL && m->sub[i]->keep) {
			MwDestroyWidget(m->sub[i]->wsub);
			m->sub[i]->wsub = NULL;
			m->sub[i]->keep = 0;
		} else if(arrlen(m->sub[i]->sub) == 0) {
			MwDispatchUserHandler(handle, MwNmenuHandler, m->sub[i]);
		}
	} else if(m->sub[i]->keep && m->sub[i]->wsub != NULL) {
		MwDestroyWidget(m->sub[i]->wsub);
		m->sub[i]->wsub = NULL;
		m->sub[i]->keep = 0;
	}
	END_MENU_LOOP;

	MwForceRender(handle);
}

static void mouse_up(MwWidget handle, void* ptr) {
	MENU_LOOP_DECL;

	if(((MwLLMouse*)ptr)->button != MwLLMouseLeft) return;

	BEGIN_MENU_LOOP;
	if(in_area && m->sub[i]->wsub != NULL) {
		m->sub[i]->keep = 1;
	} else if(m->sub[i]->wsub != NULL) {
		MwDestroyWidget(m->sub[i]->wsub);
		m->sub[i]->wsub = NULL;
		m->sub[i]->keep = 0;
	}
	END_MENU_LOOP;

	MwForceRender(handle);
}

static MwMenu mwMenuAddImpl(MwWidget handle, MwMenu menu, const char* name) {
	MwMenu m   = menu == NULL ? handle->internal : menu;
	MwMenu new = malloc(sizeof(*new));
	new->name  = MwStringDuplicate(name);
	new->sub   = NULL;
	new->wsub  = NULL;
	new->keep  = 0;

	arrput(m->sub, new);

	set_xywh(handle);

	return new;
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwMenuAdd") == 0) {
		MwMenu	    menu = va_arg(va, MwMenu);
		const char* name = va_arg(va, const char*);
		*(MwMenu*)out	 = mwMenuAddImpl(handle, menu, name);
	}
}

MwClassRec MwMenuClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    parent_resize, /* parent_resize */
    NULL,	   /* prop_change */
    NULL,	   /* mouse_move */
    mouse_up,	   /* mouse_up */
    mouse_down,	   /* mouse_down */
    NULL,	   /* key */
    func_handler,  /* execute */
    NULL,	   /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwMenuClass = &MwMenuClassRec;
