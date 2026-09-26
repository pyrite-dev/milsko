#include <Mw/Milsko.h>

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNhour, 0);
	MwSetInteger(handle, MwNminute, 0);
	MwSetInteger(handle, MwNsecond, 0);
	return 0;
}

static void hand(MwWidget handle, double x, double y, double width, double length, double angle, MwColor inside, MwColor border) {
	double	w = MwGetInteger(handle, MwNwidth);
	double	h = MwGetInteger(handle, MwNheight);
	MwPoint p[5];
	int	i;
	double	rad = (angle - 90) / 180 * M_PI;
	double	c   = cos(rad);
	double	s   = sin(rad);
	double	c2  = cos(rad - 0.5 * M_PI);
	double	s2  = sin(rad - 0.5 * M_PI);
	double	c3  = cos(rad - M_PI);
	double	s3  = sin(rad - M_PI);

	if(width < width / 10) width = width / 10;

	p[0].x = c2 * width / 2;
	p[0].y = s2 * width / 2;

	p[1].x = c3 * width / 2 * 2;
	p[1].y = s3 * width / 2 * 2;

	p[2].x = -c2 * width / 2;
	p[2].y = -s2 * width / 2;

	p[3].x = c * length;
	p[3].y = s * length;

	for(i = 0; i < 4; i++) {
		p[i].x += w / 2 + x;
		p[i].y += h / 2 + y;
	}

	p[4] = p[0];

	MwLLPolygon(handle->lowlevel, p, 4, inside->lowlevel);

	MwLLLine(handle->lowlevel, &p[0], border->lowlevel);
	MwLLLine(handle->lowlevel, &p[1], border->lowlevel);
	MwLLLine(handle->lowlevel, &p[2], border->lowlevel);
	MwLLLine(handle->lowlevel, &p[3], border->lowlevel);
}

static void draw(MwWidget handle) {
	int	ColorDiff = MwGetColorDifference(handle);
	int	ShadowDist;
	int	BaseWidth;
	MwColor base   = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwColor shadow = MwLightenColor(handle, base, -ColorDiff, -ColorDiff, -ColorDiff);
	MwColor inside = MwParseColor(handle, MwGetText(handle, MwNsubBackground));
	MwColor border = MwParseColor(handle, MwGetText(handle, MwNsubForeground));
	MwRect	r;
	int	i;
	double	w;
	double	fs = (MwGetInteger(handle, MwNsecond) % 60);
	double	fm = (MwGetInteger(handle, MwNminute) % 60) + (MwGetInteger(handle, MwNsecond) % 60) / 60.0;
	double	s  = (double)(MwGetInteger(handle, MwNsecond) % 60) / 60 * 360;
	double	m  = ((double)(MwGetInteger(handle, MwNminute) % 60) + fs / 60.0) / 60 * 360;
	double	h  = ((double)(MwGetInteger(handle, MwNhour) % 12) + fm / 60.0) / 12 * 360;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	w = r.width < r.height ? r.width : r.height;

	BaseWidth  = w / 10;
	ShadowDist = BaseWidth / 3;

	for(i = 0; i < 12; i++) {
		MwPoint p[2];
		double	rad = i * 30 / 180.0 * M_PI;
		double	c   = cos(rad);
		double	s   = sin(rad);
		double	x, y;
		double	l = w / 2 / 5;

		x = r.width / 2 + c * w / 2 - l * c;
		y = r.height / 2 + s * w / 2 - l * s;

		p[0].x = x + l * c / (i % 3 ? 2 : 1);
		p[0].y = y + l * s / (i % 3 ? 2 : 1);

		p[1].x = x;
		p[1].y = y;

		MwLLLine(handle->lowlevel, p, border->lowlevel);
	}

	hand(handle, ShadowDist, ShadowDist, BaseWidth, w / 2 * 2 / 3, h, shadow, shadow);
	hand(handle, ShadowDist, ShadowDist, BaseWidth, w / 2, m, shadow, shadow);
	hand(handle, ShadowDist, ShadowDist, BaseWidth / 4, w / 2, s, shadow, shadow);

	hand(handle, 0, 0, w / 10, w / 2 * 2 / 3, h, inside, border);
	hand(handle, 0, 0, BaseWidth, w / 2, m, inside, border);
	hand(handle, 0, 0, BaseWidth / 4, w / 2, s, inside, border);

	MwFreeColor(border);
	MwFreeColor(inside);
	MwFreeColor(shadow);
	MwFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNhour) == 0 || strcmp(key, MwNminute) == 0 || strcmp(key, MwNsecond) == 0) MwForceRender(handle);
}

MwClassRec MwClockClassRec = {
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
MwClass MwClockClass = &MwClockClassRec;
