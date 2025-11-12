/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwComboBox cb = malloc(sizeof(*cb));

	cb->opened	 = 0;
	handle->internal = cb;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	   r, rc;
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwComboBox cb	= handle->internal;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawWidgetBack(handle, &r, base, 0, 1);

	rc = r;

	/* draw text */

	r = rc;
	r.height /= 5;
	r.width	 = r.height * 3;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - (r.width / 3) * 2;
	r.y	 = MwDefaultBorderWidth(handle);
	r.height = rc.height;
	r.width += (r.width / 3) * 2;
	MwDrawWidgetBack(handle, &r, base, 0, 0);

	r	 = rc;
	r.width	 = r.height * 3 / 5;
	r.height = r.width - MwDefaultBorderWidth(handle) * 2;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - r.width / 3;
	r.y	 = (MwGetInteger(handle, MwNheight) - (r.height + MwDefaultBorderWidth(handle) * 2)) / 2;
	MwDrawTriangle(handle, &r, base, cb->opened, MwSOUTH);

	r.y += r.height;
	r.height = MwDefaultBorderWidth(handle) * 2;
	MwDrawFrame(handle, &r, base, 0);

	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwComboBox cb = handle->internal;

	cb->opened = cb->opened ? 0 : 1;

	MwForceRender(handle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwComboBoxAdd") == 0) {
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
