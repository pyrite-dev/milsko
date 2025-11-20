/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

#define OpenerSize 10
#define LineSpace 24

static void vscroll_changed(MwWidget handle, void* user, void* call) {
	MwTreeView tv = handle->parent->internal;

	(void)user;
	(void)call;

	tv->changed = 1;
}

static void recursion(MwWidget handle, MwTreeViewEntry* tree, MwLLColor base, MwLLColor text, MwPoint* p, int next, int shift, int* skip, int* shared, int draw, MwPoint* mouse) {
	int	i;
	MwPoint l[2];
	int	skipped = 0;
	if((*skip) > 0) {
		(*skip)--;
		skipped = 1;
		if(p->y == MwDefaultBorderWidth(handle)) p->y -= MwTextHeight(handle, "M");
	} else if((*shared) < (MwGetInteger(handle, MwNheight) / MwTextHeight(handle, "M") + 2)) {
		MwRect r;
		p->x += shift;
		p->y += MwTextHeight(handle, "M") / 2;

		if(shift > 0) {
			l[0] = *p;
			l[0].x -= LineSpace / 2;
			l[1] = *p;
			if(draw) MwLLLine(handle->lowlevel, &l[0], text);

			l[0] = *p;
			l[0].x -= LineSpace / 2;
			l[1] = l[0];
			l[0].y -= MwTextHeight(handle, "M") / 2;
			if(next) {
				l[1].y += MwTextHeight(handle, "M") / 2;
			}
			if(draw) MwLLLine(handle->lowlevel, &l[0], text);
		}
		if(tree->tree != NULL) {
			r.width	 = OpenerSize;
			r.height = OpenerSize;
			r.x	 = p->x - LineSpace + (LineSpace - r.width) / 2;
			r.y	 = p->y - MwTextHeight(handle, "M") / 2 + (MwTextHeight(handle, "M") - r.height) / 2;
			if(draw) {
				MwLLColor col = tree->opened ? MwLightenColor(handle, base, -8, -8, -8) : base;
				MwRect r2 = r;
				const int len = 4;

				MwDrawRect(handle, &r, col);
				MwDrawFrame(handle, &r, base, tree->opened);
				if(tree->opened){
					l[0].x = r.x + (r.width - len) / 2;
					l[0].y = r.y + r.height / 2;

					l[1].x = l[0].x + len;
					l[1].y = l[0].y;

					MwLLLine(handle->lowlevel, &l[0], text);
				}else{
					l[0].x = r.x + (r.width - len) / 2;
					l[0].y = r.y + r.height / 2;

					l[1].x = l[0].x + len;
					l[1].y = l[0].y;

					MwLLLine(handle->lowlevel, &l[0], text);

					l[0].x = r.x + r.width / 2;
					l[0].y = r.y + (r.height - len) / 2;

					l[1].x = l[0].x;
					l[1].y = l[0].y + len;

					MwLLLine(handle->lowlevel, &l[0], text);
				}
				r = r2;
				MwDrawFrame(handle, &r, base, tree->opened);

				if(col != base) MwLLFreeColor(col);
			} else if(shift != 0) {
				if(r.x <= mouse->x && mouse->x <= (r.x + r.width) && r.y <= mouse->y && mouse->y <= (r.y + r.height)) {
					tree->opened = tree->opened ? 0 : 1;
				}
			}
		}
		if(tree->pixmap != NULL) {
			r.height = MwTextHeight(handle, "M");
			r.width	 = r.height * tree->pixmap->common.width / tree->pixmap->common.height;

			r.x	 = p->x;
			r.y	 = p->y - MwTextHeight(handle, "M") / 2;

			if(draw) MwLLDrawPixmap(handle->lowlevel, &r, tree->pixmap);
		}
		p->x += MwGetInteger(handle->parent, MwNleftPadding);
		if(draw){
			if(tree->selected){
				r.x = p->x;
				r.y = p->y - MwTextHeight(handle, "M") / 2;
				r.width = MwTextWidth(handle, tree->label);
				r.height = MwTextHeight(handle, "M");
				MwDrawRect(handle, &r, text);
			}
			MwDrawText(handle, p, tree->label, 0, MwALIGNMENT_BEGINNING, tree->selected ? base : text);
		}
		p->x -= MwGetInteger(handle->parent, MwNleftPadding);

		p->y += MwTextHeight(handle, "M") / 2;
		p->x -= shift;

		(*shared)++;
	}
	if(!tree->opened) return;
	for(i = 0; i < arrlen(tree->tree); i++) {
		l[0] = *p;
		l[0].x += shift + LineSpace / 2;
		l[0].y += MwTextHeight(handle, "M") - (MwTextHeight(handle, "M") - OpenerSize) / 2;

		recursion(handle, &tree->tree[i], base, text, p, i != (arrlen(tree->tree) - 1) ? 1 : 0, shift + LineSpace, skip, shared, draw, mouse);

		l[1] = *p;
		l[1].x += shift + LineSpace / 2;

		if(skipped && p->y > l[0].y) {
			skipped = 0;
		}
		if(draw && !skipped && i != (arrlen(tree->tree) - 1)) MwLLLine(handle->lowlevel, &l[0], text);
	}
}

static void frame_draw(MwWidget handle) {
	MwRect	   r;
	MwTreeView tv	= handle->parent->internal;
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor  text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwPoint	   p;
	int	   shared = 0;
	int	   i;
	int	   skip = MwGetInteger(tv->vscroll, MwNvalue) * (MwGetInteger(tv->vscroll, MwNmaxValue) - MwGetInteger(tv->vscroll, MwNareaShown)) / MwGetInteger(tv->vscroll, MwNmaxValue);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	p.x = MwDefaultBorderWidth(handle);
	p.y = MwDefaultBorderWidth(handle);

	for(i = 0; i < arrlen(tv->tree); i++) {
		if(shared > (r.height / MwTextHeight(handle, "M"))) break;
		recursion(handle, &tv->tree[i], base, text, &p, 0, 0, &skip, &shared, 1, NULL);
	}

	MwDrawFrame(handle, &r, base, 1);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static int recursive_length(MwTreeViewEntry* e) {
	int l = 0;
	int i;
	for(i = 0; i < arrlen(e); i++) {
		if(e[i].opened && e[i].tree != NULL) l += recursive_length(e[i].tree);
		l++;
	}
	return l;
}

static void frame_mouse_down(MwWidget handle, void* user, void* call) {
	MwTreeView tv	 = handle->parent->internal;
	MwLLMouse* mouse = call;

	(void)user;

	if(mouse->button == MwLLMouseLeft) tv->pressed = mouse->point;
}

static void resize(MwWidget handle);

static void frame_mouse_up(MwWidget handle, void* user, void* call) {
	MwTreeView tv	  = handle->parent->internal;
	int	   shared = 0;
	int	   skip	  = MwGetInteger(tv->vscroll, MwNvalue) * (MwGetInteger(tv->vscroll, MwNmaxValue) - MwGetInteger(tv->vscroll, MwNareaShown)) / MwGetInteger(tv->vscroll, MwNmaxValue);
	MwPoint	   p;
	MwLLMouse* mouse = call;

	(void)user;

	if(mouse->button == MwLLMouseLeft) {
		int i;
		p.x = MwDefaultBorderWidth(tv->frame);
		p.y = MwDefaultBorderWidth(tv->frame);
		for(i = 0; i < arrlen(tv->tree); i++) {
			if(shared > (MwGetInteger(tv->frame, MwNheight) / MwTextHeight(tv->frame, "M"))) break;
			recursion(tv->frame, &tv->tree[i], NULL, NULL, &p, 0, 0, &skip, &shared, 0, &tv->pressed);
		}
		resize(handle->parent);
	}
}

static void resize(MwWidget handle) {
	MwTreeView tv = handle->internal;
	int	   w  = MwGetInteger(handle, MwNwidth);
	int	   h  = MwGetInteger(handle, MwNheight);
	int	   ih = 0;

	if(tv->vscroll == NULL) {
		tv->vscroll = MwCreateWidget(MwScrollBarClass, "vscroll", handle, w - 16, 0, 16, h);
		MwAddUserHandler(tv->vscroll, MwNchangedHandler, vscroll_changed, NULL);
		MwSetInteger(tv->vscroll, MwNvalue, 5);
	} else {
		MwVaApply(tv->vscroll,
			  MwNx, w - 16,
			  MwNy, 0,
			  MwNwidth, 16,
			  MwNheight, h,
			  NULL);
	}

	if(tv->frame == NULL) {
		tv->frame	       = MwVaCreateWidget(MwFrameClass, "frame", handle, 0, 0, w - 16, h,
							  MwNhasBorder, 1,
							  MwNinverted, 1,
							  NULL);
		tv->frame->draw_inject = frame_draw;
		MwAddUserHandler(tv->frame, MwNmouseDownHandler, frame_mouse_down, NULL);
		MwAddUserHandler(tv->frame, MwNmouseUpHandler, frame_mouse_up, NULL);
	} else {
		MwVaApply(tv->frame,
			  MwNx, 0,
			  MwNy, 0,
			  MwNwidth, w - 16,
			  MwNheight, h,
			  NULL);
	}

	ih = recursive_length(tv->tree);
	if(ih == 0) ih = 1;

	MwVaApply(tv->vscroll,
		  MwNareaShown, h / MwTextHeight(handle, "M"),
		  MwNmaxValue, ih,
		  NULL);

	if(ih <= (h / MwTextHeight(handle, "M"))) {
		MwLLShow(tv->vscroll->lowlevel, 0);
		MwSetInteger(tv->frame, MwNwidth, w);
	} else {
		MwLLShow(tv->vscroll->lowlevel, 1);
	}
}

static int create(MwWidget handle) {
	MwTreeView	tv = malloc(sizeof(*tv));
	MwTreeViewEntry e, e2, e3;
	memset(tv, 0, sizeof(*tv));

	handle->internal = tv;

	MwSetDefault(handle);

	e3.label = MwStringDupliacte("hello");
	e3.pixmap = NULL;
	e3.tree = NULL;
	e3.opened = 1;
	e3.selected = 0;

	e2.label = MwStringDupliacte("hello");
	e2.pixmap = NULL;
	e2.tree = NULL;
	e2.opened = 1;
	e2.selected = 0;
	arrput(e2.tree, e3);

	e.label = MwStringDupliacte("hello");
	e.pixmap = NULL;
	e.tree = NULL;
	e.opened = 1;
	e.selected = 1;
	arrput(e.tree, e2);
	arrput(tv->tree, e);

	MwSetInteger(handle, MwNsingleClickSelectable, 0);
	MwSetInteger(handle, MwNleftPadding, 0);

	resize(handle);
	tv->changed = 0;

	return 0;
}

static void destroy(MwWidget handle) {
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwLLColor c = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwRect	  r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, c);

	MwLLFreeColor(c);
}

static void prop_change(MwWidget handle, const char* prop) {
	if(strcmp(prop, MwNwidth) == 0 || strcmp(prop, MwNheight) == 0) resize(handle);
}

static void tick(MwWidget handle) {
	MwTreeView tv = handle->internal;

	if(tv->changed) {
		tv->changed = 0;
		MwForceRender(tv->frame);
	}
}

MwClassRec MwTreeViewClassRec = {
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
    NULL,	 /* execute */
    tick,	 /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwTreeViewClass = &MwTreeViewClassRec;
