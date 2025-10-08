/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	MwLLSetCursor(handle->lowlevel, &MwCursorText, &MwCursorTextMask);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0);
	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

static void key(MwWidget handle, int code) {
	printf("%c\n", code);
}

MwClassRec MwTextClassRec = {
    create,	   /* create */
    NULL,	   /* destroy */
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
MwClass MwTextClass = &MwTextClassRec;
