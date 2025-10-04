/* $Id$ */
#include <Mw/Milsko.h>

#include "external/stb_image.h"

#define FontWidth 7
#define FontHeight 14
#define FontScale 1
#define ColorDiff 128
#define BorderWidth 2

static int hex(const char* txt, int len) {
	int i;
	int r = 0;
	for(i = 0; i < len; i++) {
		char c = txt[i];
		int  n = 0;
		if('a' <= c && c <= 'f') {
			n = c - 'a' + 10;
		} else if('A' <= c && c <= 'F') {
			n = c - 'A' + 10;
		} else if('0' <= c && c <= '9') {
			n = c - '0';
		}
		r = r << 4;
		r |= n;
	}
	return r;
}

MwLLColor MwParseColor(MwWidget handle, const char* text) {
	int r = 0;
	int g = 0;
	int b = 0;

	if(text[0] == '#' && strlen(text) == 4) {
		r = hex(text + 1, 1);
		g = hex(text + 2, 1);
		b = hex(text + 3, 1);

		r |= r << 4;
		g |= g << 4;
		b |= b << 4;
	} else if(text[0] == '#' && strlen(text) == 7) {
		r = hex(text + 1, 2);
		g = hex(text + 3, 2);
		b = hex(text + 5, 2);
	}

	return MwLLAllocColor(handle->lowlevel, r, g, b);
}

MwLLColor MwLightenColor(MwWidget handle, MwLLColor color, int r, int g, int b) {
	int cr = color->red + r;
	int cg = color->green + g;
	int cb = color->blue + b;

	if(cr < 0) cr = 0;
	if(cg < 0) cg = 0;
	if(cb < 0) cb = 0;
	if(cr > 255) cr = 255;
	if(cg > 255) cg = 255;
	if(cb > 255) cb = 255;

	return MwLLAllocColor(handle->lowlevel, cr, cg, cb);
}

void MwDrawRect(MwWidget handle, MwRect* rect, MwLLColor color) {
	MwPoint p[4];

	p[0].x = rect->x;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width;
	p[1].y = rect->y;

	p[2].x = rect->x + rect->width;
	p[2].y = rect->y + rect->height;

	p[3].x = rect->x;
	p[3].y = rect->y + rect->height;

	MwLLPolygon(handle->lowlevel, p, 4, color);
}

void MwDrawFrame(MwWidget handle, MwRect* rect, MwLLColor color, int invert) {
	MwDrawFrameEx(handle, rect, color, invert, BorderWidth);
}

void MwDrawFrameEx(MwWidget handle, MwRect* rect, MwLLColor color, int invert, int border) {
	MwPoint	  p[6];
	MwLLColor darker  = MwLightenColor(handle, color, -ColorDiff, -ColorDiff, -ColorDiff);
	MwLLColor lighter = MwLightenColor(handle, color, ColorDiff, ColorDiff, ColorDiff);

	p[0].x = rect->x;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width;
	p[1].y = rect->y;

	p[2].x = rect->x + rect->width - border;
	p[2].y = rect->y + border;

	p[3].x = rect->x + border;
	p[3].y = rect->y + border;

	p[4].x = rect->x + border;
	p[4].y = rect->y + rect->height - border;

	p[5].x = rect->x;
	p[5].y = rect->y + rect->height;

	MwLLPolygon(handle->lowlevel, p, 6, invert ? darker : lighter);

	p[0].x = rect->x + rect->width;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width - border;
	p[1].y = rect->y + border;

	p[2].x = rect->x + rect->width - border;
	p[2].y = rect->y + rect->height - border;

	p[3].x = rect->x + border;
	p[3].y = rect->y + rect->height - border;

	p[4].x = rect->x;
	p[4].y = rect->y + rect->height;

	p[5].x = rect->x + rect->width;
	p[5].y = rect->y + rect->height;

	MwLLPolygon(handle->lowlevel, p, 6, invert ? lighter : darker);

	MwLLFreeColor(lighter);
	MwLLFreeColor(darker);

	rect->x += border;
	rect->y += border;
	rect->width -= border * 2;
	rect->height -= border * 2;
}

void MwDrawTriangle(MwWidget handle, MwRect* rect, MwLLColor color, int invert, int direction) {
	MwPoint	  p1[4], p2[4], p3[4], p4[3];
	const int border  = BorderWidth * 2;
	MwLLColor darker  = MwLightenColor(handle, color, -ColorDiff, -ColorDiff, -ColorDiff);
	MwLLColor lighter = MwLightenColor(handle, color, ColorDiff, ColorDiff, ColorDiff);

	double deg = 30 + ((direction == MwEAST || direction == MwWEST) ? 30 : 0);
	double c   = cos(deg / 180 * M_PI);
	double s   = sin(deg / 180 * M_PI);

	if(direction == MwNORTH) {
		p1[0].x = rect->x + rect->width / 2;
		p1[0].y = rect->y;

		p1[1].x = rect->x;
		p1[1].y = rect->y + rect->height;

		p1[2].x = rect->x + c * border;
		p1[2].y = rect->y + rect->height - s * border;

		p1[3].x = rect->x + rect->width / 2;
		p1[3].y = rect->y + border;

		p2[0].x = rect->x + rect->width / 2;
		p2[0].y = rect->y;

		p2[1].x = rect->x + rect->width;
		p2[1].y = rect->y + rect->height;

		p2[2].x = rect->x + rect->width - c * border;
		p2[2].y = rect->y + rect->height - s * border;

		p2[3].x = rect->x + rect->width / 2;
		p2[3].y = rect->y + border;

		p3[0].x = rect->x + c * border;
		p3[0].y = rect->y + rect->height - s * border;

		p3[1].x = rect->x;
		p3[1].y = rect->y + rect->height;

		p3[2].x = rect->x + rect->width;
		p3[2].y = rect->y + rect->height;

		p3[3].x = rect->x + rect->width - c * border;
		p3[3].y = rect->y + rect->height - s * border;

		MwLLPolygon(handle->lowlevel, p1, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p2, 4, invert ? lighter : darker);
		MwLLPolygon(handle->lowlevel, p3, 4, invert ? lighter : darker);

		p4[0].x = rect->x + c * border;
		p4[0].y = rect->y + rect->height - s * border;

		p4[1].x = rect->x + rect->width - c * border;
		p4[1].y = rect->y + rect->height - s * border;

		p4[2].x = rect->x + rect->width / 2;
		p4[2].y = rect->y + border;
	} else if(direction == MwSOUTH) {
		p1[0].x = rect->x;
		p1[0].y = rect->y;

		p1[1].x = rect->x + c * border;
		p1[1].y = rect->y + s * border;

		p1[2].x = rect->x + rect->width - c * border;
		p1[2].y = rect->y + s * border;

		p1[3].x = rect->x + rect->width;
		p1[3].y = rect->y;

		p2[0].x = rect->x;
		p2[0].y = rect->y;

		p2[1].x = rect->x + c * border;
		p2[1].y = rect->y + s * border;

		p2[2].x = rect->x + rect->width / 2;
		p2[2].y = rect->y + rect->height - border;

		p2[3].x = rect->x + rect->width / 2;
		p2[3].y = rect->y + rect->height;

		p3[0].x = rect->x + rect->width;
		p3[0].y = rect->y;

		p3[1].x = rect->x + rect->width / 2;
		p3[1].y = rect->y + rect->height;

		p3[2].x = rect->x + rect->width / 2;
		p3[2].y = rect->y + rect->height - border;

		p3[3].x = rect->x + rect->width - c * border;
		p3[3].y = rect->y + s * border;

		MwLLPolygon(handle->lowlevel, p1, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p2, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p3, 4, invert ? lighter : darker);

		p4[0].x = rect->x + c * border;
		p4[0].y = rect->y + s * border;

		p4[1].x = rect->x + rect->width - c * border;
		p4[1].y = rect->y + s * border;

		p4[2].x = rect->x + rect->width / 2;
		p4[2].y = rect->y + rect->height - border;
	} else if(direction == MwEAST) {
		p1[0].x = rect->x;
		p1[0].y = rect->y;

		p1[1].x = rect->x + c * border;
		p1[1].y = rect->y + s * border;

		p1[2].x = rect->x + c * border;
		p1[2].y = rect->y + rect->height - s * border;

		p1[3].x = rect->x;
		p1[3].y = rect->y + rect->height;

		p2[0].x = rect->x;
		p2[0].y = rect->y;

		p2[1].x = rect->x + rect->width;
		p2[1].y = rect->y + rect->height / 2;

		p2[2].x = rect->x + rect->width - border;
		p2[2].y = rect->y + rect->height / 2;

		p2[3].x = rect->x + c * border;
		p2[3].y = rect->y + s * border;

		p3[0].x = rect->x + c * border;
		p3[0].y = rect->y + rect->height - s * border;

		p3[1].x = rect->x;
		p3[1].y = rect->y + rect->height;

		p3[2].x = rect->x + rect->width;
		p3[2].y = rect->y + rect->height / 2;

		p3[3].x = rect->x + rect->width - border;
		p3[3].y = rect->y + rect->height / 2;

		MwLLPolygon(handle->lowlevel, p1, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p2, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p3, 4, invert ? lighter : darker);

		p4[0].x = rect->x + rect->width - border;
		p4[0].y = rect->y + rect->height / 2;

		p4[1].x = rect->x + c * border;
		p4[1].y = rect->y + rect->height - s * border;

		p4[2].x = rect->x + c * border;
		p4[2].y = rect->y + s * border;
	} else if(direction == MwWEST) {
		p1[0].x = rect->x;
		p1[0].y = rect->y + rect->height / 2;

		p1[1].x = rect->x + border;
		p1[1].y = rect->y + rect->height / 2;

		p1[2].x = rect->x + rect->width - c * border;
		p1[2].y = rect->y + s * border;

		p1[3].x = rect->x + rect->width;
		p1[3].y = rect->y;

		p2[0].x = rect->x;
		p2[0].y = rect->y + rect->height / 2;

		p2[1].x = rect->x + border;
		p2[1].y = rect->y + rect->height / 2;

		p2[2].x = rect->x + rect->width - c * border;
		p2[2].y = rect->y + rect->height - s * border;

		p2[3].x = rect->x + rect->width;
		p2[3].y = rect->y + rect->height;

		p3[0].x = rect->x + rect->width;
		p3[0].y = rect->y;

		p3[1].x = rect->x + rect->width - c * border;
		p3[1].y = rect->y + s * border;

		p3[2].x = rect->x + rect->width - c * border;
		p3[2].y = rect->y + rect->height - s * border;

		p3[3].x = rect->x + rect->width;
		p3[3].y = rect->y + rect->height;

		MwLLPolygon(handle->lowlevel, p1, 4, invert ? darker : lighter);
		MwLLPolygon(handle->lowlevel, p2, 4, invert ? lighter : darker);
		MwLLPolygon(handle->lowlevel, p3, 4, invert ? lighter : darker);

		p4[0].x = rect->x + border;
		p4[0].y = rect->y + rect->height / 2;

		p4[1].x = rect->x + rect->width - c * border;
		p4[1].y = rect->y + rect->height - s * border;

		p4[2].x = rect->x + rect->width - c * border;
		p4[2].y = rect->y + s * border;
	}
	MwLLPolygon(handle->lowlevel, p4, 3, color);

	MwLLFreeColor(lighter);
	MwLLFreeColor(darker);
}

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, MwLLColor color) {
	int    i, x, y, sx, sy;
	MwRect r;

	sx = point->x - strlen(text) * FontWidth * FontScale / 2;
	sy = point->y - FontHeight * FontScale / 2;

	for(i = 0; text[i] != 0; i++) {
		for(y = 0; y < FontHeight; y++) {
			for(x = 0; x < FontWidth; x++) {
				r.x	 = sx + x * FontScale;
				r.y	 = sy + y * FontScale;
				r.width	 = FontScale;
				r.height = FontScale;

				if((bold ? MwBoldFontData : MwFontData)[(unsigned char)text[i]].data[y] & (1 << ((FontWidth - 1) - x))) {
					MwDrawRect(handle, &r, color);
				}
			}
		}
		sx += FontWidth * FontScale;
	}
}

int MwTextWidth(MwWidget handle, const char* text) {
	(void)handle;

	return strlen(text) * FontWidth * FontScale;
}

int MwTextHeight(MwWidget handle, const char* text) {
	(void)handle;
	(void)text;

	return FontHeight * FontScale;
}

MwLLPixmap MwLoadImage(MwWidget handle, const char* path) {
	int	       width, height, ch;
	unsigned char* rgb = stbi_load(path, &width, &height, &ch, 4);
	MwLLPixmap     px;

	if(rgb == NULL) return NULL;

	px = MwLLCreatePixmap(handle->lowlevel, rgb, width, height);

	free(rgb);

	return px;
}
