/* $Id$ */
#include <Mw/Milsko.h>

typedef struct viewport {
	MwWidget vscroll;
	MwWidget hscroll;
	MwWidget frame;
	MwWidget inframe;
} viewport_t;

static void vscroll_changed(MwWidget handle, void* user, void* call) {
	viewport_t* vp = user;
	int	    v  = MwGetInteger(handle, MwNvalue);
	int	    mv = MwGetInteger(handle, MwNmaxValue);
	int	    l  = MwGetInteger(vp->frame, MwNheight);
	v	       = (mv - l) * (double)v / mv;
	(void)call;
	if(v < 0) v = 0;
	MwVaApply(vp->inframe,
		  MwNy, -v,
		  NULL);
}

static void hscroll_changed(MwWidget handle, void* user, void* call) {
	viewport_t* vp = user;
	int	    v  = MwGetInteger(handle, MwNvalue);
	int	    mv = MwGetInteger(handle, MwNmaxValue);
	int	    l  = MwGetInteger(vp->frame, MwNwidth);
	v	       = (mv - l) * (double)v / mv;
	(void)call;
	if(v < 0) v = 0;
	MwVaApply(vp->inframe,
		  MwNx, -v,
		  NULL);
}

static void resize(MwWidget handle) {
	viewport_t* vp = handle->internal;
	int	    w  = MwGetInteger(handle, MwNwidth);
	int	    h  = MwGetInteger(handle, MwNheight);
	int	    iw;
	int	    ih;
	if(vp->vscroll == NULL) {
		vp->vscroll = MwVaCreateWidget(MwScrollBarClass, "vscroll", handle, w - 16, 0, 16, h - 16, NULL);
		MwAddUserHandler(vp->vscroll, MwNchangedHandler, vscroll_changed, vp);
	} else {
		MwVaApply(vp->vscroll,
			  MwNx, w - 16,
			  MwNy, 0,
			  MwNwidth, 16,
			  MwNheight, h - 16,
			  NULL);
	}
	if(vp->hscroll == NULL) {
		vp->hscroll = MwVaCreateWidget(MwScrollBarClass, "hscroll", handle, 0, h - 16, w - 16, 16, MwNorientation, MwHORIZONTAL, NULL);
		MwAddUserHandler(vp->hscroll, MwNchangedHandler, hscroll_changed, vp);
	} else {
		MwVaApply(vp->hscroll,
			  MwNx, 0,
			  MwNy, h - 16,
			  MwNwidth, w - 16,
			  MwNheight, 16,
			  NULL);
	}
	if(vp->frame == NULL) {
		vp->frame = MwCreateWidget(MwFrameClass, "frame", handle, 0, 0, w - 16, h - 16);
	} else {
		MwVaApply(vp->frame,
			  MwNx, 0,
			  MwNy, 0,
			  MwNwidth, w - 16,
			  MwNheight, h - 16,
			  NULL);
	}
	if(vp->inframe == NULL) {
		vp->inframe = MwCreateWidget(MwFrameClass, "inframe", vp->frame, 0, 0, w - 16, h - 16);
	}

	iw = MwGetInteger(vp->inframe, MwNwidth);
	ih = MwGetInteger(vp->inframe, MwNheight);

	MwVaApply(vp->vscroll,
		  MwNareaShown, h - 16,
		  MwNmaxValue, ih,
		  NULL);

	MwVaApply(vp->hscroll,
		  MwNareaShown, w - 16,
		  MwNmaxValue, iw,
		  NULL);
}

static int create(MwWidget handle) {
	viewport_t* vp = malloc(sizeof(*vp));
	memset(vp, 0, sizeof(*vp));

	handle->internal = vp;

	MwSetDefault(handle);

	resize(handle);

	return 0;
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
}

MwClassRec MwViewportClassRec = {
    create,	 /* create */
    NULL,	 /* destroy */
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
MwClass MwViewportClass = &MwViewportClassRec;

MwWidget MwViewportGetViewport(MwWidget handle) {
	viewport_t* vp = handle->internal;

	return vp->inframe;
}

void MwViewportSetSize(MwWidget handle, int w, int h) {
	viewport_t* vp = handle->internal;

	MwVaApply(vp->inframe,
		  MwNwidth, w,
		  MwNheight, h,
		  NULL);

	resize(handle);
}
