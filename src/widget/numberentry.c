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
	MwEntry	    e	= handle->internal;
	const char* str = MwGetText(handle, MwNtext);
	int	    ok	= 0;
	if(str == NULL) str = "";

	if(code == '-' && e->cursor == 0 && strchr(str, (int)'-') == NULL) {
		ok = 1;
	} else if('0' <= code && code <= '9') {
		ok = 1;
	} else if(code == '.' && strchr(str, (int)'.') == NULL) {
		ok = 1;
	}

	if(ok) MwEntryClass->key(handle, code);
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
