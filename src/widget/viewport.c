/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

MwWidget MwViewportGetViewport(MwWidget widget) {
}

MwClassRec MwViewportClassRec = {
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
MwClass MwViewportClass = &MwViewportClassRec;
