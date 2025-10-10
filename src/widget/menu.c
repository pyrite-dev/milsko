/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

static void set_xywh(MwWidget handle) {
	int    height = 0;
	int    i;
	MwMenu m = handle->internal;

	for(i = 0; i < arrlen(m->sub); i++) {
		int h = MwTextHeight(handle, m->sub[i]->name);
		if(height < h) {
			height = h;
		}
	}

	height += 20;

	MwVaApply(handle,
		  MwNx, 0,
		  MwNy, 0,
		  MwNwidth, MwGetInteger(handle->parent, MwNwidth),
		  MwNheight, height,
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
\
	p.x = 10; \
	p.y = MwGetInteger(handle, MwNheight) / 2; \
\
	r.x	 = 0; \
	r.y	 = 0; \
	r.width	 = MwGetInteger(handle, MwNwidth); \
	r.height = MwGetInteger(handle, MwNheight);

#define BEGIN_MENU_LOOP \
	for(i = 0; i < arrlen(m->sub); i++) { \
		int incr = m->sub[i]->name[0] == '?' ? 1 : 0; \
		int tw	 = MwTextWidth(handle, m->sub[i]->name + incr); \
		int th	 = MwTextHeight(handle, m->sub[i]->name + incr); \
		int oldx = p.x; \
		int in_area; \
\
		if(incr) { \
			p.x = MwGetInteger(handle, MwNwidth) - tw - 10; \
		} \
\
		r.x	 = p.x - 5; \
		r.y	 = p.y - th / 2 - 5; \
		r.width	 = tw + 10; \
		r.height = th + 10; \
\
		in_area = (r.x <= handle->mouse_point.x && r.y <= handle->mouse_point.y && handle->mouse_point.x <= (int)(r.x + r.width) && handle->mouse_point.y <= (int)(r.y + r.height)) ? 1 : 0;

#define END_MENU_LOOP \
	p.x += tw / 2 + 20; \
	if(incr) p.x = oldx; \
	}

static void draw(MwWidget handle) {
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MENU_LOOP_DECL;

	MwDrawFrame(handle, &r, base, 0);
	MwDrawRect(handle, &r, base);

	BEGIN_MENU_LOOP;
	if(m->sub[i]->wsub != NULL) {
		MwDrawFrame(handle, &r, base, 0);
	} else if(in_area && handle->pressed) {
		MwDrawFrame(handle, &r, base, 0);
	}

	MwDrawText(handle, &p, m->sub[i]->name + incr, 1, MwALIGNMENT_BEGINNING, text);
	END_MENU_LOOP;

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void parent_resize(MwWidget handle) {
	set_xywh(handle);
}

static void mouse_down(MwWidget handle) {
	MENU_LOOP_DECL;
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

static void mouse_up(MwWidget handle) {
	MENU_LOOP_DECL;
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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwMenuClass = &MwMenuClassRec;

MwMenu MwMenuAdd(MwWidget handle, MwMenu menu, const char* name) {
	MwMenu m   = menu == NULL ? handle->internal : menu;
	MwMenu new = malloc(sizeof(*new));
	new->name  = malloc(strlen(name) + 1);
	new->sub   = NULL;
	new->wsub  = NULL;
	new->keep  = 0;

	strcpy(new->name, name);

	arrput(m->sub, new);

	set_xywh(handle);

	return new;
}
