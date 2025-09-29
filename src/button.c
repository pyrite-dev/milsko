/* $Id$ */
#include <Mw/Milsko.h>

static void create(MwWidget handle) {
	MwSetDefault(handle);
}

static void draw(MwWidget handle) {
	MwRect	    r;
	MwPoint	    point;
	MwLLColor   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	const char* str	 = MwGetText(handle, MwNtext);

	if(str == NULL) str = "";

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, handle->pressed);
	MwDrawRect(handle, &r, base);

	point.x = r.x + r.width / 2;
	point.y = r.x + r.height / 2;

	MwDrawText(handle, &point, str, text);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwDispatchUserHandler(handle, MwNactivateHandler, NULL);
}

MwClassRec MwButtonClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    click   /* click */
};
MwClass MwButtonClass = &MwButtonClassRec;
