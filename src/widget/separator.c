/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNorientation, MwHORIZONTAL);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	if(MwGetInteger(handle, MwNorientation) == MwVERTICAL) {
		r.x	= (r.width - MwDefaultBorderWidth(handle) * 2) / 2;
		r.width = MwDefaultBorderWidth(handle) * 2;
	} else if(MwGetInteger(handle, MwNorientation) == MwHORIZONTAL) {
		r.y	 = (r.height - MwDefaultBorderWidth(handle) * 2) / 2;
		r.height = MwDefaultBorderWidth(handle) * 2;
	}
	MwDrawWidgetBack(handle, &r, base, 1, 1);

	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNorientation) == 0) MwForceRender(handle);
}

MwClassRec MwSeparatorClassRec = {
    create,	 /* create */
    NULL,	 /* destroy */
    draw,	 /* draw */
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
    NULL,	 /* key */
    NULL,	 /* execute */
    NULL,	 /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwSeparatorClass = &MwSeparatorClassRec;
