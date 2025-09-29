/* $Id$ */
#include <Mw/Milsko.h>

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
	MwPoint	  p[6];
	const int diff	  = 128;
	const int border  = 2;
	MwLLColor darker  = MwLLAllocColor(handle->lowlevel, color->red - diff, color->green - diff, color->blue - diff);
	MwLLColor lighter = MwLLAllocColor(handle->lowlevel, color->red + diff, color->green + diff, color->blue + diff);

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

	p[5].x = rect->x + rect->height;
	p[5].y = rect->y + rect->height;

	MwLLPolygon(handle->lowlevel, p, 6, invert ? lighter : darker);

	MwLLFreeColor(lighter);
	MwLLFreeColor(darker);

	rect->x += border;
	rect->y += border;
	rect->width -= border * 2;
	rect->height -= border * 2;
}

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, MwLLColor color) {
	int    i, x, y, sx, sy;
	MwRect r;

	sx = point->x;
	sy = point->y;

	for(i = 0; text[i] != 0; i++) {
		for(y = 0; y < 14; y++) {
			for(x = 0; x < 7; x++) {
				r.x	 = sx + x;
				r.y	 = sy + y;
				r.width	 = 1;
				r.height = 1;

				if(MwFontData[(unsigned char)text[i]].data[y] & (1 << (7 - x))) {
					MwDrawRect(handle, &r, color);
				}
			}
		}
		sx += 7;
	}
}
