/* $Id$ */
#include <Mw/Milsko.h>

static void create(MwWidget handle) {
	MwSetDefault(handle);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrameEx(handle, &r, base, 1, 1);
	MwDrawFrameEx(handle, &r, base, 0, 1);
	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

MwClassRec MwFrameClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL,   /* click */
    NULL    /* parent_resize */
};
MwClass MwFrameClass = &MwFrameClassRec;
