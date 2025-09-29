/* $Id$ */
#include <Mw/Mw.h>

static void create(MwWidget handle) {
	MwSetDefault(handle);
}

static void draw(MwWidget handle) {
	MwLLColor c = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwRect    r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, c);

	MwLLFreeColor(c);
}

MwClassRec MwWindowClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MwClass MwWindowClass = &MwWindowClassRec;
