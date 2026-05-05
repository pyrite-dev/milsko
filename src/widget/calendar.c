#include <Mw/Milsko.h>

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNmonth, 0);
	MwSetInteger(handle, MwNdate, 1);
	MwSetInteger(handle, MwNday, 3);
	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor c    = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor cb   = MwParseColor(handle, "#fff");
	MwLLColor cb_g = MwParseColor(handle, "#666");
	MwLLColor ct   = MwParseColor(handle, "#000");
	MwRect	  r, r2, r3;
	MwPoint	  p[4];
	int	  h  = 64;
	int	  pw = h / 16; /* parallelogram width */
	int	  ph = pw * 3; /* parallelogram height */
	int	  gh = h / 32; /* gray area height */
	char	  buf[3];

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, c);

	r2.width  = h;
	r2.height = h - ph - gh;
	r2.x	  = (r.width - r2.width) / 2;
	r2.y	  = (r.height - r2.height - ph - gh) / 2;
	MwDrawRect(handle, &r2, cb);

	r3.x	  = r2.x;
	r3.y	  = r2.y + r2.height;
	r3.width  = r2.width;
	r3.height = ph + gh;
	MwDrawRect(handle, &r3, cb_g);

	p[0].x = r2.x;
	p[0].y = r2.y + r2.height;
	p[1].x = r2.x - pw;
	p[1].y = r2.y + r2.height + ph;
	p[2].x = r2.x + r2.width - pw;
	p[2].y = r2.y + r2.height + ph;
	p[3].x = r2.x + r2.width;
	p[3].y = r2.y + r2.height;
	MwLLPolygon(handle->lowlevel, p, 4, cb);

	p[0].x = r.width / 2;
	p[0].y = r.height / 2;
	sprintf(buf, "%d", MwGetInteger(handle, MwNdate));
	MwDrawText(handle, MwFLBuildFont(MwFLFlagBold), p, buf, MwALIGNMENT_CENTER, ct);

	MwLLFreeColor(ct);
	MwLLFreeColor(cb_g);
	MwLLFreeColor(cb);
	MwLLFreeColor(c);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNmonth) == 0 || strcmp(key, MwNdate) == 0 || strcmp(key, MwNday) == 0) MwForceRender(handle);
}

MwClassRec MwCalendarClassRec = {
    wcreate,	 /* create */
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
    NULL,	 /* resize */
    NULL,	 /* children_update */
    NULL,	 /* children_prop_change */
    NULL,	 /* clipboard */
    NULL,	 /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwCalendarClass = &MwCalendarClassRec;
