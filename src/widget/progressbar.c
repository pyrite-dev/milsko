#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);
	MwVaApply(handle,
		  MwNminValue, 0,
		  MwNmaxValue, 100,
		  MwNvalue, 0,
		  NULL);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor fill = MwParseColor(handle, MwGetText(handle, MwNforeground));
	double	  w;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, 1);
	MwDrawRect(handle, &r, base);
	w = MwGetInteger(handle, MwNvalue) - MwGetInteger(handle, MwNminValue);
	w = w / (MwGetInteger(handle, MwNmaxValue) - MwGetInteger(handle, MwNminValue));

	r.x += MwDefaultBorderWidth(handle);
	r.y += MwDefaultBorderWidth(handle);
	r.width -= MwDefaultBorderWidth(handle) * 2;
	r.height -= MwDefaultBorderWidth(handle) * 2;

	r.width = r.width * w;
	MwDrawRect(handle, &r, fill);

	MwLLFreeColor(fill);
	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNminValue) == 0 || strcmp(key, MwNmaxValue) == 0 || strcmp(key, MwNvalue) == 0) MwForceRender(handle);
}

MwClassRec MwProgressBarClassRec = {
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
MwClass MwProgressBarClass = &MwProgressBarClassRec;
