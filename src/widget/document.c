#include <Mw/Milsko.h>

#include "../../external/md4c.h"
#include "../../external/stb_ds.h"

#define IsFontChange(x) ((x) == MwDOCUMENT_HEADER || (x) == MwDOCUMENT_BOLD || (x) == MwDOCUMENT_MONOSPACE)

static void reset(MwWidget handle) {
	MwDocument d = handle->internal;
	int	   i;

	for(i = 0; i < arrlen(d->layouts); i++) {
		if(d->layouts[i].text != NULL) free(d->layouts[i].text);
	}
	arrfree(d->layouts);

	for(i = 0; i < arrlen(d->clickables); i++) {
		if(d->clickables[i].event != NULL) free(d->clickables[i].event);
	}
	arrfree(d->clickables);
}

static int wcreate(MwWidget handle) {
	MwDocument d = malloc(sizeof(*d));
	int	   i;

	memset(d, 0, sizeof(*d));

	for(i = 0; i < 6; i++) {
		d->headers[i] = MwFLFontLoad == NULL ? NULL : MwFLFontLoad(MwBoldTTFData, MwBoldTTFDataSize, 13 + 2 * (6 - i));
	}

	handle->internal = d;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	MwDocument d = handle->internal;
	int	   i;

	reset(handle);

	for(i = 0; i < 6; i++) {
		if(d->headers[i] != NULL) MwFLFontFree(d->headers[i]);
	}

	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwDocument d	= handle->internal;
	MwColor	   base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwColor	   text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	   r;
	int	   i;
	MwFLFont   font = NULL;
	MwPoint	   u_p, s_p, c_p;
	int	   u = 0, s = 0, c = 0;
	MwRect	   clip;

	MwGetClipFrame(handle, &clip);

	u_p.x = s_p.x = c_p.x = 0;
	u_p.y = s_p.y = c_p.y = 0;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	for(i = 0; i < arrlen(d->layouts); i++) {
		MwDocumentLayout* l = &d->layouts[i];

		switch(l->type) {
		case MwDOCUMENT_TEXT:
		{
			MwPoint p;
			int	th2;

			p.x = l->x;
			p.y = l->y + (th2 = MwTextHeight(handle, font, l->text) / 2);

			if((p.y + th2) <= clip.y || (p.y - th2) >= clip.y + clip.height) break;

			if(u || s || c) {
				MwPoint line[2];
				int	j;

				if(u_p.y != l->y) {
					u_p.x = 0;
					u_p.y = l->y;
				}
				if(s_p.y != l->y) {
					s_p.x = 0;
					s_p.y = l->y;
				}
				if(c_p.y != l->y) {
					c_p.x = 0;
					c_p.y = l->y;
				}

				for(j = 0; j < 3; j++) {
					if(j == 0) {
						if(!u) continue;
						line[0] = u_p;
						line[1] = u_p;
					} else if(j == 1) {
						if(!s) continue;
						line[0] = s_p;
						line[1] = s_p;
					} else if(j == 2) {
						if(!c) continue;
						line[0] = c_p;
						line[1] = c_p;
					}

					line[1].x = p.x + MwTextWidth(handle, font, l->text);

					if(j == 0) u_p.x = line[1].x;
					if(j == 1) s_p.x = line[1].x;
					if(j == 2) c_p.x = line[1].x;

					if(j == 2) {
						MwRect c_r;

						c_r.x	   = line[0].x;
						c_r.y	   = line[0].y;
						c_r.width  = line[1].x - line[0].x;
						c_r.height = MwTextHeight(handle, font, l->text);

						MwDrawRect(handle, &c_r, text);
					}

					if(j == 0 || j == 1) {
						line[0].y += MwTextHeight(handle, font, l->text) / 2 * (2 - j);
						line[1].y += MwTextHeight(handle, font, l->text) / 2 * (2 - j);
						MwLLLine(handle->lowlevel, line, (c ? base : text)->lowlevel);
					}
				}
			}

			handle->bgcolor = c ? text : NULL;
			MwDrawText(handle, font, &p, l->text, MwALIGNMENT_BEGINNING, c ? base : text);
			handle->bgcolor = NULL;
			break;
		}
		case MwDOCUMENT_UNDERLINE:
		case MwDOCUMENT_STRIKETHROUGH:
		case MwDOCUMENT_CLICKABLE:
		{
			if(l->type == MwDOCUMENT_UNDERLINE) {
				u = l->integer;
			} else if(l->type == MwDOCUMENT_STRIKETHROUGH) {
				s = l->integer;
			} else {
				c = l->integer;
			}

			if(l->type == MwDOCUMENT_UNDERLINE ? u : l->type == MwDOCUMENT_STRIKETHROUGH ? s
												     : c) {
				int j;

				for(j = i + 1; j < arrlen(d->layouts) && d->layouts[j].type != l->type; j++) {
					if(d->layouts[j].type == MwDOCUMENT_TEXT) {
						MwPoint p;

						p.x = d->layouts[j].x;
						p.y = d->layouts[j].y;

						if(l->type == MwDOCUMENT_UNDERLINE) {
							u_p = p;
						} else if(l->type == MwDOCUMENT_STRIKETHROUGH) {
							s_p = p;
						} else {
							c_p = p;
						}
						break;
					}
				}
			}
			break;
		}
		default:
			break;
		}

		if(IsFontChange(l->type)) font = l->font;
	}

	MwFreeColor(text);
	MwFreeColor(base);
}

static char* in_hitbox(MwWidget handle, MwPoint* p) {
	int	   i;
	MwDocument d = handle->internal;

	for(i = 0; i < arrlen(d->clickables); i++) {
		MwRect hitbox = d->clickables[i].hitbox;

		if(hitbox.x <= p->x && p->x <= (hitbox.x + hitbox.width) && hitbox.y <= p->y && p->y <= (hitbox.y + hitbox.height)) {
			return d->clickables[i].event;
		}
	}

	return NULL;
}

static void click(MwWidget handle) {
	char* event;

	if((event = in_hitbox(handle, &handle->mouse_point)) != NULL) {
		MwDispatchUserHandler(handle, MwNdocumentActivateHandler, event);
	}
}

static int enter_block(MD_BLOCKTYPE type, void* detail, void* userdata) {
	MwDocument	 d = userdata;
	MwDocumentLayout l;

	memset(&l, 0, sizeof(l));

	switch(type) {
	case MD_BLOCK_H:
	{
		l.type	  = MwDOCUMENT_HEADER;
		l.integer = ((MD_BLOCK_H_DETAIL*)detail)->level;

		arrput(d->layouts, l);
		break;
	}
	case MD_BLOCK_CODE:
	{
		l.type	  = MwDOCUMENT_MONOSPACE;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	default:
		break;
	}

	return 0;
}

static int leave_block(MD_BLOCKTYPE type, void* detail, void* userdata) {
	MwDocument	 d = userdata;
	MwDocumentLayout l;

	(void)detail;

	memset(&l, 0, sizeof(l));

	switch(type) {
	case MD_BLOCK_H:
	{
		l.type	  = MwDOCUMENT_HEADER;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	case MD_BLOCK_P:
	{
		l.type = MwDOCUMENT_NEWLINE;

		arrput(d->layouts, l);
		break;
	}
	case MD_BLOCK_CODE:
	{
		l.type	  = MwDOCUMENT_MONOSPACE;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	default:
		break;
	}

	return 0;
}

static int enter_span(MD_SPANTYPE type, void* detail, void* userdata) {
	MwDocument	 d = userdata;
	MwDocumentLayout l;

	(void)detail;

	memset(&l, 0, sizeof(l));

	switch(type) {
	case MD_SPAN_STRONG:
	{
		l.type	  = MwDOCUMENT_BOLD;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_U:
	{
		l.type	  = MwDOCUMENT_UNDERLINE;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_CODE:
	{
		l.type	  = MwDOCUMENT_MONOSPACE;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_DEL:
	{
		l.type	  = MwDOCUMENT_STRIKETHROUGH;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_A:
	{
		MD_ATTRIBUTE* href = &((MD_SPAN_A_DETAIL*)detail)->href;
		char*	      text = malloc(href->size + 1);

		memcpy(text, href->text, href->size);
		text[href->size] = 0;

		l.type	  = MwDOCUMENT_CLICKABLE;
		l.text	  = text;
		l.integer = 1;

		arrput(d->layouts, l);
		break;
	}
	default:
		break;
	}

	return 0;
}

static int leave_span(MD_SPANTYPE type, void* detail, void* userdata) {
	MwDocument	 d = userdata;
	MwDocumentLayout l;

	(void)detail;

	memset(&l, 0, sizeof(l));

	switch(type) {
	case MD_SPAN_STRONG:
	{
		l.type	  = MwDOCUMENT_BOLD;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_U:
	{
		l.type	  = MwDOCUMENT_UNDERLINE;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_CODE:
	{
		l.type	  = MwDOCUMENT_MONOSPACE;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_DEL:
	{
		l.type	  = MwDOCUMENT_STRIKETHROUGH;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	case MD_SPAN_A:
	{
		l.type	  = MwDOCUMENT_CLICKABLE;
		l.integer = 0;

		arrput(d->layouts, l);
		break;
	}
	default:
		break;
	}

	return 0;
}

static int text(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* userdata) {
	MwDocument	 d = userdata;
	MwDocumentLayout l;

	memset(&l, 0, sizeof(l));

	switch(type) {
	case MD_TEXT_NORMAL:
	case MD_TEXT_CODE:
	{
		char* str   = malloc(size + 1);
		char* begin = str;
		char* next;

		memcpy(str, text, size);
		str[size] = 0;

		do {
			next = strchr(str, ' ');
			if(next != NULL) next[0] = 0;

			if(strcmp(str, "\n") == 0) {
				memset(&l, 0, sizeof(l));

				l.type = MwDOCUMENT_NEWLINE;

				arrput(d->layouts, l);
			} else if(strlen(str) > 0) {
				memset(&l, 0, sizeof(l));

				l.type = MwDOCUMENT_TEXT;
				l.text = MwStringDuplicate(str);

				arrput(d->layouts, l);
			} else {
				memset(&l, 0, sizeof(l));

				l.type = MwDOCUMENT_SPACE;

				arrput(d->layouts, l);
			}

			str = next + 1;
		} while(next != NULL);

		free(begin);
		break;
	}
	case MD_TEXT_BR:
	{
		l.type = MwDOCUMENT_NEWLINE;

		arrput(d->layouts, l);
		break;
	}
	default:
		break;
	}

	return 0;
}

#define TOPFONT fontstack[arrlen(fontstack) - 1]
#define POP arrdel(fontstack, arrlen(fontstack) - 1);

static void add_clickable(MwDocument d, char* c_title, MwRect* clickable) {
	MwDocumentClickable c;

	c.hitbox = *clickable;
	c.event	 = MwStringDuplicate(c_title);

	arrput(d->clickables, c);
}

static void mouse_move(MwWidget handle) {
	char*	   event = in_hitbox(handle, &handle->mouse_point);
	MwDocument d	 = handle->internal;

	if(d->center_cursor && event == NULL) {
		MwLLSetCursor(handle->lowlevel, &MwCursorDefault, &MwCursorDefaultMask);

		d->center_cursor = 0;
	} else if(!d->center_cursor && event != NULL) {
		MwLLSetCursor(handle->lowlevel, &MwCursorCenter, &MwCursorCenterMask);

		d->center_cursor = 1;
	}
}

static void layout(MwWidget handle) {
	MwDocument d = handle->internal;
	int	   w = MwGetInteger(handle, MwNwidth);
	int	   i;
	int	   x	     = 0;
	int	   y	     = 0;
	MwFLFont*  fontstack = NULL;
	MwRect	   clickable;
	char*	   c_title = NULL;
	MwRect	   size;

	arrput(fontstack, NULL);

	for(i = 0; i < arrlen(d->clickables); i++) {
		if(d->clickables[i].event != NULL) free(d->clickables[i].event);
	}
	arrfree(d->clickables);

	for(i = 0; i < arrlen(d->layouts); i++) {
		MwDocumentLayout* l = &d->layouts[i];

		switch(l->type) {
		case MwDOCUMENT_TEXT:
		{
			int t = MwTextWidth(handle, TOPFONT, l->text);

			if(x > 0) x += MwTextWidth(handle, TOPFONT, ".");

			if((x + t) >= w) {
				x = 0;
				y += MwTextHeight(handle, TOPFONT, "M");
			}

			l->x = x;
			l->y = y;

			if(c_title != NULL) {
				if(clickable.y < y) {
					if(clickable.y >= 0 && clickable.width > 0) add_clickable(d, c_title, &clickable);

					clickable.x = x;
					clickable.y = y;
				}
			}

			x += t;

			if(c_title != NULL) {
				clickable.width = x - clickable.x;
			}

			break;
		}
		case MwDOCUMENT_SPACE:
		{
			x += MwTextWidth(handle, TOPFONT, ".");
			break;
		}
		case MwDOCUMENT_NEWLINE:
		{
			x = 0;
			y += MwTextHeight(handle, TOPFONT, "M");

			if(c_title != NULL) {
				add_clickable(d, c_title, &clickable);
			}

			break;
		}
		case MwDOCUMENT_BOLD:
		{
			if(l->integer) {
				MwFLFont f = (void*)((size_t)(TOPFONT) | MwFLFlagBold);

				arrput(fontstack, f);
			} else {
				POP;
			}
			break;
		}
		case MwDOCUMENT_MONOSPACE:
		{
			if(l->integer) {
				MwFLFont f = (void*)((size_t)(TOPFONT) | MwFLFlagMonospace);

				arrput(fontstack, f);
			} else {
				POP;
			}
			break;
		}
		case MwDOCUMENT_HEADER:
		{
			if(l->integer) {
				arrput(fontstack, d->headers[l->integer - 1]);
			} else {
				x = 0;
				y += MwTextHeight(handle, TOPFONT, "M");

				POP;
			}
			break;
		}
		case MwDOCUMENT_CLICKABLE:
		{
			if(l->integer) {
				clickable.y	 = -100;
				clickable.width	 = 0;
				clickable.height = MwTextHeight(handle, TOPFONT, "M");

				c_title = MwStringDuplicate(l->text);
			} else {
				if(clickable.y >= 0 && clickable.width > 0) add_clickable(d, c_title, &clickable);

				free(c_title);
				c_title = NULL;
			}
			break;
		}
		}

		l->font = TOPFONT;
	}

	size.width  = w;
	size.height = y + MwTextHeight(handle, TOPFONT, "M");
	MwDispatchUserHandler(handle, MwNlayoutHandler, &size);

	arrfree(fontstack);

	mouse_move(handle);
	MwForceRender(handle);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtext) == 0) MwForceRender(handle);

	if(strcmp(key, MwNtext) == 0) {
		MD_PARSER   parser;
		const char* str = MwGetText(handle, MwNtext);

		memset(&parser, 0, sizeof(parser));

		parser.flags	   = MD_FLAG_UNDERLINE | MD_FLAG_STRIKETHROUGH;
		parser.enter_block = enter_block;
		parser.leave_block = leave_block;
		parser.enter_span  = enter_span;
		parser.leave_span  = leave_span;
		parser.text	   = text;

		reset(handle);

		md_parse(str, strlen(str), &parser, handle->internal);

		layout(handle);
	}
}

static void resize(MwWidget handle) {
	layout(handle);
}

MwClassRec MwDocumentClassRec = {
    wcreate,	 /* create */
    destroy,	 /* destroy */
    draw,	 /* draw */
    click,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    mouse_move,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
    NULL,	 /* key */
    NULL,	 /* execute */
    NULL,	 /* tick */
    resize,	 /* resize */
    NULL,	 /* children_update */
    NULL,	 /* children_prop_change */
    NULL,	 /* clipboard */
    NULL,	 /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwDocumentClass = &MwDocumentClassRec;
