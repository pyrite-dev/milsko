/* $Id $*/
#include <Mw/Milsko.h>

typedef struct scrollbar {
	MwPoint point;
	int	point_set;
} scrollbar_t;

static int create(MwWidget handle) {
	scrollbar_t* scr = malloc(sizeof(*scr));

	scr->point_set = 0;

	handle->internal = scr;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	     r, rt;
	MwLLColor    base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor    dark = MwLightenColor(handle, base, -64, -64, -64);
	scrollbar_t* scr  = handle->internal;
	int or ;
	int uy, dy, ux, dx;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	uy = r.width;
	dy = r.height - r.width;

	ux = r.height;
	dx = r.width - r.height;

	MwDrawFrame(handle, &r, dark, 1);
	MwDrawRect(handle, &r, dark);

	if(handle->pressed && !scr->point_set) {
		scr->point     = handle->mouse_point;
		scr->point_set = 1;
	} else if(!handle->pressed && scr->point_set) {
		scr->point_set = 0;
	}

	rt = r;

	if((or = MwGetInteger(handle, MwNorientation)) == -1 || or == MwVERTICAL) {
		rt.height = rt.width;

		rt.y = r.y;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.y <= uy) ? 1 : 0, MwNORTH);

		rt.y = r.y + r.height - rt.height;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.y >= dy) ? 1 : 0, MwSOUTH);
	} else if((or = MwGetInteger(handle, MwNorientation)) == -1 || or == MwHORIZONTAL) {
		rt.width = rt.height;

		rt.x = r.x;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.x <= ux) ? 1 : 0, MwWEST);

		rt.x = r.x + r.width - rt.width;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.x >= dx) ? 1 : 0, MwEAST);
	}

	MwLLFreeColor(dark);
	MwLLFreeColor(base);
}

MwClassRec MwScrollBarClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    NULL,	   /* mouse_move */
    MwForceRender, /* mouse_up */
    MwForceRender  /* mouse_down */
};
MwClass MwScrollBarClass = &MwScrollBarClassRec;
