#include <Mw/Milsko.h>

#include "../../external/md4c.h"
#include "../../external/stb_ds.h"

#define IsFontChange(x) ((x) == MwDOCUMENT_HEADER || (x) == MwDOCUMENT_BOLD || (x) == MwDOCUMENT_MONOSPACE || (x) == MwDOCUMENT_STRIKETHROUGH)

static void reset(MwWidget handle) {
	MwDocument d = handle->internal;
	int	   i;

	for(i = 0; i < arrlen(d->layouts); i++) {
		if(d->layouts[i].text != NULL) free(d->layouts[i].text);
	}
	arrfree(d->layouts);
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
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor  text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	   r;
	int	   i;
	MwFLFont   font = NULL;
	MwPoint	   u_p, s_p;
	int	   u = 0, s = 0;

	u_p.x = u_p.y = 0;
	s_p.x = s_p.y = 0;

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

			p.x = l->x;
			p.y = l->y + MwTextHeight(handle, font, l->text) / 2;

			MwDrawText(handle, font, &p, l->text, MwALIGNMENT_BEGINNING, text);

			if(u || s) {
				MwPoint line[2];
				int	j;

				if(u_p.y != l->y) {
					u_p.x = 0;
					u_p.y = l->y;
				}

				line[0] = u_p;
				line[1] = u_p;

				line[1].x = p.x + MwTextWidth(handle, font, l->text);
				u_p.x	  = line[1].x;

				for(j = 0; j < 2; j++) {
					line[0].y += MwTextHeight(handle, font, l->text) / 2;
					line[1].y += MwTextHeight(handle, font, l->text) / 2;
					if((j == 0 && s) || (j == 1 && u)) MwLLLine(handle->lowlevel, line, text);
				}
			}
			break;
		}
		case MwDOCUMENT_UNDERLINE:
		case MwDOCUMENT_STRIKETHROUGH:
		{
			if(l->type == MwDOCUMENT_UNDERLINE) {
				u = l->integer;
			} else {
				s = l->integer;
			}

			if(l->type == MwDOCUMENT_UNDERLINE ? u : s) {
				int j;

				for(j = i + 1; j < arrlen(d->layouts) && d->layouts[j].type != l->type; j++) {
					if(d->layouts[j].type == MwDOCUMENT_TEXT) {
						MwPoint p;

						p.x = d->layouts[j].x;
						p.y = d->layouts[j].y;

						if(l->type == MwDOCUMENT_UNDERLINE) {
							u_p = p;
						} else {
							s_p = p;
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

	MwLLFreeColor(text);
	MwLLFreeColor(base);
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

			if(strlen(str) > 0) {
				memset(&l, 0, sizeof(l));

				l.type = MwDOCUMENT_TEXT;
				l.text = MwStringDuplicate(str);

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

static void layout(MwWidget handle) {
	MwDocument d = handle->internal;
	int	   w = MwGetInteger(handle, MwNwidth);
	int	   i;
	int	   x	     = 0;
	int	   y	     = 0;
	MwFLFont*  fontstack = NULL;

	arrput(fontstack, NULL);

	for(i = 0; i < arrlen(d->layouts); i++) {
		MwDocumentLayout* l = &d->layouts[i];

		switch(l->type) {
		case MwDOCUMENT_TEXT:
		{
			int t = MwTextWidth(handle, TOPFONT, l->text);

			if(x > 0) x += MwTextWidth(handle, TOPFONT, " ");

			if((x + t) >= w) {
				x = 0;
				y += MwTextHeight(handle, TOPFONT, "M");
			}

			l->x = x;
			l->y = y;

			x += t;
			break;
		}
		case MwDOCUMENT_NEWLINE:
		{
			x = 0;
			y += MwTextHeight(handle, TOPFONT, "M");
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
		}

		l->font = TOPFONT;
	}

	y += MwTextHeight(handle, TOPFONT, "M");
	MwDispatchUserHandler(handle, MwNlayoutHandler, &y);

	arrfree(fontstack);

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
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
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
