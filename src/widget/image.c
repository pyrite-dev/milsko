/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	   r;
	MwLLPixmap px	= MwGetVoid(handle, MwNpixmap);
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);
	if(px != NULL) {
		MwLLDrawPixmap(handle->lowlevel, &r, px);
	}

	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNpixmap) == 0) MwForceRender(handle);
}

MwClassRec MwImageClassRec = {
    create,	 /* create */
    NULL,	 /* destroy */
    draw,	 /* draw */
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
    NULL,	 /* key */
    NULL,	 /* execute */
    NULL, /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwImageClass = &MwImageClassRec;
