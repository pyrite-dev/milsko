/* $Id$ */
#include <Milsko/Milsko.h>

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

MilskoLLColor MilskoParseColor(MilskoWidget handle, const char* text) {
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

	return MilskoLLAllocColor(handle->lowlevel, r, g, b);
}

void MilskoDrawRect(MilskoWidget handle, MilskoRect* rect, MilskoLLColor color) {
	MilskoPoint p[4];

	p[0].x = rect->x;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width;
	p[1].y = rect->y;

	p[2].x = rect->x + rect->width;
	p[2].y = rect->y + rect->height;

	p[3].x = rect->x;
	p[3].y = rect->y + rect->height;

	MilskoLLPolygon(handle->lowlevel, p, 4, color);
}

void MilskoDrawFrame(MilskoWidget handle, MilskoRect* rect, MilskoLLColor color, int invert) {
	MilskoPoint   p[6];
	const int     diff    = 128;
	const int     border  = 2;
	MilskoLLColor darker  = MilskoLLAllocColor(handle->lowlevel, color->red - diff, color->green - diff, color->blue - diff);
	MilskoLLColor lighter = MilskoLLAllocColor(handle->lowlevel, color->red + diff, color->green + diff, color->blue + diff);

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

	MilskoLLPolygon(handle->lowlevel, p, 6, invert ? darker : lighter);

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

	MilskoLLPolygon(handle->lowlevel, p, 6, invert ? lighter : darker);

	MilskoLLFreeColor(lighter);
	MilskoLLFreeColor(darker);

	rect->x += border;
	rect->y += border;
	rect->width -= border * 2;
	rect->height -= border * 2;
}
