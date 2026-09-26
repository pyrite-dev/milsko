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
    NULL};

#define NUMFMT "%.2f"
#define PIE_DIV 10

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

static void draw_line(MwWidget handle, const char* text, double x, double y, double width, double height, MwColor color) {
	MwPoint p[2];
	int	type = MwGetInteger(handle, MwNtype);

	p[0].x = x;
	p[0].y = y;

	p[1].x = x + width;
	p[1].y = p[0].y;
	MwDrawText(handle, NULL, p, text, MwALIGNMENT_END, color);

	if(type == MwCHART_BAR_3D) {
		MwPoint p2[2];

		p2[0] = p[0];
		p2[1] = p[0];

		p2[1].x += height;
		p2[1].y -= height;

		MwLine(handle, &p2[0], color);

		p[0].x += height;
		p[0].y -= height;
		p[1].y -= height;
	}
	MwLine(handle, &p[0], color);
}

#define CALC_VMIN_VMAX \
	if(vmin > 0) { \
		vmin = 0; \
	} else if(vmin < 0) { \
		vmin -= (double)width / r.height * (vmax - vmin); \
	} \
\
	if(vmax < 0) { \
		vmax = 0; \
	} else if(vmax > 0) { \
		vmax += (double)width / r.height * (vmax - vmin); \
	}

static void bar_chart(MwWidget handle, MwRect* _r, double vmin, double vmax, int space, const char** colors, int n, MwColor border) {
	int	ColorDiff = MwGetColorDifference(handle);
	MwChart c	  = handle->internal;
	double	s	  = 1.5;
	double	twidth;
	char	buf[128];
	MwRect	node;
	MwPoint p[2];
	int	i;
	int	width;
	int	gap;
	MwRect	r      = *_r;
	double	ovmin  = vmin;
	double	ovmax  = vmax;
	int	type   = MwGetInteger(handle, MwNtype);
	int	modern = MwGetInteger(handle, MwNmodernLook);

	node.x = 0;

	for(i = 0; i < arrlen(c->entries); i++) {
		int w;

		MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, c->entries[i].value);

		w = MwTextWidth(handle, NULL, buf);

		if(node.x < w) node.x = w;
	}

	node.y = 0;

	width = (r.width - node.x) * s / (arrlen(c->entries) * (1 + s) + 1);
	gap   = width / s;

	CALC_VMIN_VMAX;

	twidth = (width + gap) * arrlen(c->entries) + gap;

	MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, ovmax);
	draw_line(handle, buf, node.x, (r.height - space) - (ovmax - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	draw_line(handle, "0", node.x, (r.height - space) - (0 - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, ovmin);
	draw_line(handle, buf, node.x, (r.height - space) - (ovmin - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	p[0].x = node.x;
	p[0].y = 0;

	p[1].x = node.x;
	p[1].y = r.height - space;
	MwLine(handle, &p[0], border);

	p[0].x = node.x;
	p[0].y = r.height - space;

	p[1].x = node.x + twidth;
	p[1].y = p[0].y;
	MwLine(handle, &p[0], border);

	node.width = width;
	for(i = 0; i < arrlen(c->entries); i++) {
		MwColor color  = MwParseColor(handle, c->entries[i].color == NULL ? colors[i % n] : c->entries[i].color);
		MwColor colorl = MwLightenColor(handle, color, ColorDiff, ColorDiff, ColorDiff);
		MwColor colord = MwLightenColor(handle, color, -ColorDiff, -ColorDiff, -ColorDiff);
		MwPoint persp_top[5];
		MwPoint persp_right[5];

		node.x += gap;

		node.height = -c->entries[i].value / (vmax - vmin) * (r.height - space);
		node.y	    = (r.height - space) - (0 - vmin) / (vmax - vmin) * (r.height - space);

		MwDrawRect(handle, &node, color);

		if(type == MwCHART_BAR_3D) {
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

			MwPolygon(handle, persp_top, 4, colorl);

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

			MwPolygon(handle, persp_right, 4, colord);
		}

		if(!modern) {
			MwDrawRectLine(handle, &node, border);

			if(type == MwCHART_BAR_3D) {
				MwLine(handle, &persp_top[0], border);
				MwLine(handle, &persp_top[1], border);
				MwLine(handle, &persp_top[2], border);
				MwLine(handle, &persp_top[3], border);

				MwLine(handle, &persp_right[0], border);
				MwLine(handle, &persp_right[1], border);
				MwLine(handle, &persp_right[2], border);
				MwLine(handle, &persp_right[3], border);
			}
		}

		p[0].x = node.x + node.width / 2;
		p[0].y = r.height - space / 2;

		MwDrawText(handle, NULL, p, c->entries[i].name, MwALIGNMENT_CENTER, border);

		node.x += node.width;

		MwFreeColor(colord);
		MwFreeColor(colorl);
		MwFreeColor(color);
	}
}

static void pie_chart(MwWidget handle, MwRect* _r, const char** colors, int n, MwColor border) {
	int	ColorDiff = MwGetColorDifference(handle);
	MwChart c	  = handle->internal;
	double	sum	  = 0;
	MwRect	r	  = *_r;
	int	radius	  = r.width < r.height ? r.width : r.height;
	double	cangle	  = 0;
	int	k;
	int	type	= MwGetInteger(handle, MwNtype);
	int	modern	= MwGetInteger(handle, MwNmodernLook);
	double	vsquish = type == MwCHART_PIE_3D ? 2 : 1;
	int	i;
	MwPoint p[2];
	MwPoint p2[360 + 1 + 1];

	for(i = 0; i < arrlen(c->entries); i++) {
		if(c->entries[i].value > 0) sum += c->entries[i].value;
	}

	for(k = 0; k < 4; k++) {
		for(i = 0; i < arrlen(c->entries); i++) {
			MwColor color;
			MwColor colorl;
			double	angle = c->entries[i].value / sum * 360;
			int	div   = angle / PIE_DIV;
			int	j;
			int	count;

			if(c->entries[i].value <= 0) continue;
			if((k == 0 || k == 1) && type == MwCHART_PIE) continue;

			color  = MwParseColor(handle, c->entries[i].color == NULL ? colors[i % n] : c->entries[i].color);
			colorl = MwLightenColor(handle, color, ColorDiff, ColorDiff, ColorDiff);

			p2[0].x = r.width / 2;
			p2[0].y = r.height / 2;
			for(j = 0; j <= div; j++) {
				double a = angle / div * j;

				p2[div - j + 1].x = p2[0].x + cos((cangle + a - 90) / 180 * M_PI) * radius / 2;
				p2[div - j + 1].y = p2[0].y + sin((cangle + a - 90) / 180 * M_PI) * radius / 2 / vsquish;
			}

			count = j + 1;

			if((k == 0 || k == 1) && type == MwCHART_PIE_3D) {
				int	j;
				MwPoint p3[360 + 1 + 1];
				int	d = 0;

				for(d = 1; d <= 2; d++) {
					for(j = 0; j < count - 1; j++) {
						p3[3 + j] = p2[1 + j];
						p3[3 + j].y += radius / 8 / d;
					}

					p3[0] = p2[count - 1];
					p3[1] = p2[0];
					p3[2] = p2[1];

					if(k == 0) {
						MwPolygon(handle, p3, count + 2, color);
					} else if(d == 1 && k == 1) {
						for(j = 3; j < count + 1; j++) {
							if(!modern) MwLine(handle, &p3[j], border);
						}

						p2[0] = p2[1];
						p2[0].y += radius / 8;
						if(!modern) MwLine(handle, p2, border);
					}
				}
			}

			if(k == 2) {
				MwPolygon(handle, p2, count, (type == MwCHART_PIE_3D ? colorl : color));
			} else if(k == 3) {
				int j;

				p2[count] = p2[0];
				for(j = 0; j < count; j++) {
					if(!modern) MwLine(handle, &p2[j], border);
				}

				p2[0].x += cos((cangle + angle / 2 - 90) / 180 * M_PI) * radius / 4;
				p2[0].y += sin((cangle + angle / 2 - 90) / 180 * M_PI) * radius / 4 / vsquish;

				handle->bgcolor = type == MwCHART_PIE_3D ? colorl : color;

				MwDrawText(handle, NULL, p2, c->entries[i].name, MwALIGNMENT_CENTER, border);
			}

			cangle += angle;

			MwFreeColor(colorl);
			MwFreeColor(color);
		}
	}

	if(!modern && type == MwCHART_PIE_3D) {
		p[0].x = r.width / 2 - radius / 2;
		p[0].y = r.height / 2;
		p[1]   = p[0];
		p[1].y += radius / 8;
		MwLine(handle, p, border);

		p[0].x = p[1].x = r.width / 2 + radius / 2;
		MwLine(handle, p, border);
	}
}

static void line_chart(MwWidget handle, MwRect* _r, double vmin, double vmax, int space, MwColor border) {
	MwChart c = handle->internal;
	char	buf[128];
	double	twidth;
	double	x = 0;
	int	j;
	MwPoint p[2];
	MwRect	r     = *_r;
	double	ovmin = vmin;
	double	ovmax = vmax;
	int	i;
	int	width;

	for(i = 0; i < arrlen(c->entries); i++) {
		int w;

		MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, c->entries[i].value);

		w = MwTextWidth(handle, NULL, buf);

		if(x < w) x = w;
	}

	width  = 16;
	twidth = r.width - x;

	CALC_VMIN_VMAX;

	MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, ovmax);
	draw_line(handle, buf, x, (r.height - space) - (ovmax - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	draw_line(handle, "0", x, (r.height - space) - (0 - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	MwStringPrintIntoBuffer(buf, sizeof(buf), NUMFMT, ovmin);
	draw_line(handle, buf, x, (r.height - space) - (ovmin - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	draw_line(handle, "", x, (r.height - space) - (vmin - vmin) / (vmax - vmin) * (r.height - space), twidth, width / 2, border);

	p[0].x = x;
	p[0].y = 0;

	p[1]   = p[0];
	p[1].y = r.height - space;

	MwLine(handle, p, border);

	for(j = 0; j < 2; j++) {
		for(i = j == 0 ? 1 : 0; i < arrlen(c->entries); i++) {
			p[0].x = x + (r.width - x) / (arrlen(c->entries) + 1) * i;
			p[0].y = (r.height - space) - (c->entries[i - 1].value - vmin) / (vmax - vmin) * (r.height - space);
			p[1].x = x + (r.width - x) / (arrlen(c->entries) + 1) * (i + 1);
			p[1].y = (r.height - space) - (c->entries[i].value - vmin) / (vmax - vmin) * (r.height - space);

			if(j == 0) {
				MwLine(handle, p, border);
			} else {
				MwDrawText(handle, NULL, &p[1], c->entries[i].name, MwALIGNMENT_CENTER, border);
			}
		}
	}
}

static void draw(MwWidget handle) {
	MwColor	     base   = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwColor	     border = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwRect	     r;
	MwChart	     c = handle->internal;
	int	     i;
	const char** colors = palette0;
	int	     n;
	double	     vmin  = 0xffffffff;
	double	     vmax  = -0xffffffff;
	int	     space = MwTextHeight(handle, NULL, "M");
	int	     type  = MwGetInteger(handle, MwNtype);

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

	switch(type) {
	case MwCHART_BAR:
	case MwCHART_BAR_3D:
		bar_chart(handle, &r, vmin, vmax, space, colors, n, border);
		break;
	case MwCHART_PIE:
	case MwCHART_PIE_3D:
		pie_chart(handle, &r, colors, n, border);
		break;
	case MwCHART_LINE:
		line_chart(handle, &r, vmin, vmax, space, border);
		break;
	}

	MwFreeColor(border);
	MwFreeColor(base);
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
