/* $Id: button.c 168 2025-10-04 19:30:58Z nishi $ */
#include <Mw/Milsko.h>

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

	MwDrawWidgetBack(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0, MwTRUE);
	if(handle->pressed || MwGetInteger(handle, MwNchecked)) {
		/* TODO: write check mark */
	}

	MwLLFreeColor(base);
}

static void click(MwWidget handle) {
	MwSetInteger(handle, MwNchecked, MwGetInteger(handle, MwNchecked) ? 0 : 1);

	MwDispatchUserHandler(handle, MwNchangedHandler, NULL);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNchecked) == 0) MwForceRender(handle);
}

MwClassRec MwCheckBoxClassRec = {
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
MwClass MwCheckBoxClass = &MwCheckBoxClassRec;
