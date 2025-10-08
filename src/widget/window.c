/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor c = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwRect	  r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, c);

	MwLLFreeColor(c);
}

MwClassRec MwWindowClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL,   /* click */
    NULL,   /* parent_resize */
    NULL,   /* prop_change */
    NULL,   /* mouse_move */
    NULL,   /* mouse_up */
    NULL,   /* mouse_down */
    NULL,   /* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwWindowClass = &MwWindowClassRec;

void MwWindowSetIcon(MwWidget handle, MwLLPixmap pixmap) {
	MwLLSetIcon(handle->lowlevel, pixmap);
}
