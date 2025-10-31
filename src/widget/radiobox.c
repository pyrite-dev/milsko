/* $Id: button.c 168 2025-10-04 19:30:58Z nishi $ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int create(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNchecked, 0);

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
	MwDrawDiamond(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0);

	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwSetInteger(handle, MwNchecked, MwGetInteger(handle, MwNchecked) ? 0 : 1);
	if(MwGetInteger(handle, MwNchecked) && handle->parent != NULL) {
		int i;
		for(i = 0; i < arrlen(handle->parent->children); i++) {
			MwWidget w = handle->parent->children[i];
			if(w != handle && w->widget_class == MwRadioBoxClass && MwGetInteger(w, MwNchecked)) {
				MwSetInteger(w, MwNchecked, 0);
			}
		}
	}

	MwDispatchUserHandler(handle, MwNchangedHandler, NULL);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNchecked) == 0) MwForceRender(handle);
}

MwClassRec MwRadioBoxClassRec = {
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
    NULL,	    /* execute */
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwRadioBoxClass = &MwRadioBoxClassRec;
