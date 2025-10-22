/* $Id$ */

#include "Mw/LowLevel.h"
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	    r;
	MwPoint	    point;
	MwLLColor   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	const char* str	 = MwGetText(handle, MwNtext);
	MwLLPixmap  px	 = MwGetVoid(handle, MwNpixmap);

	if(str == NULL) str = "";

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawWidgetBack(handle, &r, base, handle->pressed, MwTRUE);

	if(px != NULL) {
		int ow = r.width;
		int oh = r.height;

		double sw = (double)ow / px->width;
		double sh = (double)oh / px->height;

		if(sw < sh) {
			r.width	 = px->width * sw;
			r.height = px->height * sw;
		} else {
			r.width	 = px->width * sh;
			r.height = px->height * sh;
		}
		r.width -= MwDefaultBorderWidth * 2;
		r.height -= MwDefaultBorderWidth * 2;

		r.x += (double)(ow - r.width) / 2;
		r.y += (double)(oh - r.height) / 2;

		MwLLDrawPixmap(handle->lowlevel, &r, px);
	} else {
		point.x = r.x + r.width / 2;
		point.y = r.x + r.height / 2;

		MwDrawText(handle, &point, str, 0, MwALIGNMENT_CENTER, text);
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwDispatchUserHandler(handle, MwNactivateHandler, NULL);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtext) == 0 || strcmp(key, MwNpixmap) == 0) MwForceRender(handle);
}

MwClassRec MwButtonClassRec = {
    create,	    /* create */
    NULL,	    /* destroy */
    draw,	    /* draw */
    click,	    /* click */
    NULL,	    /* parent_resize */
    prop_change,    /* prop_change */
    NULL,	    /* mouse_move */
    MwForceRender2, /* mouse_up */
    MwForceRender2, /* mouse_down */
    NULL,	    /* key */
    NULL,	    /* custom */
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwButtonClass = &MwButtonClassRec;
