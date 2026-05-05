#include <Mw/Milsko.h>

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNmonth, 0);
	MwSetInteger(handle, MwNdate, 1);
	MwSetInteger(handle, MwNday, 3);
	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor c   = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor csb = MwParseColor(handle, MwGetText(handle, MwNsubBackground));
	MwLLColor csf = MwParseColor(handle, MwGetText(handle, MwNsubForeground));
	MwRect	  r, r2;
	MwPoint	  p[4];
	int	  tw = 2;
	int	  th = 6;
	char	  buf[3];

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, c);

	r2.width  = 32;
	r2.height = 32 - th - 1;
	r2.x	  = (r.width - r2.width) / 2;
	r2.y	  = (r.height - r2.height - th - 1) / 2;
	MwDrawRect(handle, &r2, csb);

	p[0].x = r2.x;
	p[0].y = r2.y + r2.height;
	p[1].x = r2.x - tw;
	p[1].y = r2.y + r2.height + th;
	p[2].x = r2.x + r2.width - tw;
	p[2].y = r2.y + r2.height + th;
	p[3].x = r2.x + r2.width;
	p[3].y = r2.y + r2.height;
	MwLLPolygon(handle->lowlevel, p, 4, csb);

	p[0].x = r.width / 2;
	p[0].y = r.height / 2;
	sprintf(buf, "%d", MwGetInteger(handle, MwNdate));
	MwDrawText(handle, MwFLBuildFont(MwFLFlagBold), p, buf, MwALIGNMENT_CENTER, csf);

	MwLLFreeColor(csf);
	MwLLFreeColor(csb);
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
