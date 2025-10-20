/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);
	MwSetInteger(handle, MwNalignment, MwALIGNMENT_CENTER);
	MwSetInteger(handle, MwNbold, 0);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	    r;
	MwPoint	    p;
	MwLLColor   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	int	    align;
	const char* str = MwGetText(handle, MwNtext);

	if(str == NULL) str = "";

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, base);

	align = MwGetInteger(handle, MwNalignment);
	if(align == MwALIGNMENT_CENTER) {
		p.x = r.width / 2;
	} else if(align == MwALIGNMENT_BEGINNING) {
		p.x = MwTextWidth(handle, str) / 2;
	} else if(align == MwALIGNMENT_END) {
		p.x = r.width - MwTextWidth(handle, str) / 2;
	}
	p.y = r.height / 2;
	MwDrawText(handle, &p, str, MwGetInteger(handle, MwNbold), MwALIGNMENT_CENTER, text);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtext) == 0 || strcmp(key, MwNalignment) == 0) MwForceRender(handle);
}

MwClassRec MwLabelClassRec = {
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
    NULL,	 /* custom */
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwLabelClass = &MwLabelClassRec;
