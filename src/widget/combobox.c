/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int create(MwWidget handle) {
	MwComboBox cb = malloc(sizeof(*cb));

	cb->list = NULL;
	cb->opened	 = 0;
	cb->selected = 0;
	handle->internal = cb;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	MwComboBox cb = handle->internal;
	int i;

	for(i = 0; i < arrlen(cb->list); i++){
		free(cb->list[i]);
	}
	arrfree(cb->list);
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	   r, rc;
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor  text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwComboBox cb	= handle->internal;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawWidgetBack(handle, &r, base, 0, 1);

	rc = r;

	/* draw text */
	if(arrlen(cb->list) > cb->selected){
		MwPoint p;

		p.x = MwDefaultBorderWidth(handle) * 2;
		p.y = MwGetInteger(handle, MwNheight) / 2;

		MwDrawText(handle, &p, cb->list[0], 0, MwALIGNMENT_BEGINNING, text);
	}

	r = rc;
	r.height /= 5;
	r.width	 = r.height * 3;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - MwDefaultBorderWidth(handle) * 2;
	r.y	 = MwDefaultBorderWidth(handle);
	r.height = rc.height;
	r.width += MwDefaultBorderWidth(handle) * 2;
	MwDrawWidgetBack(handle, &r, base, 0, 0);

	r	 = rc;
	r.width	 = r.height * 3 / 5;
	r.height = r.width - MwDefaultBorderWidth(handle) * 2;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - MwDefaultBorderWidth(handle) * 2;
	r.y	 = (MwGetInteger(handle, MwNheight) - (r.height + MwDefaultBorderWidth(handle) * 2)) / 2;
	MwDrawTriangle(handle, &r, base, cb->opened, MwSOUTH);

	r.y += r.height;
	r.height = MwDefaultBorderWidth(handle) * 2;
	MwDrawFrame(handle, &r, base, 0);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwComboBox cb = handle->internal;

	cb->opened = cb->opened ? 0 : 1;

	MwForceRender(handle);
}

static void mwComboBoxAddImpl(MwWidget handle, int index, const char* text){
	MwComboBox cb = handle->internal;
	char* t = MwStringDupliacte(text);

	if(index == -1) index = arrlen(cb->list);

	arrins(cb->list, index, t);

	if(index <= cb->selected) MwForceRender(handle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	(void)out;
	if(strcmp(name, "mwComboBoxAdd") == 0) {
		int index = va_arg(va, int);
		const char* text = va_arg(va, const char*);
		mwComboBoxAddImpl(handle, index, text);
	}
}

MwClassRec MwComboBoxClassRec = {
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
    NULL,
    NULL,
    NULL};
MwClass MwComboBoxClass = &MwComboBoxClassRec;
