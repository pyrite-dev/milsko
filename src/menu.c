/* $Id$ */
#include <Mw/Milsko.h>

static void set_xywh(MwWidget handle) {
	int height = 0;

	height += 20;

	MwVaApply(handle,
		  MwNx, 0,
		  MwNy, 0,
		  MwNwidth, MwGetInteger(handle->parent, MwNwidth),
		  MwNheight, height,
		  NULL);
}

static void create(MwWidget handle) {
	MwSetDefault(handle);

	set_xywh(handle);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, 0);
	MwDrawRect(handle, &r, base);
}

MwClassRec MwMenuClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MwClass MwMenuClass = &MwMenuClassRec;
