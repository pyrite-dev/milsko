#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static void layout(MwWidget handle);
static void calc(MwWidget handle);

static int wcreate(MwWidget handle) {
	MwText	   txt = malloc(sizeof(*txt));
	MwTextLine l;

	l.line	= MwStringDuplicate("");
	l.width = 0;

	txt->lines	= NULL;
	txt->width	= 0;
	txt->cursor.x	= 0;
	txt->cursor.y	= 0;
	txt->force_prop = 0;
	txt->result	= NULL;
	txt->line	= 0;
	txt->index	= 0;

	arrput(txt->lines, l);

	handle->internal = txt;

	MwSetDefault(handle);

	MwLLSetCursor(handle->lowlevel, &MwCursorText, &MwCursorTextMask);

	return 0;
}

static void destroy(MwWidget handle) {
	MwText txt = handle->internal;
	int    i;

	for(i = 0; i < arrlen(txt->lines); i++) {
		free(txt->lines[i].line);
	}
	arrfree(txt->lines);

	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwColor base = MwParseColor(handle, MwGetString(handle, MwNbackground));
	MwColor text = MwParseColor(handle, MwGetString(handle, MwNforeground));
	MwRect	r;
	int	i;
	MwText	txt = handle->internal;
	MwPoint p   = {0};
	MwRect	clip;
	int	th2;
	MwRect	cursor;

	MwGetClipFrame(handle, &clip);

	MwGetFrame(handle, &r);
	MwDrawRect(handle, &r, base);

	p.y += th2 = MwTextHeight(handle, NULL, "M") / 2;

	for(i = 0; i < arrlen(txt->lines); i++) {
		if(clip.y <= (p.y - th2) || (p.y + th2) <= (clip.y + clip.height)) {
			MwDrawText(handle, NULL, &p, txt->lines[i].line, MwALIGNMENT_BEGINNING, text);
		}
		p.y += MwTextHeight(handle, NULL, "M");
	}

	cursor.x      = txt->cursor.x;
	cursor.y      = txt->cursor.y;
	cursor.width  = 1;
	cursor.height = MwTextHeight(handle, NULL, "M");
	MwDrawRect(handle, &cursor, text);

	MwFreeColor(text);
	MwFreeColor(base);
}

static void layout(MwWidget handle) {
	MwRect r   = {0};
	MwText txt = handle->internal;

	r.width	 = txt->width;
	r.height = MwTextHeight(handle, NULL, "M") * arrlen(txt->lines);

	MwDispatchUserHandler(handle, MwNlayoutHandler, &r);
}

static void calc(MwWidget handle) {
	char*  text;
	char*  last;
	int    i;
	MwText txt = handle->internal;

	txt->force_prop = 1;
	text		= (char*)MwGetString(handle, MwNtext);
	last		= text;
	txt->force_prop = 0;

	txt->width = 0;
	for(i = 0; i < arrlen(txt->lines); i++) {
		free(txt->lines[i].line);
	}
	arrfree(txt->lines);

	while(1) {
		if(*text == '\n' || *text == 0) {
			int	   b = *text == 0;
			char*	   t = malloc(text - last + 1);
			MwTextLine l;

			memcpy(t, last, text - last);
			t[text - last] = 0;

			for(i = 0; t[i] != 0; i++) {
				if(t[i] == '\r') t[i] = 0;
			}

			l.line	= t;
			l.width = MwTextWidth(handle, NULL, t);

			if(txt->width < l.width) txt->width = l.width;

			arrput(txt->lines, l);

			if(b) break;
			last = ++text;
		} else {
			text++;
		}
	}
}

static void calc_width(MwWidget handle) {
	int    i;
	MwText txt = handle->internal;

	txt->width = 0;
	for(i = 0; i < arrlen(txt->lines); i++) {
		if(txt->width < txt->lines[i].width) txt->width = txt->lines[i].width;
	}
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtext) == 0) {
		calc(handle);
		layout(handle);
		MwForceRender(handle);
	}
}

static void mouse_down(MwWidget handle, void* mouse) {
	int	 th  = MwTextHeight(handle, NULL, "M");
	int	 th2 = th / 2;
	int	 i;
	MwText	 txt = handle->internal;
	MwMouse* m   = mouse;
	MwPoint	 p   = {0};

	for(i = 0; i < arrlen(txt->lines); i++) {
		if(p.y <= m->point.y && m->point.y <= (p.y + th)) {
			char* text = txt->lines[i].line;
			char* line = malloc(strlen(text) + 1);
			line[0]	   = 0;

			p.x = 0;
			do {
				int c;
				int tw = MwTextWidth(handle, NULL, line);
				int c2;
				int l = strlen(line);

				text += MwUTF8ToUTF32(text, &c);

				line[l + MwUTF32ToUTF8(c, line + l)] = 0;

				c2 = MwTextWidth(handle, NULL, line + l) / 2;

				p.x = tw + c2;

				if((p.x - c2) <= m->point.x && m->point.x <= p.x) {
					/* left */
					txt->cursor.x = p.x - c2;
					txt->cursor.y = p.y;
					txt->line     = i;
					txt->index    = l;
					break;
				} else if(p.x <= m->point.x && ((*text) == 0 ? 1 : (m->point.x <= (p.x + c2)))) {
					/* right */
					txt->cursor.x = p.x + c2;
					txt->cursor.y = p.y;
					txt->line     = i;
					txt->index    = strlen(line);
					break;
				}

				p.x = MwTextWidth(handle, NULL, line);
			} while(*text);

			free(line);
		}
		p.y += th;
	}

	MwForceRender(handle);
}

static void key(MwWidget handle, int key) {
	MwText	txt = handle->internal;
	int	i;
	char	u[5];
	char*	l;
	int	tw;
	MwPoint p;

	while(arrlen(txt->lines) < txt->line) {
		MwTextLine l;

		l.line	= MwStringDuplicate("");
		l.width = 0;

		arrput(txt->lines, l);
	}

	if(key == MwKEY_ENTER) {
		MwTextLine l;

		l.line	= MwStringDuplicate(txt->lines[txt->line].line + txt->index);
		l.width = MwTextWidth(handle, NULL, l.line);

		txt->lines[txt->line].line[txt->index] = 0;
		txt->lines[txt->line].width	       = MwTextWidth(handle, NULL, txt->lines[txt->line].line);

		arrins(txt->lines, txt->line + 1, l);
		calc_width(handle);
		layout(handle);

		txt->line++;
		txt->index    = 0;
		txt->cursor.x = 0;
		txt->cursor.y += MwTextHeight(handle, NULL, "M");

		p.x = 0;
		p.y = txt->cursor.y;

		MwForceRender(handle);
		MwDispatchUserHandler(handle, MwNchangedHandler, &p);
		return;
	} else if(key == MwKEY_BACKSPACE) {
		if(txt->index > 0) {
			char* text = txt->lines[txt->line].line;
			int   b	   = 0;

			l    = malloc(strlen(text) + 1); /* this is technically wrong... */
			l[0] = 0;

			while(*text) {
				int c;

				text += b	       = MwUTF8ToUTF32(text, &c);
				u[MwUTF32ToUTF8(c, u)] = 0;

				if((text - txt->lines[txt->line].line) == txt->index) {
					txt->cursor.x -= MwTextWidth(handle, NULL, u);
					txt->index -= b;
				} else {
					strcat(l, u);
				}
			}

			free(txt->lines[txt->line].line);
			txt->lines[txt->line].line  = l;
			txt->lines[txt->line].width = MwTextWidth(handle, NULL, txt->lines[txt->line].line);
		} else if(txt->line > 0) {
			char* text = MwStringConcat(txt->lines[txt->line - 1].line, txt->lines[txt->line].line);

			free(txt->lines[txt->line].line);
			arrdel(txt->lines, txt->line);

			txt->line--;
			txt->index    = strlen(txt->lines[txt->line].line);
			txt->cursor.x = MwTextWidth(handle, NULL, txt->lines[txt->line].line);
			txt->cursor.y -= MwTextHeight(handle, NULL, "M");

			free(txt->lines[txt->line].line);
			txt->lines[txt->line].line  = text;
			txt->lines[txt->line].width = MwTextWidth(handle, NULL, txt->lines[txt->line].line);
		}

		calc_width(handle);
		layout(handle);

		p.x = txt->cursor.x;
		p.y = txt->cursor.y;

		MwForceRender(handle);
		MwDispatchUserHandler(handle, MwNchangedHandler, &p);
		return;
	} else if(key == MwKEY_LEFT || key == MwKEY_RIGHT) {
		if(key == MwKEY_LEFT ? (txt->index > 0) : (txt->index < strlen(txt->lines[txt->line].line))) {
			char* text = txt->lines[txt->line].line;
			int   m	   = key == MwKEY_LEFT ? -1 : 1;

			if(m == -1 && txt->index == 1) {
				txt->index    = 0;
				txt->cursor.x = 0;
			} else {
				l    = malloc(strlen(text) + 1);
				l[0] = 0;

				while(*text) {
					int c;

					text += MwUTF8ToUTF32(text, &c);
					u[MwUTF32ToUTF8(c, u)] = 0;
					strcat(l, u);

					if((text - txt->lines[txt->line].line) == (txt->index + m)) {
						txt->cursor.x = MwTextWidth(handle, NULL, l);
						break;
					}
				}

				txt->index += m;

				free(l);
			}
		}

		calc_width(handle);
		layout(handle);
		MwForceRender(handle);
		return;
	} else if(key == (MwKEY_CONTROL_FLAG | 'v')) {
		MwLLGetClipboard(handle->lowlevel, MwCLIPBOARD_MAIN);
		return;
	}

	if(key & MwKEY_FLAG) return;

	u[MwUTF32ToUTF8(key, u)] = 0;

	l = malloc(strlen(txt->lines[txt->line].line) + strlen(u) + 1);

	memcpy(l, txt->lines[txt->line].line, txt->index);
	memcpy(l + txt->index, u, strlen(u));
	l[txt->index + strlen(u)] = 0;
	tw			  = MwTextWidth(handle, NULL, l);

	memcpy(l + txt->index + strlen(u), txt->lines[txt->line].line + txt->index, strlen(txt->lines[txt->line].line + txt->index));
	l[strlen(txt->lines[txt->line].line) + strlen(u)] = 0;

	free(txt->lines[txt->line].line);
	txt->lines[txt->line].line  = l;
	txt->lines[txt->line].width = MwTextWidth(handle, NULL, txt->lines[txt->line].line);
	calc_width(handle);
	layout(handle);

	txt->cursor.x = tw;
	txt->index += strlen(u);

	p.x = txt->cursor.x;
	p.y = txt->cursor.y;

	MwForceRender(handle);
	MwDispatchUserHandler(handle, MwNchangedHandler, &p);
}

static void clipboard(MwWidget handle, const char* data) {
	MwText txt = handle->internal;
	char*  l;
	int    tw;
	char*  clip = MwStringDuplicate(data);
	char*  last = clip;
	int    i;
	int    nl	  = 0;
	int    oldindex	  = txt->index;
	char*  oldlinestr = MwStringDuplicate(txt->lines[txt->line].line);

	for(i = 0;; i++) {
		if(clip[i] == '\n' || clip[i] == 0) {
			int c = clip[i];

			clip[i] = 0;

			l = malloc(((c == 0 || nl == 0) ? strlen(oldlinestr) : 0) + strlen(last) + 1);

			if(nl == 0) memcpy(l, oldlinestr, oldindex);
			memcpy(l + (nl == 0 ? oldindex : 0), last, strlen(last));
			l[(nl == 0 ? oldindex : 0) + strlen(last)] = 0;
			tw					   = MwTextWidth(handle, NULL, l);

			if(c == 0) {
				memcpy(l + (nl == 0 ? oldindex : 0) + strlen(last), oldlinestr + oldindex, strlen(oldlinestr + oldindex));
				l[(nl == 0 ? oldindex : 0) + strlen(last)] = 0;
			}

			free(txt->lines[txt->line].line);
			txt->lines[txt->line].line  = l;
			txt->lines[txt->line].width = MwTextWidth(handle, NULL, txt->lines[txt->line].line);
			calc_width(handle);
			layout(handle);

			txt->cursor.x = tw;
			txt->index += strlen(last);

			if(c == '\n') {
				MwTextLine ln;

				ln.line	 = MwStringDuplicate("");
				ln.width = MwTextWidth(handle, NULL, ln.line);

				txt->line++;
				arrins(txt->lines, txt->line, ln);
				txt->index = 0;
				txt->cursor.y += MwTextHeight(handle, NULL, "M");
			}

			last = clip + i + 1;
			nl++;
			if(c == 0) break;
		} else if(clip[i] == '\r') {
			clip[i] = 0;
		}
	}

	free(oldlinestr);
	free(clip);

	calc_width(handle);
	layout(handle);
	MwForceRender(handle);
}

static int get_prop(MwWidget handle, const char* key, void* out) {
	if(strcmp(key, MwNtext) == 0) {
		char** s = (char**)out;
		int    i;
		MwText txt = handle->internal;
		int    sz  = 1;

		if(txt->force_prop) return -1;

		for(i = 0; i < arrlen(txt->lines); i++) {
			sz += strlen(txt->lines[i].line) + 2;
		}

		if(txt->result != NULL) free(txt->result);

		*s = txt->result = malloc(sz);
		**s		 = 0;
		(*s)[sz]	 = 0;

		for(i = 0; i < arrlen(txt->lines); i++) {
			strcat(*s, txt->lines[i].line);
#ifdef _WIN32
			strcat(*s, "\r\n");
#else
			strcat(*s, "\n");
#endif
		}

		return 0;
	}

	return -1;
}

MwClassRec MwTextClassRec = {
    wcreate,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    prop_change,   /* prop_change */
    NULL,	   /* mouse_move */
    NULL,	   /* mouse_up */
    mouse_down,	   /* mouse_down */
    key,	   /* key */
    NULL,	   /* execute */
    NULL,	   /* tick */
    MwForceRender, /* resize */
    NULL,	   /* children_update */
    NULL,	   /* children_prop_change */
    clipboard,	   /* clipboard */
    NULL,	   /* props_change */
    NULL,	   /* drag_and_drop */
    get_prop,	   /* get_prop */
    NULL};
MwClass MwTextClass = &MwTextClassRec;
