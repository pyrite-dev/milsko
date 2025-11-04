/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  fr;
	MwRect	  rr;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	int hasBorder = MwGetInteger(handle, MwnhasBorder);
	int inverted;

	if(hasBorder) {
		inverted  = MwGetInteger(handle, MwNinverted);
		fr.x	  = 0;
		fr.y	  = 0;
		fr.width  = MwGetInteger(handle, MwNwidth);
		fr.height = MwGetInteger(handle, MwNheight);
		MwDrawFrame(handle, &fr, base, inverted);

		rr.x	  = MwDefaultBorderWidth(handle);
		rr.y	  = MwDefaultBorderWidth(handle);
		rr.width  = MwGetInteger(handle, MwNwidth) - (MwDefaultBorderWidth(handle) * 2);
		rr.height = MwGetInteger(handle, MwNheight) - (MwDefaultBorderWidth(handle) * 2);
	} else {

		rr.x	  = 0;
		rr.y	  = 0;
		rr.width  = MwGetInteger(handle, MwNwidth);
		rr.height = MwGetInteger(handle, MwNheight);
	}

	MwDrawRect(handle, &rr, base);

	MwLLFreeColor(base);
}

MwClassRec MwFrameClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL,   /* click */
    NULL,   /* parent_resize */
    NULL,   /* prop_change */
    NULL,   /* mouse_move */
    NULL,   /* mouse_up */
    NULL,   /* mouse_down */
    NULL,   /* key */
    NULL,   /* execute */
    NULL,   /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwFrameClass = &MwFrameClassRec;
