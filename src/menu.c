/* $Id$ */
#include <Mw/Milsko.h>

#include "stb_ds.h"

typedef struct menu menu_t;

struct menu {
	char*	 name;
	menu_t** sub;
};

static void set_xywh(MwWidget handle) {
	int	height = 0;
	int	i;
	menu_t* m = handle->internal;

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

static void create(MwWidget handle) {
	menu_t* m = malloc(sizeof(*m));

	m->name		 = NULL;
	m->sub		 = NULL;
	handle->internal = m;

	MwSetDefault(handle);

	set_xywh(handle);
}

static void recursive_free(menu_t* m) {
	int i;

	for(i = 0; i < arrlen(m->sub); i++) {
		recursive_free(m->sub[i]);
	}

	if(m->sub != NULL) arrfree(m->sub);
	if(m->name != NULL) free(m->name);
	free(m);
}

static void destroy(MwWidget handle) {
	menu_t* m = handle->internal;

	recursive_free(m);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwPoint	  p;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	menu_t*	  m    = handle->internal;
	int	  i;

	p.x = 10;
	p.y = MwGetInteger(handle, MwNheight) / 2;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, 0);
	MwDrawRect(handle, &r, base);
	for(i = 0; i < arrlen(m->sub); i++) {
		int tw = MwTextWidth(handle, m->sub[i]->name);

		p.x += tw / 2;

		MwDrawText(handle, &p, m->sub[i]->name, 1, text);

		p.x += tw / 2 + 20;
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

void* MwMenuAdd(MwWidget handle, void* menu, const char* name) {
	menu_t* m   = menu == NULL ? handle->internal : menu;
	menu_t* new = malloc(sizeof(*new));
	new->name   = malloc(strlen(name) + 1);
	new->sub    = NULL;

	strcpy(new->name, name);

	arrput(m->sub, new);

	set_xywh(handle);

	return new;
}
