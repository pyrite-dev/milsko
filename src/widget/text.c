/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	MwLLSetCursor(handle->lowlevel, &MwCursorText, &MwCursorTextMask);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	    r;
	MwLLColor   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	const char* str	 = MwGetText(handle, MwNtext);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0);
	MwDrawRect(handle, &r, base);
	if(str != NULL) {
		int	h = MwTextHeight(handle, "M");
		MwPoint p;

		p.x = (r.height - h) / 2;
		p.y = r.height / 2;

		/* limit so there isn't a crazy padding */
		if(p.x > 4) p.x = 4;

		MwDrawText(handle, &p, str, 0, MwALIGNMENT_BEGINNING, text);
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void key(MwWidget handle, int code) {
	const char* str = MwGetText(handle, MwNtext);
	char*	    out;
	char	    buf[2];
	if(str == NULL) str = "";

	if(code == MwLLKeyBackSpace) {
		if(strlen(str) == 0) return;
		out = malloc(strlen(str) + 1);

		strcpy(out, str);
		out[strlen(out) - 1] = 0;

		MwSetText(handle, MwNtext, out);

		free(out);
	} else {
		buf[0] = code;
		buf[1] = 0;

		out = malloc(strlen(str) + 1 + 1);
		strcpy(out, str);
		strcat(out, buf);

		MwSetText(handle, MwNtext, out);

		free(out);
	}

	MwForceRender(handle);
}

MwClassRec MwTextClassRec = {
    create,	   /* create */
    NULL,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    NULL,	   /* prop_change */
    NULL,	   /* mouse_move */
    MwForceRender, /* mouse_up */
    MwForceRender, /* mouse_down */
    key,	   /* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwTextClass = &MwTextClassRec;
