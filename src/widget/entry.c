/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwEntry t = malloc(sizeof(*t));

	t->cursor	 = 0;
	t->right	 = 0;
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
	r.width	 = MwGetInteger(handle, MwNwidth) - t->right;
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawWidgetBack(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0, MwTRUE);
	if(str != NULL) {
		int	w = MwTextWidth(handle, "M");
		int	h = MwTextHeight(handle, "M");
		MwPoint p;
		char*	show;
		int	len;
		int	i;
		int	start;
		int	textlen;
		int	attr;
		MwRect	currc;

		p.x = (r.width - (r.width / w * w)) / 2;
		p.y = r.height / 2;

		len = (r.width - p.x * 2) / w;

		show = malloc(len * 4 + 1);
		memset(show, 0, len * 4 + 1);

		start = (t->cursor - 1) / len;
		start *= len;
		if((attr = MwGetInteger(handle, MwNhideInput)) == MwDEFAULT || !attr) {
			MwUTF8Copy(str, start, show, 0, len);
		} else {
			for(i = 0; i < MwUTF8Length(str) - start; i++) show[i] = '*';
		}

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
	} else if(code == MwLLKeyEnter) {
		MwDispatchUserHandler(handle, MwNactivateHandler, NULL);
	} else if(code == (MwLLControlMask | 'v')){
		char* c = MwLLGetClipboard(handle->lowlevel);
		if(c != NULL){
			char* out = malloc(strlen(str) + strlen(c) + 1);

			MwUTF8Copy(str, 0, out, 0, t->cursor);
			MwUTF8Copy(c, 0, out, t->cursor, MwUTF8Length(c));
			MwUTF8Copy(str, t->cursor, out, t->cursor + MwUTF8Length(c), MwUTF8Length(str) - t->cursor);

			t->cursor += MwUTF8Length(c);

			MwSetText(handle, MwNtext, out);
			free(out);
			free(c);
		}
	} else if(!(code & MwLLKeyMask)) {
		int incr = 0;
		out	 = malloc(strlen(str) + 5 + 1);
		incr += MwUTF8Copy(str, 0, out, 0, t->cursor);
		MwUTF32ToUTF8(code, out + incr);
		MwUTF8Copy(str, t->cursor, out, t->cursor + 1, MwUTF8Length(str) - t->cursor);

		t->cursor++;

		MwSetText(handle, MwNtext, out);

		free(out);
	}

	MwForceRender(handle);
}

static void prop_change(MwWidget handle, const char* prop) {
	if(strcmp(prop, MwNtext) == 0 || strcmp(prop, MwNhideInput) == 0) MwForceRender(handle);
}

MwClassRec MwEntryClassRec = {
    create,	    /* create */
    destroy,	    /* destroy */
    draw,	    /* draw */
    NULL,	    /* click */
    NULL,	    /* parent_resize */
    prop_change,    /* prop_change */
    NULL,	    /* mouse_move */
    MwForceRender2, /* mouse_up */
    MwForceRender2, /* mouse_down */
    key,	    /* key */
    NULL,	    /* execute */
    NULL,	    /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwEntryClass = &MwEntryClassRec;
