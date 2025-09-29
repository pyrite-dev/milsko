/* $Id$ */
#include <Mw/Milsko.h>

static void create(MwWidget handle) {
	MwSetDefault(handle);
}

static void draw(MwWidget handle) {
	MwRect r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, MwParseColor(handle, MwGetText(handle, MwNbackground)), handle->pressed);

//	MwDrawRect(handle, &r, MwParseColor(handle, MwGetText(handle, MwNbackground)));
}

static void click(MwWidget handle) {
	MwDispatchUserHandler(handle, MwNactivateHandler, NULL);
}

MwClassRec MwButtonClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    click   /* click */
};
MwClass MwButtonClass = &MwButtonClassRec;
