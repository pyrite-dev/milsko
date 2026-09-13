#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static const char* palette0[] = {
    "#800",
    "#080",
    "#880",
    "#008",
    "#808",
    "#088",
    "#888",
    "#f00",
    "#0f0",
    "#ff0",
    "#00f",
    "#f0f",
    "#0ff",
    "#fff",
    NULL};

static int wcreate(MwWidget handle) {
	MwChart c = malloc(sizeof(*c));
	memset(c, 0, sizeof(*c));

	handle->internal = c;

	MwSetDefault(handle);

	MwSetInteger(handle, MwNtype, MwCHART_BAR);

	return 0;
}

static void destroy(MwWidget handle) {
	MwChartReset(handle);
	free(handle->internal);
}

static void draw_line(MwWidget handle, const char* text, double x, double y, double width, double height, MwLLColor color) {
	MwPoint p[2];
	int	type = MwGetInteger(handle, MwNtype);

	p[0].x = x;
	p[0].y = y;

	p[1].x = x + width;
	p[1].y = p[0].y;
	MwDrawText(handle, NULL, p, text, MwALIGNMENT_END, color);

	if(type == MwCHART_3D_BAR) {
		MwPoint p2[2];

		p2[0] = p[0];
		p2[1] = p[0];

		p2[1].x += height;
		p2[1].y -= height;

		MwLLLine(handle->lowlevel, &p2[0], color);

		p[0].x += height;
		p[0].y -= height;
		p[1].y -= height;
	}
	MwLLLine(handle->lowlevel, &p[0], color);
}

static void draw(MwWidget handle) {
	MwLLColor    base   = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor    border = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	     r;
	MwChart	     c = handle->internal;
	int	     i;
	const char** colors = palette0;
	int	     n;
	int	     gap;
	int	     width;
	MwRect	     node;
	double	     vmin  = 0xffffffff;
	double	     vmax  = -0xffffffff;
	int	     space = MwTextHeight(handle, NULL, "M");
	MwPoint	     p[2];
	int	     type = MwGetInteger(handle, MwNtype);

	for(n = 0; colors[n] != NULL; n++);

	for(i = 0; i < arrlen(c->entries); i++) {
		if(vmin > c->entries[i].value) vmin = c->entries[i].value;
		if(vmax < c->entries[i].value) vmax = c->entries[i].value;
	}

	if(MwGetInteger(handle, MwNminValue) != MwDEFAULT) vmin = MwGetInteger(handle, MwNminValue);
	if(MwGetInteger(handle, MwNmaxValue) != MwDEFAULT) vmax = MwGetInteger(handle, MwNmaxValue);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	if(type == MwCHART_BAR || type == MwCHART_3D_BAR) {
		double s     = 1.5;
		double ovmin = vmin;
		double ovmax = vmax;
		double twidth;
		char   buf[128];

		node.x = 128;
		node.y = 0;

		width = (r.width - node.x) * s / (arrlen(c->entries) * (1 + s) + 1);
		gap   = width / s;

		twidth = (width + gap) * arrlen(c->entries) + gap;

		if(vmin > 0) {
			vmin = 0;
		} else if(vmin < 0) {
			vmin -= (double)width / r.height * (vmax - vmin);
		}

		if(vmax < 0) {
			vmax = 0;
		} else if(vmax > 0) {
			vmax += (double)width / r.height * (vmax - vmin);
		}

		sprintf(buf, "%.2f", ovmax);
		draw_line(handle, buf, node.x, (r.height - space) - (ovmax - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

		draw_line(handle, "0", node.x, (r.height - space) - (0 - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

		sprintf(buf, "%.2f", ovmin);
		draw_line(handle, buf, node.x, (r.height - space) - (ovmin - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

		p[0].x = node.x;
		p[0].y = 0;

		p[1].x = node.x;
		p[1].y = r.height - space;
		MwLLLine(handle->lowlevel, &p[0], border);

		p[0].x = node.x;
		p[0].y = r.height - space;

		p[1].x = node.x + twidth;
		p[1].y = p[0].y;
		MwLLLine(handle->lowlevel, &p[0], border);

		node.width = width;
		for(i = 0; i < arrlen(c->entries); i++) {
			int	  ColorDiff = MwGetColorDifference(handle);
			MwLLColor color	    = MwParseColor(handle, c->entries[i].color == NULL ? colors[i % n] : c->entries[i].color);
			MwLLColor colorl    = MwLightenColor(handle, color, ColorDiff, ColorDiff, ColorDiff);
			MwLLColor colord    = MwLightenColor(handle, color, -ColorDiff, -ColorDiff, -ColorDiff);
			MwPoint	  persp_top[5];
			MwPoint	  persp_right[5];

			node.x += gap;

			node.height = -c->entries[i].value / (vmax - vmin) * (r.height - space);
			node.y	    = (r.height - space) - (0 - vmin) / (vmax - vmin) * (r.height - space);

			MwDrawRect(handle, &node, color);

			if(type == MwCHART_3D_BAR) {
				MwRect node2 = node;

				MwFixRect(&node2);

				persp_top[0] = *(MwPoint*)&node2;
				persp_top[1] = *(MwPoint*)&node2;
				persp_top[1].x += node2.width;

				persp_top[2] = persp_top[1];
				persp_top[3] = persp_top[0];

				persp_top[2].x += node2.width / 2;
				persp_top[2].y -= node2.width / 2;

				persp_top[3].x += node2.width / 2;
				persp_top[3].y -= node2.width / 2;

				persp_top[4] = persp_top[0];

				MwLLPolygon(handle->lowlevel, persp_top, 4, colorl);

				persp_right[0] = *(MwPoint*)&node2;
				persp_right[1] = *(MwPoint*)&node2;
				persp_right[0].x += node2.width;
				persp_right[1].x += node2.width;
				persp_right[1].y += node2.height;

				persp_right[2] = persp_right[1];
				persp_right[3] = persp_right[0];

				persp_right[2].x += node2.width / 2;
				persp_right[2].y -= node2.width / 2;

				persp_right[3].x += node2.width / 2;
				persp_right[3].y -= node2.width / 2;

				persp_right[4] = persp_right[0];

				MwLLPolygon(handle->lowlevel, persp_right, 4, colord);
			}

			if(!MwGetInteger(handle, MwNmodernLook)) {
				MwDrawRectLine(handle, &node, border);

				if(type == MwCHART_3D_BAR) {
					MwLLLine(handle->lowlevel, &persp_top[0], border);
					MwLLLine(handle->lowlevel, &persp_top[1], border);
					MwLLLine(handle->lowlevel, &persp_top[2], border);
					MwLLLine(handle->lowlevel, &persp_top[3], border);

					MwLLLine(handle->lowlevel, &persp_right[0], border);
					MwLLLine(handle->lowlevel, &persp_right[1], border);
					MwLLLine(handle->lowlevel, &persp_right[2], border);
					MwLLLine(handle->lowlevel, &persp_right[3], border);
				}
			}

			p[0].x = node.x + node.width / 2;
			p[0].y = r.height - space / 2;

			MwDrawText(handle, NULL, p, c->entries[i].name, MwALIGNMENT_CENTER, border);

			node.x += node.width;

			MwLLFreeColor(colord);
			MwLLFreeColor(colorl);
			MwLLFreeColor(color);
		}
	}

	MwLLFreeColor(border);
	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtype) == 0) MwForceRender(handle);
}

static int mwChartAddImpl(MwWidget handle, int index, const char* text, double value, const char* color) {
	MwChart	     c = handle->internal;
	MwChartEntry e;

	if(index == -1) index = arrlen(c->entries);

	e.name	= MwStringDuplicate(text);
	e.color = color == NULL ? NULL : MwStringDuplicate(color);
	e.value = value;

	arrins(c->entries, index, e);

	MwForceRender(handle);

	return index;
}

static void mwChartDeleteImpl(MwWidget handle, int index) {
	MwChart c = handle->internal;

	if(index == -1) index = arrlen(c->entries) - 1;

	free(c->entries[index].name);
	if(c->entries[index].color != NULL) free(c->entries[index].color);
	arrdel(c->entries, index);

	MwForceRender(handle);
}

static void mwChartResetImpl(MwWidget handle) {
	MwChart c = handle->internal;
	int	i;

	for(i = 0; i < arrlen(c->entries); i++) {
		free(c->entries[i].name);
		if(c->entries[i].color != NULL) free(c->entries[i].color);
	}
	arrfree(c->entries);

	MwForceRender(handle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwChartAdd") == 0) {
		int	    index = va_arg(va, int);
		const char* text  = va_arg(va, const char*);
		double	    value = va_arg(va, double);
		const char* color = va_arg(va, const char*);
		*(int*)out	  = mwChartAddImpl(handle, index, text, value, color);
	}
	if(strcmp(name, "mwChartDelete") == 0) {
		int index = va_arg(va, int);
		mwChartDeleteImpl(handle, index);
	}
	if(strcmp(name, "mwChartReset") == 0) {
		mwChartResetImpl(handle);
	}
}

MwClassRec MwChartClassRec = {
    wcreate,	   /* create */
    destroy,	   /* destroy */
    draw,	   /* draw */
    NULL,	   /* click */
    NULL,	   /* parent_resize */
    prop_change,   /* prop_change */
    NULL,	   /* mouse_move */
    NULL,	   /* mouse_up */
    NULL,	   /* mouse_down */
    NULL,	   /* key */
    func_handler,  /* execute */
    NULL,	   /* tick */
    MwForceRender, /* resize */
    NULL,	   /* children_update */
    NULL,	   /* children_prop_change */
    NULL,	   /* clipboard */
    NULL,	   /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwChartClass = &MwChartClassRec;
