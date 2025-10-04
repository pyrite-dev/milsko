/* $Id$ */
#include <Mw/Milsko.h>

static void create(MwWidget handle) {
	MwSetDefault(handle);
}

static void draw(MwWidget handle) {
	MwRect	   r;
	MwLLPixmap px = MwGetVoid(handle, MwNpixmap);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	if(px != NULL) {
		MwLLDrawPixmap(handle->lowlevel, &r, px);
	}
}

MwClassRec MwImageClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL,   /* click */
    NULL    /* parent_resize */
};
MwClass MwImageClass = &MwImageClassRec;
