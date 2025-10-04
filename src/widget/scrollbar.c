/* $Id $*/
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r, rt;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor dark = MwLightenColor(handle, base, -64, -64, -64);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, dark, 1);
	MwDrawRect(handle, &r, dark);

	rt	  = r;
	rt.height = rt.width;
	MwDrawTriangle(handle, &rt, base, 0, MwNORTH);
	rt.y = r.y + r.height - rt.height;
	MwDrawTriangle(handle, &rt, base, 0, MwSOUTH);

	MwLLFreeColor(dark);
	MwLLFreeColor(base);
}

MwClassRec MwScrollBarClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL,   /* click */
    NULL    /* parent_resize */
};
MwClass MwScrollBarClass = &MwScrollBarClassRec;
