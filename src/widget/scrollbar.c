/* $Id $*/
#include <Mw/Milsko.h>

typedef struct scrollbar {
	MwPoint point;
	int	drag;
	int	pos;
} scrollbar_t;

static int create(MwWidget handle) {
	scrollbar_t* scr = malloc(sizeof(*scr));

	handle->internal = scr;

	MwSetDefault(handle);
	MwVaApply(handle,
		  MwNminValue, 0,
		  MwNmaxValue, 100,
		  MwNvalue, 0,
		  MwNareaShown, 50,
		  MwNorientation, MwVERTICAL,
		  NULL);

	return 0;
}

static void destroy(MwWidget handle) {
	free(handle->internal);
}

static int calc_length(MwWidget handle) {
	int max	 = MwScrollBarGetVisibleLength(handle);
	int len	 = MwGetInteger(handle, MwNmaxValue) - MwGetInteger(handle, MwNminValue);
	int area = MwGetInteger(handle, MwNareaShown);

	return max * (double)area / len;
}

static int calc_positition(MwWidget handle) {
	int max = MwScrollBarGetVisibleLength(handle);
	int len = MwGetInteger(handle, MwNmaxValue) - MwGetInteger(handle, MwNminValue);
	int val = MwGetInteger(handle, MwNvalue);

	return (max - calc_length(handle)) * (double)val / len;
}

static void add_value(MwWidget handle, int mul) {
	int val = MwGetInteger(handle, MwNvalue);
	int min = MwGetInteger(handle, MwNminValue);
	int max = MwGetInteger(handle, MwNmaxValue);

	val += mul * MwGetInteger(handle, MwNareaShown);

	if(val < min) val = min;
	if(val > max) val = max;

	MwSetInteger(handle, MwNvalue, val);
}

static void draw(MwWidget handle) {
	MwRect	     r, rt, rbar;
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

	rt = r;

	or = MwGetInteger(handle, MwNorientation);
	if(or == MwVERTICAL) {
		rt.height = rt.width;

		rt.y = r.y;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.y <= uy) ? 1 : 0, MwNORTH);
		if(handle->pressed && scr->point.y <= uy) {
			add_value(handle, -1);
		}

		rbar.width  = r.width;
		rbar.height = calc_length(handle);
		rbar.x	    = r.x;
		rbar.y	    = r.y + rt.height + calc_positition(handle);

		rt.y = r.y + r.height - rt.height;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.y >= dy) ? 1 : 0, MwSOUTH);
		if(handle->pressed && scr->point.y >= dy) {
			add_value(handle, 1);
		}
	} else if(or == MwHORIZONTAL) {
		rt.width = rt.height;

		rt.x = r.x;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.x <= ux) ? 1 : 0, MwWEST);
		if(handle->pressed && scr->point.x <= ux) {
			add_value(handle, -1);
		}

		rbar.width  = calc_length(handle);
		rbar.height = r.height;
		rbar.x	    = r.x + rt.width + calc_positition(handle);
		rbar.y	    = r.y;

		rt.x = r.x + r.width - rt.width;
		MwDrawTriangle(handle, &rt, base, (handle->pressed && scr->point.x >= dx) ? 1 : 0, MwEAST);
		if(handle->pressed && scr->point.x >= dx) {
			add_value(handle, 1);
		}
	}

	MwDrawFrame(handle, &rbar, base, 0);
	MwDrawRect(handle, &rbar, base);

	MwLLFreeColor(dark);
	MwLLFreeColor(base);
}

static void mouse_move(MwWidget handle) {
	int	     or	 = MwGetInteger(handle, MwNorientation);
	scrollbar_t* scr = handle->internal;

	if(!handle->pressed) return;

	if(scr->drag) {
		int    l   = 0;
		double len = MwScrollBarGetVisibleLength(handle) - calc_length(handle);
		int    min = MwGetInteger(handle, MwNminValue);
		int    max = MwGetInteger(handle, MwNmaxValue);

		if(or == MwVERTICAL) {
			l = handle->mouse_point.y + scr->pos;
		} else if(or == MwHORIZONTAL) {
			l = handle->mouse_point.x + scr->pos;
		}

		len = l / len;
		if(len < 0) len = 0;
		if(len > 1) len = 1;
		MwSetInteger(handle, MwNvalue, (max - min) * len - min);

		MwForceRender(handle);
	}
}

static void mouse_down(MwWidget handle) {
	int	     ww	 = MwGetInteger(handle, MwNwidth);
	int	     wh	 = MwGetInteger(handle, MwNheight);
	int	     or	 = MwGetInteger(handle, MwNorientation);
	scrollbar_t* scr = handle->internal;

	scr->point = handle->mouse_point;
	scr->drag  = 0;
	if(or == MwVERTICAL) {
		int tri = (ww - MwDefaultBorderWidth * 2) + MwDefaultBorderWidth;
		if(tri <= scr->point.y && scr->point.y <= (wh - tri)) {
			scr->drag = 1;
			scr->pos  = calc_positition(handle) - scr->point.y;
		}
	} else if(or == MwHORIZONTAL) {
		int tri = (wh - MwDefaultBorderWidth * 2) + MwDefaultBorderWidth;
		if(tri <= scr->point.x && scr->point.x <= (ww - tri)) {
			scr->drag = 1;
			scr->pos  = calc_positition(handle) - scr->point.x;
		}
	}

	MwForceRender(handle);
}

MwClassRec MwScrollBarClassRec = {
    create,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    mouse_move,	   /* mouse_move */
    MwForceRender, /* mouse_up */
    mouse_down	   /* mouse_down */
};
MwClass MwScrollBarClass = &MwScrollBarClassRec;

int MwScrollBarGetVisibleLength(MwWidget handle) {
	int ww	= MwGetInteger(handle, MwNwidth);
	int wh	= MwGetInteger(handle, MwNheight);
	int or	= MwGetInteger(handle, MwNorientation);
	int tri = 0;
	int s	= 0;

	if(or == MwVERTICAL) {
		tri = (ww - MwDefaultBorderWidth * 2) * 2;
		s   = wh;
	} else if(or == MwHORIZONTAL) {
		tri = (wh - MwDefaultBorderWidth * 2) * 2;
		s   = ww;
	}
	return s - tri - MwDefaultBorderWidth * 2;
}
