/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

#define Padding 2

MwListBoxPacket* MwListBoxCreatePacket(void) {
	MwListBoxPacket* packet = malloc(sizeof(*packet));
	memset(packet, 0, sizeof(*packet));

	return packet;
}

void MwListBoxDestroyPacket(MwListBoxPacket* packet) {
	int i;
	int j;

	for(i = 0; i < arrlen(packet->names); i++) {
		for(j = 0; j < arrlen(packet->names[i]); j++) {
			if(packet->names[i][j] != NULL) free(packet->names[i][j]);
		}
		arrfree(packet->names[i]);
	}

	arrfree(packet->names);
	arrfree(packet->pixmaps);

	free(packet);
}

int MwListBoxPacketInsert(MwListBoxPacket* packet, int index) {
	int i;

	if(index == -1) index = arrlen(packet->names);
	for(i = arrlen(packet->names); i < index; i++) {
		arrput(packet->names, NULL);
		arrput(packet->pixmaps, NULL);
	}

	arrins(packet->names, index, NULL);
	arrins(packet->pixmaps, index, NULL);

	return index;
}

void MwListBoxPacketSet(MwListBoxPacket* packet, int index, int col, const char* text) {
	char* t = text == NULL ? NULL : MwStringDuplicate(text);
	int   i;

	if(col == -1) col = arrlen(packet->names[index]);
	for(i = arrlen(packet->names[index]); i < col; i++) {
		arrput(packet->names[index], NULL);
	}

	arrins(packet->names[index], col, t);
}

void MwListBoxPacketSetIcon(MwListBoxPacket* packet, int index, MwLLPixmap icon) {
	packet->pixmaps[index] = icon;
}

static int get_first_entry(MwWidget handle, MwListBox lb) {
	int st = 0;
	int y  = MwGetInteger(handle, MwNhasHeading) ? 1 : 0;

	st = MwGetInteger(lb->vscroll, MwNvalue);
	st = st * (MwGetInteger(lb->vscroll, MwNmaxValue) - MwGetInteger(lb->vscroll, MwNareaShown)) / MwGetInteger(lb->vscroll, MwNmaxValue);
	if(st < y) st = y;

	return st;
}

static int get_col_width(MwListBox lb, int ind) {
	int total = MwGetInteger(lb->frame, MwNwidth);
	int wid, i;
	if(arrlen(lb->width) <= ind) {
		wid = 0;
	} else {
		wid = lb->width[ind];
	}
	if(wid > 0) return wid;

	if(wid == 0) {
		for(i = 0; i < arrlen(lb->width) && i < ind; i++) {
			int w = lb->width[i];
			if(w < 0) {
				total -= total + w;
			} else if(wid > 0) {
				total -= w;
			}
		}
		return total;
	}

	if(wid < 0) {
		for(i = 0; i < arrlen(lb->width) && i < ind; i++) {
			int w = lb->width[i];
			if(w < 0) {
				total -= total + w;
			} else if(wid > 0) {
				total -= w;
			}
		}
		return total + wid;
	}

	return 0;
}

static void vscroll_changed(MwWidget handle, void* user, void* call) {
	MwListBox lb = handle->parent->internal;

	(void)user;
	(void)call;

	lb->changed = 1;
}

static void frame_mouse_down(MwWidget handle, void* user, void* call) {
	MwListBox  lb = handle->parent->internal;
	MwLLMouse* m  = call;

	(void)user;

	if(m->button == MwLLMouseLeft) {
		int st = 0;
		int i;
		int y = MwDefaultBorderWidth(handle);
		int h = MwGetInteger(handle, MwNheight);

		st = get_first_entry(handle->parent, lb);
		for(i = 0; (st + i) < arrlen(lb->list) && i < (h - MwDefaultBorderWidth(handle) * 2) / (MwTextHeight(handle, "M") + Padding) + 2; i++) {
			if(y <= m->point.y && m->point.y <= (y + MwTextHeight(handle, "M") + Padding)) {
				unsigned long t;
				int	      old = MwGetInteger(handle->parent, MwNvalue);

				MwSetInteger(handle->parent, MwNvalue, st + i);

				if((((t = MwTimeGetTick()) - lb->click_time) < MwDoubleClickTimeout && old == st + i) || MwGetInteger(handle->parent, MwNsingleClickSelectable)) {
					int n = MwGetInteger(handle->parent, MwNvalue);
					MwDispatchUserHandler(handle->parent, MwNactivateHandler, &n);
				}

				lb->click_time = t;
			}
			y += MwTextHeight(handle, "M") + Padding;
		}

		MwForceRender(lb->frame);
		lb->pressed = 1;
	}
}

static void frame_mouse_up(MwWidget handle, void* user, void* call) {
	MwListBox  lb = handle->parent->internal;
	MwLLMouse* m  = call;

	(void)user;

	if(m->button == MwLLMouseLeft) {
		lb->pressed = 0;
	}
}

static void frame_mouse_move(MwWidget handle, void* user, void* call) {
	MwListBox lb = handle->parent->internal;
	MwPoint*  p  = call;

	(void)user;

	if(lb->pressed) {
		int st = 0;
		int i;
		int y = MwDefaultBorderWidth(handle);
		int h = MwGetInteger(handle, MwNheight);

		st = get_first_entry(handle->parent, lb);
		for(i = 0; (st + i) < arrlen(lb->list) && i < (h - MwDefaultBorderWidth(handle) * 2) / (MwTextHeight(handle, "M") + Padding) + 2; i++) {
			if(y <= p->y && p->y <= (y + MwTextHeight(handle, "M") + Padding)) {
				MwSetInteger(handle->parent, MwNvalue, st + i);
			}
			y += MwTextHeight(handle, "M") + Padding;
		}

		MwForceRender(lb->frame);
	}
}

static void frame_draw(MwWidget handle) {
	MwRect	  r, r2;
	MwListBox lb	= handle->parent->internal;
	MwLLColor base	= MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor base2 = MwParseColor(handle, MwGetText(handle, MwNsubBackground));
	MwLLColor text	= MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwLLColor text2 = MwParseColor(handle, MwGetText(handle, MwNsubForeground));
	int	  i;
	MwPoint	  p;
	int	  st = 0;
	int	  ent;
	int	  area;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	r2	 = r;

	p.x = MwDefaultBorderWidth(handle);
	p.y = MwDefaultBorderWidth(handle);

	MwDrawFrame(handle, &r, base, 1);
	MwDrawRect(handle, &r, base2);

	r = r2;

	st = get_first_entry(handle->parent, lb);

	area = r.height - MwDefaultBorderWidth(handle) * 2;
	ent  = area / (MwTextHeight(handle, "M") + Padding) + 2;

	for(i = st; i < arrlen(lb->list) && i < st + ent; i++) {
		int selected = MwGetInteger(handle->parent, MwNvalue) == i ? 1 : 0;
		int j;

		if(selected) {
			MwRect r2;
			r2.x	  = MwDefaultBorderWidth(handle) + MwGetInteger(handle->parent, MwNleftPadding);
			r2.y	  = p.y;
			r2.width  = r.width - r2.x;
			r2.height = MwTextHeight(handle, "M") + Padding;
			MwDrawRect(handle, &r2, text2);
			handle->bgcolor = text2;
		}
		if(lb->list[i].pixmap != NULL) {
			MwRect r2;
			int    h  = (lb->list[i].pixmap->common.height > (MwTextHeight(handle, "M") + Padding)) ? (MwTextHeight(handle, "M") + Padding) : lb->list[i].pixmap->common.height;
			r2.x	  = MwDefaultBorderWidth(handle);
			r2.y	  = p.y + (MwTextHeight(handle, "M") + Padding - h) / 2;
			r2.width  = h * lb->list[i].pixmap->common.width / lb->list[i].pixmap->common.height;
			r2.height = h;
			MwLLDrawPixmap(handle->lowlevel, &r2, lb->list[i].pixmap);
		}
		p.y += (MwTextHeight(handle, "M") + Padding) / 2;
		p.x = MwDefaultBorderWidth(handle) + MwGetInteger(handle->parent, MwNleftPadding);
		for(j = 0; j < arrlen(lb->list[i].name); j++) {
			char* t = lb->list[i].name[j];

			if(t == NULL) t = "";

			if(j == (arrlen(lb->list[i].name) - 1)) p.x -= MwDefaultBorderWidth(handle);
			if(arrlen(lb->alignment) <= j || lb->alignment[j] == MwALIGNMENT_BEGINNING) {
				char* str = MwStringDuplicate(t);
				int   l	  = (j == 0 ? MwGetInteger(handle->parent, MwNleftPadding) : 0);

				if(MwUTF8Length(str) > (get_col_width(lb, j) - l) / MwTextWidth(handle, "M")) {
					int ind	 = (get_col_width(lb, j) - l) / MwTextWidth(handle, "M");
					str[ind] = 0;
					memset(str + ind - 3, '.', 3);
				}

				p.x += 4;
				MwDrawText(handle, &p, str, 0, MwALIGNMENT_BEGINNING, selected ? base2 : text2);
				p.x -= 4;
				p.x += get_col_width(lb, j);

				free(str);
			} else if(lb->alignment[j] == MwALIGNMENT_CENTER) {
				int l = (j == 0 ? MwGetInteger(handle->parent, MwNleftPadding) : 0);
				p.x += (get_col_width(lb, j) - l) / 2;
				MwDrawText(handle, &p, t, 0, MwALIGNMENT_CENTER, selected ? base2 : text2);
				p.x += (get_col_width(lb, j) - l) / 2;
				p.x += l;
			} else if(lb->alignment[j] == MwALIGNMENT_END) {
				p.x += get_col_width(lb, j);
				p.x -= 4;
				MwDrawText(handle, &p, t, 0, MwALIGNMENT_END, selected ? base2 : text2);
				p.x += 4;
			}

			if(j == 0) p.x -= MwGetInteger(handle->parent, MwNleftPadding);
		}
		p.y += (MwTextHeight(handle, "M") + Padding) / 2;
		handle->bgcolor = NULL;
	}

	MwDrawFrame(handle, &r, base, 1);

	MwLLFreeColor(text2);
	MwLLFreeColor(text);
	MwLLFreeColor(base2);
	MwLLFreeColor(base);
}

static void resize(MwWidget handle) {
	MwListBox lb = handle->internal;
	int	  w  = MwGetInteger(handle, MwNwidth);
	int	  h  = MwGetInteger(handle, MwNheight);
	int	  ih, y;
	int	  m = 0;

	y = MwGetInteger(handle, MwNhasHeading) ? (MwTextHeight(handle, "M") + MwDefaultBorderWidth(handle) * 2) : 0;

	if(lb->vscroll == NULL) {
		lb->vscroll = MwVaCreateWidget(MwScrollBarClass, "vscroll", handle, w - 16, 0, 16, h, NULL);
		MwAddUserHandler(lb->vscroll, MwNchangedHandler, vscroll_changed, NULL);
	} else {
		MwVaApply(lb->vscroll,
			  MwNx, w - 16,
			  MwNy, 0,
			  MwNwidth, 16,
			  MwNheight, h,
			  NULL);
	}

	ih = arrlen(lb->list);
	if(ih == 0) ih = 1;

	h -= y;

	if(ih <= (h / (MwTextHeight(handle, "M") + Padding))) {
		MwLLShow(lb->vscroll->lowlevel, 0);
	} else {
		MwLLShow(lb->vscroll->lowlevel, 1);
		m = 16;
	}

	if(lb->frame == NULL) {
		lb->frame	       = MwVaCreateWidget(MwFrameClass, "frame", handle, 0, y, w - m, h, NULL);
		lb->frame->draw_inject = frame_draw;
		MwAddUserHandler(lb->frame, MwNmouseDownHandler, frame_mouse_down, NULL);
		MwAddUserHandler(lb->frame, MwNmouseUpHandler, frame_mouse_up, NULL);
		MwAddUserHandler(lb->frame, MwNmouseMoveHandler, frame_mouse_move, NULL);
	} else {
		MwVaApply(lb->frame,
			  MwNx, 0,
			  MwNy, y,
			  MwNwidth, w - m,
			  MwNheight, h,
			  NULL);
	}
	h -= MwDefaultBorderWidth(handle) * 2;

	MwVaApply(lb->vscroll,
		  MwNareaShown, h / (MwTextHeight(handle, "M") + Padding),
		  MwNmaxValue, ih,
		  NULL);
}

static int create(MwWidget handle) {
	MwListBox lb = malloc(sizeof(*lb));
	memset(lb, 0, sizeof(*lb));

	handle->internal = lb;

	MwSetDefault(handle);

	MwSetInteger(handle, MwNsingleClickSelectable, 0);
	MwSetInteger(handle, MwNhasHeading, 0);
	MwSetInteger(handle, MwNleftPadding, 0);
	MwSetInteger(handle, MwNvalue, -1);

	resize(handle);
	lb->list       = NULL;
	lb->click_time = 0;
	lb->width      = NULL;
	lb->alignment  = NULL;
	lb->changed    = 0;

	return 0;
}

static void destroy(MwWidget handle) {
	MwListBox lb = handle->internal;
	MwListBoxReset(handle);
	arrfree(lb->list);
	arrfree(lb->width);
	arrfree(lb->alignment);
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwListBox lb   = handle->internal;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, base);
	if(MwGetInteger(handle, MwNhasHeading) && arrlen(lb->list) > 0) {
		MwPoint p;
		int	i;
		int	x = 0;

		r.width -= 16;

		for(i = 0; i < arrlen(lb->list[0].name); i++) {
			r.x	 = x;
			r.y	 = 0;
			r.width	 = get_col_width(lb, i);
			r.height = MwDefaultBorderWidth(handle) * 2 + MwTextHeight(handle, "M");
			MwDrawFrame(handle, &r, base, 0);

			x += MwDefaultBorderWidth(handle);

			if(arrlen(lb->alignment) <= i || lb->alignment[i] == MwALIGNMENT_BEGINNING) {
				p.x = 4 + x;
				p.y = r.y + r.height / 2;
				MwDrawText(handle, &p, lb->list[0].name[i], 0, MwALIGNMENT_BEGINNING, text);
			} else if(lb->alignment[i] == MwALIGNMENT_CENTER) {
				p.x = x + r.width / 2;
				p.y = r.y + r.height / 2;
				MwDrawText(handle, &p, lb->list[0].name[i], 0, MwALIGNMENT_CENTER, text);
			} else if(lb->alignment[i] == MwALIGNMENT_END) {
				p.x = x + r.width - 4;
				p.y = r.y + r.height / 2;
				MwDrawText(handle, &p, lb->list[0].name[i], 0, MwALIGNMENT_END, text);
			}

			x += r.width;

			x += MwDefaultBorderWidth(handle);
		}
	}

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* prop) {
	if(strcmp(prop, MwNwidth) == 0 || strcmp(prop, MwNheight) == 0 || strcmp(prop, MwNhasHeading) == 0) resize(handle);
	if(strcmp(prop, MwNleftPadding) == 0) {
		MwListBox lb = handle->internal;
		MwForceRender(lb->frame);
	}
	if(strcmp(prop, MwNhasHeading) == 0) {
		MwForceRender(handle);
	}
}

static void mwListBoxInsertImpl(MwWidget handle, int index, MwListBoxPacket* packet) {
	int	  i;
	MwListBox lb = handle->internal;
	int	  old;
	int	  max = 0;
	if(index == -1) index = arrlen(lb->list);
	old = index;

	for(i = 0; i < arrlen(packet->names); i++) {
		if(arrlen(packet->names[i]) > max) max = arrlen(packet->names[i]);
	}

	for(i = 0; i < arrlen(packet->names); i++) {
		MwListBoxEntry entry;
		char*	       name;
		int	       j;

		entry.name = NULL;
		for(j = 0; j < max; j++) {
			if(arrlen(packet->names[i]) > j && packet->names[i][j] != NULL) {
				name = MwStringDuplicate(packet->names[i][j]);
				arrput(entry.name, name);
			} else {
				arrput(entry.name, NULL);
			}
		}

		entry.pixmap = packet->pixmaps[i];

		arrins(lb->list, index, entry);
		index++;
	}

	resize(handle);
	if(old < (MwGetInteger(lb->vscroll, MwNvalue) + MwGetInteger(lb->vscroll, MwNareaShown))) {
		MwForceRender(lb->frame);
	}
}

static void mwListBoxDeleteImpl(MwWidget handle, int index) {
	MwListBox lb = handle->internal;
	int	  i;

	if(index == -1) index = arrlen(lb->list) - 1;
	for(i = 0; i < arrlen(lb->list[index].name); i++) {
		free(lb->list[index].name[i]);
	}
	arrfree(lb->list[index].name);
	arrdel(lb->list, index);

	if(MwGetInteger(handle, MwNvalue) >= arrlen(lb->list)) {
		MwSetInteger(handle, MwNvalue, arrlen(lb->list) - 1);
	}
	if(MwGetInteger(handle, MwNvalue) < 0) {
		MwSetInteger(handle, MwNvalue, -1);
	}

	resize(handle);
	if(index < (MwGetInteger(lb->vscroll, MwNvalue) + MwGetInteger(lb->vscroll, MwNareaShown))) {
		MwForceRender(lb->frame);
	}
}

static void mwListBoxResetImpl(MwWidget handle) {
	MwListBox lb = handle->internal;
	int	  i;

	while(arrlen(lb->list) > 0) {
		for(i = 0; i < arrlen(lb->list[0].name); i++) {
			if(lb->list[0].name[i] != NULL) free(lb->list[0].name[i]);
		}
		arrfree(lb->list[0].name);
		arrdel(lb->list, 0);
	}

	MwSetInteger(handle, MwNvalue, -1);

	MwSetInteger(lb->vscroll, MwNvalue, 0);

	resize(handle);
	MwForceRender(lb->frame);
}

static const char* mwListBoxGetImpl(MwWidget handle, int index) {
	MwListBox lb = handle->internal;

	if(index < 0) return NULL;
	if(index >= arrlen(lb->list)) return NULL;

	return lb->list[index].name[0];
}

static void mwListBoxSetWidthImpl(MwWidget handle, int index, int width) {
	MwListBox lb = handle->internal;

	while(((index + 1) - arrlen(lb->width)) > 0) arrput(lb->width, 0);

	lb->width[index] = width;

	MwForceRender(handle);
	MwForceRender(lb->frame);
}

static void mwListBoxSetAlignmentImpl(MwWidget handle, int index, int alignment) {
	MwListBox lb = handle->internal;

	while(((index + 1) - arrlen(lb->alignment)) > 0) arrput(lb->alignment, MwALIGNMENT_BEGINNING);

	lb->alignment[index] = alignment;

	MwForceRender(handle);
	MwForceRender(lb->frame);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwListBoxDelete") == 0) {
		int index = va_arg(va, int);
		mwListBoxDeleteImpl(handle, index);
	}
	if(strcmp(name, "mwListBoxReset") == 0) {
		mwListBoxResetImpl(handle);
	}
	if(strcmp(name, "mwListBoxGet") == 0) {
		int index	   = va_arg(va, int);
		*(const char**)out = mwListBoxGetImpl(handle, index);
	}
	if(strcmp(name, "mwListBoxSetWidth") == 0) {
		int index = va_arg(va, int);
		int width = va_arg(va, int);
		mwListBoxSetWidthImpl(handle, index, width);
	}
	if(strcmp(name, "mwListBoxSetAlignment") == 0) {
		int index     = va_arg(va, int);
		int alignment = va_arg(va, int);
		mwListBoxSetAlignmentImpl(handle, index, alignment);
	}
	if(strcmp(name, "mwListBoxInsert") == 0) {
		int		 index	= va_arg(va, int);
		MwListBoxPacket* packet = va_arg(va, MwListBoxPacket*);
		mwListBoxInsertImpl(handle, index, packet);
	}
}

static void tick(MwWidget handle) {
	MwListBox lb = handle->internal;

	if(lb->changed) {
		lb->changed = 0;
		MwForceRender(lb->frame);
	}
}

MwClassRec MwListBoxClassRec = {
    create,	  /* create */
    destroy,	  /* destroy */
    draw,	  /* draw */
    NULL,	  /* click */
    NULL,	  /* parent_resize */
    prop_change,  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* execute */
    tick,	  /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwListBoxClass = &MwListBoxClassRec;
