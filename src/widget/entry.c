/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwEntry t = malloc(sizeof(*t));

	t->cursor	 = 0;
	handle->internal = t;

	MwSetDefault(handle);

	MwLLSetCursor(handle->lowlevel, &MwCursorText, &MwCursorTextMask);

	return 0;
}

static void destroy(MwWidget handle) {
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	    r;
	MwEntry	    t	 = handle->internal;
	MwLLColor   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	const char* str	 = MwGetText(handle, MwNtext);
	if(str == NULL) str = "";

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0);
	MwDrawRect(handle, &r, base);
	if(str != NULL) {
		int	w = MwTextWidth(handle, "M");
		int	h = MwTextHeight(handle, "M");
		MwPoint p;
		char*	show;
		int	len;
		int	i;
		int	start;
		int	textlen;
		MwRect	currc;

		p.x = (r.width - (r.width / w * w)) / 2;
		p.y = r.height / 2;

		len = (r.width - p.x * 2) / w;

		show = malloc(len * 4 + 1);
		memset(show, 0, len * 4 + 1);

		start = (t->cursor - 1) / len;
		start *= len;
		MwUTF8Copy(str, start, show, 0, len);

		MwDrawText(handle, &p, show, 0, MwALIGNMENT_BEGINNING, text);

		textlen = (t->cursor - 1) % len + 1;
		for(i = 0; i < textlen; i++) show[i] = 'M';
		show[i] = 0;

		currc.x	     = p.x + MwTextWidth(handle, show);
		currc.y	     = (r.height - h) / 2;
		currc.width  = 1;
		currc.height = h;
		MwDrawRect(handle, &currc, text);

		free(show);
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void key(MwWidget handle, int code) {
	MwEntry	    t	= handle->internal;
	const char* str = MwGetText(handle, MwNtext);
	char*	    out;
	if(str == NULL) str = "";

	if(code == MwLLKeyBackSpace) {
		if(t->cursor == 0) return;
		out = malloc(strlen(str) + 1);

		t->cursor--;

		MwUTF8Copy(str, 0, out, 0, t->cursor);
		MwUTF8Copy(str, t->cursor + 1, out, t->cursor, MwUTF8Length(str) - (t->cursor + 1));

		MwSetText(handle, MwNtext, out);

		free(out);
	} else if(code == MwLLKeyLeft) {
		if(t->cursor == 0) return;
		t->cursor--;
	} else if(code == MwLLKeyRight) {
		if(t->cursor == MwUTF8Length(str)) return;
		t->cursor++;
	} else {
		int incr = 0;
		out	 = malloc(strlen(str) + 1 + 1);
		incr += MwUTF8Copy(str, 0, out, 0, t->cursor);
		MwUTF32ToUTF8(code, out + incr);
		MwUTF8Copy(str, t->cursor, out, t->cursor + 1, MwUTF8Length(str) - t->cursor);

		t->cursor++;

		MwSetText(handle, MwNtext, out);

		free(out);
	}

	MwForceRender(handle);
}

MwClassRec MwEntryClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
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
MwClass MwEntryClass = &MwEntryClassRec;
