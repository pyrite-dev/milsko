/* $Id$ */
#include <Mw/Milsko.h>

#define FontWidth 7
#define FontHeight 14

static void bitmap_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	int	       i = 0, x, y, sx, sy;
	int	       tw;
	int	       th;
	unsigned char* px;
	MwRect	       r;
	MwLLPixmap     p;

	if(strlen(text) == 0) text = " ";
	tw = MwTextWidth(handle, text);
	th = MwTextHeight(handle, text);
	px = malloc(tw * th * 4);

	memset(px, 0, tw * th * 4);

	sx = 0;
	sy = 0;

	while(text[i] != 0) {
		int out;
		i += MwUTF8ToUTF32(text + i, &out);

		if(out >= 0x80) out = 0;

		if(out == '\n') {
			sx = 0;
			sy += FontHeight;
		} else {
			for(y = 0; y < FontHeight; y++) {
				for(x = 0; x < FontWidth; x++) {
					unsigned char* ppx = &px[((sy + y) * tw + sx + x) * 4];
					if((bold ? MwBoldFontData : MwFontData)[out].data[y] & (1 << ((FontWidth - 1) - x))) {
						ppx[0] = color->red;
						ppx[1] = color->green;
						ppx[2] = color->blue;
						ppx[3] = 255;
					} else {
						ppx[0] = 0;
						ppx[1] = 0;
						ppx[2] = 0;
						ppx[3] = 0;
					}
				}
			}
			sx += FontWidth;
		}
	}

	p	 = MwLoadRaw(handle, px, tw, th);
	r.x	 = point->x;
	r.y	 = point->y - th / 2;
	r.width	 = tw;
	r.height = th;

	if(align == MwALIGNMENT_CENTER) {
		r.x -= tw / 2;
	} else if(align == MwALIGNMENT_END) {
		r.x -= tw;
	}

	MwLLDrawPixmap(handle->lowlevel, &r, p);
	MwLLDestroyPixmap(p);
	free(px);
}

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	bitmap_MwDrawText(handle, point, text, bold, align, color);
}

int MwTextWidth(MwWidget handle, const char* text) {
	(void)handle;

	/* TODO: check newline */
	return strlen(text) * FontWidth;
}

int MwTextHeight(MwWidget handle, const char* text) {
	int c = 1;
	int i = 0;

	(void)handle;
	(void)text;

	while(text[i] != 0) {
		int out;
		i += MwUTF8ToUTF32(text + i, &out);

		if(out == '\n') c++;
	}
	return FontHeight * c;
}
