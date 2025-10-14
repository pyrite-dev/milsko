/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	int st;

	if((st = MwViewportClass->create(handle)) != 0) return st;

	return 0;
}

static void destroy(MwWidget handle) {
	MwViewportClass->destroy(handle);
}

static void draw(MwWidget handle) {
	MwViewportClass->draw(handle);
}

static void prop_change(MwWidget handle, const char* prop) {
	MwViewportClass->prop_change(handle, prop);
}

MwClassRec MwListBoxClassRec = {
    create,	 /* create */
    destroy,	 /* destroy */
    draw,	 /* draw */
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
    NULL,	 /* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwListBoxClass = &MwListBoxClassRec;
