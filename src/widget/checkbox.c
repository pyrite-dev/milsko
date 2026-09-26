#include <Mw/Milsko.h>

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNchecked, 0);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	 r;
	MwColor	 base  = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwColor	 base2 = MwParseColor(handle, MwGetText(handle, MwNsubBackground));
	MwColor	 text2 = MwParseColor(handle, MwGetText(handle, MwNsubForeground));
	MwPixmap bgpx  = MwGetPointer(handle, MwNbackgroundPixmap);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	if(r.width < r.height) {
		r.height = r.width;
	} else {
		r.width = r.height;
	}
	r.x = (MwGetInteger(handle, MwNwidth) - r.width) / 2;
	r.y = (MwGetInteger(handle, MwNheight) - r.height) / 2;

	MwDrawFrame(handle, &r, base, 1);
	MwDrawRect(handle, &r, base2);
	if(bgpx != NULL) MwDrawPixmap(handle, &r, bgpx);
	if(handle->pressed || MwGetInteger(handle, MwNchecked)) {
		MwPoint p[6];
		int	gap_w = r.width / 4 / 3;
		int	gap_h = r.height / 4 / 3;

		r.x += gap_w;
		r.y += gap_h;
		r.width -= gap_w * 2;
		r.height -= gap_h * 2;

		p[0].x = r.x;
		p[0].y = r.y + r.height * 2 / 3;

		p[1].x = r.x + r.width / 3;
		p[1].y = r.y + r.height;

		p[2].x = r.x + r.width;
		p[2].y = r.y + r.height / 3;

		p[3] = p[2];
		p[3].y -= r.height / 3;

		p[4] = p[1];
		p[4].y -= r.height / 3;

		p[5] = p[0];
		p[5].y -= r.height / 3;

		MwPolygon(handle, p, 6, text2);
	}

	MwFreeColor(text2);
	MwFreeColor(base2);
	MwFreeColor(base);
}

static void click(MwWidget handle) {
	MwSetInteger(handle, MwNchecked, MwGetInteger(handle, MwNchecked) ? 0 : 1);

	MwDispatchUserHandler(handle, MwNchangedHandler, NULL);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNchecked) == 0) MwForceRender(handle);
}

MwClassRec MwCheckBoxClassRec = {
    wcreate,	    /* create */
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
    NULL,	    /* tick */
    MwForceRender,  /* resize */
    NULL,	    /* children_update */
    NULL,	    /* children_prop_change */
    NULL,	    /* clipboard */
    NULL,	    /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwCheckBoxClass = &MwCheckBoxClassRec;
