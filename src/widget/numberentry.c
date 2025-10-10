/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	int	st;
	MwEntry e;

	if((st = MwEntryClass->create(handle)) != 0) return st;

	e	 = handle->internal;
	e->right = 32;

	MwSetText(handle, MwNtext, "0");

	return 0;
}

static void destroy(MwWidget handle) {
	MwEntryClass->destroy(handle);
}

static void draw(MwWidget handle) {
	int	  w = MwGetInteger(handle, MwNwidth);
	int	  h = MwGetInteger(handle, MwNheight);
	MwEntry	  e = handle->internal;
	MwRect	  r;
	int	  pr;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	MwEntryClass->draw(handle);

	pr	 = (handle->pressed && e->mouse.x >= (w - e->right) && e->mouse.y < (h / 2)) ? 1 : 0;
	r.x	 = w - e->right;
	r.y	 = 0;
	r.width	 = e->right;
	r.height = h / 2;
	MwDrawFrame(handle, &r, base, pr);
	MwDrawRect(handle, &r, base);

	if(r.width > r.height) {
		r.width = r.height;
	} else {
		r.height = r.width;
	}
	r.x = r.x + (e->right - r.width) / 2;
	r.y = r.y + (h / 2 - r.height) / 2;
	MwDrawTriangle(handle, &r, base, pr, MwNORTH);

	pr	 = (handle->pressed && e->mouse.x >= (w - e->right) && e->mouse.y >= (h / 2)) ? 1 : 0;
	r.x	 = w - e->right;
	r.y	 = h / 2;
	r.width	 = e->right;
	r.height = h / 2;
	MwDrawFrame(handle, &r, base, pr);
	MwDrawRect(handle, &r, base);

	if(r.width > r.height) {
		r.width = r.height;
	} else {
		r.height = r.width;
	}
	r.x = r.x + (e->right - r.width) / 2;
	r.y = r.y + (h / 2 - r.height) / 2;
	MwDrawTriangle(handle, &r, base, pr, MwSOUTH);

	MwLLFreeColor(base);
}

static void key(MwWidget handle, int code) {
	MwEntry	    e	= handle->internal;
	const char* str = MwGetText(handle, MwNtext);
	int	    ok	= 0;
	if(str == NULL) str = "";

	if(code == '-' && e->cursor == 0 && strchr(str, (int)'-') == NULL) {
		ok = 1;
	} else if('0' <= code && code <= '9') {
		ok = 1;
	} else if(code == '.' && strchr(str, (int)'.') == NULL) {
		ok = 1;
	} else if(code == MwLLKeyBackSpace || code == MwLLKeyLeft || code == MwLLKeyRight) {
		ok = 1;
	}

	if(ok) MwEntryClass->key(handle, code);
}

static void mouse_move(MwWidget handle) {
	MwEntry e = handle->internal;
	int	w = MwGetInteger(handle, MwNwidth);
	if(handle->mouse_point.x >= (w - e->right)) {
		MwLLSetCursor(handle->lowlevel, &MwCursorDefault, &MwCursorDefaultMask);
	} else {
		MwLLSetCursor(handle->lowlevel, &MwCursorText, &MwCursorTextMask);
	}
}

static void mouse_up(MwWidget handle) {
	MwEntry	    e	= handle->internal;
	int	    w	= MwGetInteger(handle, MwNwidth);
	int	    h	= MwGetInteger(handle, MwNheight);
	const char* str = MwGetText(handle, MwNtext);

	if(e->mouse.x >= (w - e->right)) {
		char s[512];
		if(e->mouse.y >= (h / 2)) {
			sprintf(s, "%g", atof(str) - 1);
		} else {
			sprintf(s, "%g", atof(str) + 1);
		}
		MwSetText(handle, MwNtext, s);
	}

	MwForceRender(handle);
}

static void mouse_down(MwWidget handle) {
	MwEntry e = handle->internal;

	e->mouse = handle->mouse_point;

	MwForceRender(handle);
}

MwClassRec MwNumberEntryClassRec = {
    create,	/* create */
    destroy,	/* destroy */
    draw,	/* draw */
    NULL,	/* click */
    NULL,	/* parent_resize */
    NULL,	/* prop_change */
    mouse_move, /* mouse_move */
    mouse_up,	/* mouse_up */
    mouse_down, /* mouse_down */
    key,	/* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwNumberEntryClass = &MwNumberEntryClassRec;
