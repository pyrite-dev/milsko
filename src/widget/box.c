#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int create(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNorientation, MwHORIZONTAL);
	MwSetInteger(handle, MwNmargin, 0);
	MwSetInteger(handle, MwNpadding, 0);
	MwSetInteger(handle, MwNhasBorder, 0);
	MwSetInteger(handle, MwNinverted, 1);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	if(MwGetInteger(handle, MwNhasBorder)) {
		MwDrawFrame(handle, &r, base, MwGetInteger(handle, MwNinverted));
	}

	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

#define Margin ((i != (arrlen(handle->children) - 1)) ? MwGetInteger(handle, MwNmargin) : 0)

static void layout(MwWidget handle) {
	int i;
	int sum	  = 0;
	int horiz = MwGetInteger(handle, MwNorientation) == MwHORIZONTAL ? 1 : 0;
	int sz	  = MwGetInteger(handle, horiz ? MwNwidth : MwNheight) - (MwGetInteger(handle, MwNpadding) + (MwGetInteger(handle, MwNhasBorder) ? MwDefaultBorderWidth(handle) : 0)) * 2;
	int fsz	  = MwGetInteger(handle, horiz ? MwNheight : MwNwidth) - (MwGetInteger(handle, MwNpadding) + (MwGetInteger(handle, MwNhasBorder) ? MwDefaultBorderWidth(handle) : 0)) * 2;
	int sk	  = MwGetInteger(handle, MwNpadding) + (MwGetInteger(handle, MwNhasBorder) ? MwDefaultBorderWidth(handle) : 0);

	for(i = 0; i < arrlen(handle->children); i++) {
		int n = MwGetInteger(handle->children[i], MwNratio);
		int s = MwGetInteger(handle->children[i], MwNfixedSize);
		if(n == MwDEFAULT) n = 1;

		if(handle->children[i]->destroyed) continue;

		if(s != MwDEFAULT) {
			sz -= s + Margin;
		} else {
			sum += n;
		}
	}

	for(i = 0; i < arrlen(handle->children); i++) {
		int n = MwGetInteger(handle->children[i], MwNratio);
		int s = MwGetInteger(handle->children[i], MwNfixedSize);
		int wsz;
		if(n == MwDEFAULT) n = 1;

		if(handle->children[i]->destroyed) continue;

		if(s != MwDEFAULT) {
			wsz = s;
		} else {
			wsz = sz * n / sum;
		}

		MwVaApply(handle->children[i],
			  horiz ? MwNx : MwNy, sk,													   /* this is what gets changed */
			  horiz ? MwNy : MwNx, MwGetInteger(handle, MwNpadding) + (MwGetInteger(handle, MwNhasBorder) ? MwDefaultBorderWidth(handle) : 0), /* fixed between widgets */
			  horiz ? MwNwidth : MwNheight, wsz,												   /* this is what gets changed */
			  horiz ? MwNheight : MwNwidth, fsz,												   /* fixed between widgets */
			  NULL);
		sk += wsz + Margin;
	}
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNorientation) == 0) layout(handle);
}

static void children_prop_change(MwWidget handle, MwWidget child, const char* key) {
	(void)child;

	if(strcmp(key, MwNratio) == 0 || strcmp(key, MwNfixedSize) == 0) layout(handle);
}

static void resize(MwWidget handle) {
	layout(handle);
}

static void children_update(MwWidget handle) {
	layout(handle);
}

MwClassRec MwBoxClassRec = {
    create,		  /* create */
    NULL,		  /* destroy */
    draw,		  /* draw */
    NULL,		  /* click */
    NULL,		  /* parent_resize */
    prop_change,	  /* prop_change */
    NULL,		  /* mouse_move */
    NULL,		  /* mouse_up */
    NULL,		  /* mouse_down */
    NULL,		  /* key */
    NULL,		  /* execute */
    NULL,		  /* tick */
    resize,		  /* resize */
    children_update,	  /* children_update */
    children_prop_change, /* children_prop_change */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwBoxClass = &MwBoxClassRec;
