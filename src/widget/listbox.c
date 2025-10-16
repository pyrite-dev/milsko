/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int get_first_entry(MwListBox lb) {
	int st = 0;
	st     = MwGetInteger(lb->vscroll, MwNvalue);
	st     = st * (MwGetInteger(lb->vscroll, MwNmaxValue) - MwGetInteger(lb->vscroll, MwNareaShown)) / MwGetInteger(lb->vscroll, MwNmaxValue);
	if(st < 0) st = 0;

	return st;
}

static void vscroll_changed(MwWidget handle, void* user, void* call) {
	MwListBox lb = handle->parent->internal;
	MwForceRender(lb->frame);
}

static void frame_mouse_down(MwWidget handle, void* user, void* call) {
	MwListBox  lb = handle->parent->internal;
	MwLLMouse* m  = call;
	if(m->button == MwLLMouseLeft) {
		int st = 0;
		int i;
		int y = MwDefaultBorderWidth;
		int h = MwGetInteger(handle, MwNheight);

		st = get_first_entry(lb);
		for(i = 0; i < (h - MwDefaultBorderWidth * 2) / MwTextHeight(handle, "M"); i++) {
			if(y <= m->point.y && m->point.y <= (y + MwTextHeight(handle, "M"))) {
				unsigned long t;
				if(((t = MwLLGetTick()) - lb->click_time) < 250 && lb->selected == st + i) {
					MwDispatchUserHandler(handle->parent, MwNactivateHandler, &lb->selected);
				}
				lb->selected   = st + i;
				lb->click_time = t;
			}
			y += MwTextHeight(handle, "M");
		}

		MwForceRender(lb->frame);
		lb->pressed = 1;
	}
}

static void frame_mouse_up(MwWidget handle, void* user, void* call) {
	MwListBox  lb = handle->parent->internal;
	MwLLMouse* m  = call;
	if(m->button == MwLLMouseLeft) {
		lb->pressed = 0;
	}
}

static void frame_mouse_move(MwWidget handle, void* user, void* call) {
	MwListBox lb = handle->parent->internal;
	MwPoint*  p  = call;
	if(lb->pressed) {
		int st = 0;
		int i;
		int y = MwDefaultBorderWidth;
		int h = MwGetInteger(handle, MwNheight);

		st = get_first_entry(lb);
		for(i = 0; i < (h - MwDefaultBorderWidth * 2) / MwTextHeight(handle, "M"); i++) {
			if(y <= p->y && p->y <= (y + MwTextHeight(handle, "M"))) {
				lb->selected = st + i;
			}
			y += MwTextHeight(handle, "M");
		}

		MwForceRender(lb->frame);
	}
}

static void frame_draw(MwWidget handle) {
	MwRect	  r;
	MwListBox lb   = handle->parent->internal;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	int	  i;
	MwPoint	  p;
	int	  st = 0;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	p.x = MwDefaultBorderWidth + MwGetInteger(handle->parent, MwNleftPadding);
	p.y = MwDefaultBorderWidth;

	st = get_first_entry(lb);

	for(i = st; i < arrlen(lb->list) && i < st + (r.height - MwDefaultBorderWidth * 2) / MwTextHeight(handle, "M"); i++) {
		int selected = lb->selected == i ? 1 : 0;

		if(selected) {
			MwRect r2;
			r2.x	  = 0;
			r2.y	  = p.y;
			r2.width  = r.width;
			r2.height = MwTextHeight(handle, lb->list[i]);
			MwDrawRect(handle, &r2, text);
		}
		if(lb->pixmap_list[i] != NULL) {
			MwRect r2;
			int    h  = (lb->pixmap_list[i]->height > MwTextHeight(handle, "M")) ? MwTextHeight(handle, "M") : lb->pixmap_list[i]->height;
			r2.x	  = MwDefaultBorderWidth;
			r2.y	  = p.y + (MwTextHeight(handle, "M") - h) / 2;
			r2.width  = h * lb->pixmap_list[i]->width / lb->pixmap_list[i]->height;
			r2.height = h;
			MwLLDrawPixmap(handle->lowlevel, &r2, lb->pixmap_list[i]);
		}
		p.y += MwTextHeight(handle, lb->list[i]) / 2;
		MwDrawText(handle, &p, lb->list[i], 0, MwALIGNMENT_BEGINNING, selected ? base : text);
		p.y += MwTextHeight(handle, lb->list[i]) / 2;
	}

	MwDrawFrame(handle, &r, base, 1);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void resize(MwWidget handle) {
	MwListBox lb = handle->internal;
	int	  w  = MwGetInteger(handle, MwNwidth);
	int	  h  = MwGetInteger(handle, MwNheight);
	int	  ih;
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
	if(lb->frame == NULL) {
		lb->frame	       = MwVaCreateWidget(MwFrameClass, "frame", handle, 0, 0, w - 16, h, NULL);
		lb->frame->draw_inject = frame_draw;
		MwAddUserHandler(lb->frame, MwNmouseDownHandler, frame_mouse_down, NULL);
		MwAddUserHandler(lb->frame, MwNmouseUpHandler, frame_mouse_up, NULL);
		MwAddUserHandler(lb->frame, MwNmouseMoveHandler, frame_mouse_move, NULL);
	} else {
		MwVaApply(lb->frame,
			  MwNx, 0,
			  MwNy, 0,
			  MwNwidth, w - 16,
			  MwNheight, h,
			  NULL);
	}

	ih = arrlen(lb->list);
	if(ih == 0) ih = 1;

	MwVaApply(lb->vscroll,
		  MwNareaShown, h / MwTextHeight(handle, "M"),
		  MwNmaxValue, ih,
		  NULL);
}

static int create(MwWidget handle) {
	MwListBox lb = malloc(sizeof(*lb));
	memset(lb, 0, sizeof(*lb));

	handle->internal = lb;

	MwSetDefault(handle);

	resize(handle);
	lb->list	= NULL;
	lb->pixmap_list = NULL;
	lb->selected	= -1;
	lb->click_time	= 0;

	MwSetInteger(handle, MwNleftPadding, 0);

	return 0;
}

static void destroy(MwWidget handle) {
	MwListBox lb = handle->internal;
	arrfree(lb->list);
	arrfree(lb->pixmap_list);
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* prop) {
	if(strcmp(prop, MwNwidth) == 0 || strcmp(prop, MwNheight) == 0) resize(handle);
	if(strcmp(prop, MwNleftPadding) == 0) {
		MwListBox lb = handle->internal;
		MwForceRender(lb->frame);
	}
}

MwClassRec MwListBoxClassRec = {
    create,	 /* create */
    destroy,	 /* destroy */
    draw,	 /* draw */
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
    NULL,	 /* key */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwListBoxClass = &MwListBoxClassRec;

void MwListBoxInsert(MwWidget handle, int index, const char* text, MwLLPixmap pixmap) {
	char*	  str = malloc(strlen(text) + 1);
	MwListBox lb  = handle->internal;

	strcpy(str, text);

	if(index == -1) index = arrlen(lb->list);
	arrins(lb->list, index, str);
	arrins(lb->pixmap_list, index, pixmap);

	resize(handle);
	if(index < (MwGetInteger(lb->vscroll, MwNvalue) + MwGetInteger(lb->vscroll, MwNareaShown))) {
		MwForceRender(lb->frame);
	}
}

void MwListBoxInsertMultiple(MwWidget handle, int index, char** text, MwLLPixmap* pixmap, int count) {
	int	  i;
	MwListBox lb = handle->internal;
	int	  old;
	if(index == -1) index = arrlen(lb->list);
	old = index;

	for(i = 0; i < count; i++) {
		char* str = malloc(strlen(text[i]) + 1);
		strcpy(str, text[i]);

		arrins(lb->list, index, str);
		if(pixmap == NULL) {
			arrins(lb->pixmap_list, index, NULL);
		} else {
			arrins(lb->pixmap_list, index, pixmap[i]);
		}
		index++;
	}

	resize(handle);
	if(old < (MwGetInteger(lb->vscroll, MwNvalue) + MwGetInteger(lb->vscroll, MwNareaShown))) {
		MwForceRender(lb->frame);
	}
}

void MwListBoxDelete(MwWidget handle, int index) {
	MwListBox lb = handle->internal;

	if(index == -1) index = arrlen(lb->list) - 1;
	arrdel(lb->list, index);
	arrdel(lb->pixmap_list, index);

	if(lb->selected >= arrlen(lb->list)) {
		lb->selected = arrlen(lb->list) - 1;
	}
	if(lb->selected < 0) {
		lb->selected = -1;
	}

	resize(handle);
	if(index < (MwGetInteger(lb->vscroll, MwNvalue) + MwGetInteger(lb->vscroll, MwNareaShown))) {
		MwForceRender(lb->frame);
	}
}

const char* MwListBoxGet(MwWidget handle, int index) {
	MwListBox lb = handle->internal;

	if(index < 0) return NULL;
	if(index >= arrlen(lb->list)) return NULL;

	return lb->list[index];
}
