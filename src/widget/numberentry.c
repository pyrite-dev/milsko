/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	int st;

	if((st = MwEntryClass->create(handle)) != 0) return st;

	return 0;
}

static void destroy(MwWidget handle) {
	MwEntryClass->destroy(handle);
}

static void draw(MwWidget handle) {
	MwEntryClass->draw(handle);
}

static void key(MwWidget handle, int code) {
	MwEntryClass->key(handle, code);
}

MwClassRec MwNumberEntryClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    NULL,	   /* prop_change */
    NULL,	   /* mouse_move */
    MwForceRender, /* mouse_up */
    MwForceRender, /* mouse_down */
    key,	   /* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwNumberEntryClass = &MwNumberEntryClassRec;
