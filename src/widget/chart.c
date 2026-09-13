#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static const char* palette0[] = {
    "#800",
    "#080",
    "#880",
    "#008",
    "#808",
    "#088",
    "#888",
    "#f00",
    "#0f0",
    "#ff0",
    "#00f",
    "#f0f",
    "#0ff",
    "#fff",
    NULL};

static int wcreate(MwWidget handle) {
	MwChart c = malloc(sizeof(*c));
	memset(c, 0, sizeof(*c));

	handle->internal = c;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	MwChartReset(handle);
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwLLColor    base   = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor    border = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	     r;
	MwChart	     c = handle->internal;
	int	     i;
	const char** colors = palette0;
	int	     n;
	int	     gap;
	int	     width;
	MwRect	     node;
	double	     min   = 0xffffffff;
	double	     max   = -0xffffffff;
	int	     space = MwTextHeight(handle, NULL, "M");
	MwPoint	     p[2];

	for(n = 0; colors[n] != NULL; n++);

	for(i = 0; i < arrlen(c->entries); i++) {
		if(min > c->entries[i].value) min = c->entries[i].value;
		if(max < c->entries[i].value) max = c->entries[i].value;
	}

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	if(min > 0) {
		min = 0;
	} else if(min < 0) {
		min -= r.height / 10;
	}

	node.x = 16;
	node.y = 0;

	width = (r.width - node.x) * 2 / (arrlen(c->entries) * 3 + 1);
	gap   = width / 2;

	p[0].x = node.x;
	p[0].y = r.height - space;

	p[1].x = node.x + (width + gap) * arrlen(c->entries) + gap;
	p[1].y = p[0].y;
	MwLLLine(handle->lowlevel, &p[0], border);

	p[0].y = r.height - space - abs(min / (max - min) * (r.height - space));
	p[1].y = p[0].y;
	MwLLLine(handle->lowlevel, &p[0], border);
	MwDrawText(handle, NULL, p, "0", MwALIGNMENT_END, border);

	p[0].x = node.x;
	p[0].y = 0;

	p[1].x = node.x;
	p[1].y = r.height - space;
	MwLLLine(handle->lowlevel, &p[0], border);

	node.width = width;
	for(i = 0; i < arrlen(c->entries); i++) {
		MwLLColor color = MwParseColor(handle, c->entries[i].color == NULL ? colors[i % n] : c->entries[i].color);

		node.x += gap;

		node.height = c->entries[i].value / (max - min) * (r.height - space);
		node.y	    = r.height - space - node.height - abs(min / (max - min) * (r.height - space));

		MwDrawRect(handle, &node, color);
		MwDrawRectLine(handle, &node, border);

		p[0].x = node.x + node.width / 2;
		p[0].y = r.height - space / 2;

		MwDrawText(handle, NULL, p, c->entries[i].name, MwALIGNMENT_CENTER, border);

		node.x += node.width;

		MwLLFreeColor(color);
	}

	MwLLFreeColor(border);
	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtype) == 0) MwForceRender(handle);
}

static int mwChartAddImpl(MwWidget handle, int index, const char* text, double value, const char* color) {
	MwChart	     c = handle->internal;
	MwChartEntry e;

	if(index == -1) index = arrlen(c->entries);

	e.name	= MwStringDuplicate(text);
	e.color = color == NULL ? NULL : MwStringDuplicate(color);
	e.value = value;

	arrins(c->entries, index, e);

	MwForceRender(handle);

	return index;
}

static void mwChartDeleteImpl(MwWidget handle, int index) {
	MwChart c = handle->internal;

	if(index == -1) index = arrlen(c->entries) - 1;

	free(c->entries[index].name);
	if(c->entries[index].color != NULL) free(c->entries[index].color);
	arrdel(c->entries, index);

	MwForceRender(handle);
}

static void mwChartResetImpl(MwWidget handle) {
	MwChart c = handle->internal;
	int	i;

	for(i = 0; i < arrlen(c->entries); i++) {
		free(c->entries[i].name);
		if(c->entries[i].color != NULL) free(c->entries[i].color);
	}
	arrfree(c->entries);

	MwForceRender(handle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwChartAdd") == 0) {
		int	    index = va_arg(va, int);
		const char* text  = va_arg(va, const char*);
		double	    value = va_arg(va, double);
		const char* color = va_arg(va, const char*);
		*(int*)out	  = mwChartAddImpl(handle, index, text, value, color);
	}
	if(strcmp(name, "mwChartDelete") == 0) {
		int index = va_arg(va, int);
		mwChartDeleteImpl(handle, index);
	}
	if(strcmp(name, "mwChartReset") == 0) {
		mwChartResetImpl(handle);
	}
}

MwClassRec MwChartClassRec = {
    wcreate,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    prop_change,   /* prop_change */
    NULL,	   /* mouse_move */
    NULL,	   /* mouse_up */
    NULL,	   /* mouse_down */
    NULL,	   /* key */
    func_handler,  /* execute */
    NULL,	   /* tick */
    MwForceRender, /* resize */
    NULL,	   /* children_update */
    NULL,	   /* children_prop_change */
    NULL,	   /* clipboard */
    NULL,	   /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwChartClass = &MwChartClassRec;
