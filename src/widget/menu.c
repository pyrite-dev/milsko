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
	m->wait		 = 0;
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

static void draw(MwWidget handle) {
	MwRect	  r;
	MwPoint	  p;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwMenu	  m    = handle->internal;
	int	  i;

	p.x = 10;
	p.y = MwGetInteger(handle, MwNheight) / 2;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, 0);
	MwDrawRect(handle, &r, base);
	if(!handle->pressed) m->wait = 0;
	for(i = 0; i < arrlen(m->sub); i++) {
		int incr = m->sub[i]->name[0] == '?' ? 1 : 0;
		int tw	 = MwTextWidth(handle, m->sub[i]->name + incr);
		int th	 = MwTextHeight(handle, m->sub[i]->name + incr);
		int oldx = p.x;
		int in_area;

		if(incr) {
			p.x = MwGetInteger(handle, MwNwidth) - tw - 10;
		}
		p.x += tw / 2;

		r.x	 = p.x - tw / 2 - 5;
		r.y	 = p.y - th / 2 - 5;
		r.width	 = tw + 10;
		r.height = th + 10;

		in_area = (r.x <= handle->mouse_point.x && r.y <= handle->mouse_point.y && handle->mouse_point.x <= (int)(r.x + r.width) && handle->mouse_point.y <= (int)(r.y + r.height)) ? 1 : 0;
		if(!m->wait && handle->pressed && in_area) {
			MwDrawFrame(handle, &r, base, 0);
			if(m->sub[i]->wsub == NULL && arrlen(m->sub[i]->sub) > 0) {
				MwPoint p2;

				p2.x = p.x - tw / 2 - 5;
				p2.y = p.y + th / 2 + 5;

				m->sub[i]->wsub = MwCreateWidget(MwSubMenuClass, "submenu", handle, 0, 0, 0, 0);
				MwSubMenuAppear(m->sub[i]->wsub, m->sub[i], &p2);
			} else if(m->sub[i]->wsub != NULL && m->sub[i]->keep) {
				MwDestroyWidget(m->sub[i]->wsub);
				m->sub[i]->wsub = NULL;
				m->sub[i]->keep = 0;
				m->wait		= 1;
			} else if(arrlen(m->sub[i]->sub) == 0) {
				MwDispatchUserHandler(handle, MwNmenuHandler, m->sub[i]);
			}
		} else if(!handle->pressed && m->sub[i]->wsub != NULL) {
			if(in_area) {
				MwDrawFrame(handle, &r, base, 0);
				m->sub[i]->keep = 1;
			} else {
				MwDestroyWidget(m->sub[i]->wsub);
				m->sub[i]->wsub = NULL;
				m->sub[i]->keep = 0;
			}
		} else if(handle->pressed && m->sub[i]->keep && m->sub[i]->wsub != NULL) {
			MwDestroyWidget(m->sub[i]->wsub);
			m->sub[i]->wsub = NULL;
			m->sub[i]->keep = 0;
		}

		MwDrawText(handle, &p, m->sub[i]->name + incr, 1, text);

		p.x += tw / 2 + 20;
		if(incr) p.x = oldx;
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void parent_resize(MwWidget handle) {
	set_xywh(handle);
}

MwClassRec MwMenuClassRec = {
    create,	  /* create */
    destroy,	  /* destroy */
    draw,	  /* draw */
    NULL,	  /* click */
    parent_resize /* parent_resize */
};
MwClass MwMenuClass = &MwMenuClassRec;

MwMenu MwMenuAdd(MwWidget handle, MwMenu menu, const char* name) {
	MwMenu m   = menu == NULL ? handle->internal : menu;
	MwMenu new = malloc(sizeof(*new));
	new->name  = malloc(strlen(name) + 1);
	new->sub   = NULL;
	new->wsub  = NULL;
	new->keep  = 0;
	new->wait  = 0;

	strcpy(new->name, name);

	arrput(m->sub, new);

	set_xywh(handle);

	return new;
}
