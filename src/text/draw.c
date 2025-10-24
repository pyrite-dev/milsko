/* $Id$ */
#include <Mw/Milsko.h>

#ifdef USE_STB_TRUETYPE
#include "../../external/stb_truetype.h"

typedef struct ttf {
	stbtt_fontinfo font;
	void*	       data;
	float	       scale;
	int	       ascent;
	int	       descent;
} ttf_t;
#endif

#define FontWidth 7
#define FontHeight 14

static void bitmap_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	int	       i = 0, x, y, sx, sy;
	int	       tw, th;
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

#ifdef USE_STB_TRUETYPE
static int ttf_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	ttf_t*	       ttf = MwGetVoid(handle, bold ? MwNboldFont : MwNfont);
	MwLLColor      base;
	unsigned char* px;
	int	       tw, th;
	MwRect	       r;
	MwLLPixmap     p;
	int	       ax, lsb;
	int	       x = 0;
	if(ttf == NULL) return 1;

	base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	tw = MwTextWidth(handle, text);
	th = MwTextHeight(handle, text);
	px = malloc(tw * th * 4);

	memset(px, 0, tw * th * 4);
	while(text[0] != 0) {
		int	       c;
		int	       x0, y0, x1, y1, cx, cy;
		int	       ow, oh;
		unsigned char* out;

		text += MwUTF8ToUTF32(text, &c);

		stbtt_GetCodepointHMetrics(&ttf->font, c, &ax, &lsb);

		stbtt_GetCodepointBitmapBox(&ttf->font, c, ttf->scale, ttf->scale, &x0, &y0, &x1, &y1);
		ow  = x1 - x0;
		oh  = y1 - y0;
		out = malloc(ow * oh);
		stbtt_MakeCodepointBitmap(&ttf->font, out, ow, oh, ow, ttf->scale, ttf->scale, c);

		for(cy = 0; cy < oh; cy++) {
			for(cx = 0; cx < ow; cx++) {
				int	       ox  = x + (lsb * ttf->scale) + cx;
				int	       oy  = (ttf->ascent * ttf->scale) + y0 + cy;
				unsigned char* opx = &px[(oy * tw + ox) * 4];
				double	       a   = out[cy * ow + cx];

				opx[0] = color->red;
				opx[1] = color->green;
				opx[2] = color->blue;
				opx[3] = a;
			}
		}

		x += ax * ttf->scale;

		free(out);
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

	MwLLFreeColor(base);

	return 0;
}

int ttf_MwTextWidth(MwWidget handle, const char* text) {
	ttf_t* ttf = MwGetVoid(handle, MwNfont);
	int    ax, lsb;
	int    tw = 0;
	if(ttf == NULL) return -1;

	while(text[0] != 0) {
		int c;
		text += MwUTF8ToUTF32(text, &c);

		stbtt_GetCodepointHMetrics(&ttf->font, c, &ax, &lsb);

		tw += ax * ttf->scale;
	}

	return tw;
}

int ttf_MwTextHeight(MwWidget handle, int count) {
	ttf_t* ttf = MwGetVoid(handle, MwNfont);
	if(ttf == NULL) return -1;

	return (ttf->ascent - ttf->descent) * ttf->scale * count;
}
#endif

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
#ifdef USE_STB_TRUETYPE
	if(ttf_MwDrawText(handle, point, text, bold, align, color))
#endif
		bitmap_MwDrawText(handle, point, text, bold, align, color);
}

int MwTextWidth(MwWidget handle, const char* text) {
	int st;

	/* TODO: check newline */
#ifdef USE_STB_TRUETYPE
	if((st = ttf_MwTextWidth(handle, text)) != -1) return st;
#else
	(void)handle;

#endif
	return strlen(text) * FontWidth;
}

int MwTextHeight(MwWidget handle, const char* text) {
	int c = 1;
	int i = 0;
	int st;

	(void)handle;
	(void)text;

	while(text[i] != 0) {
		int out;
		i += MwUTF8ToUTF32(text + i, &out);

		if(out == '\n') c++;
	}
#ifdef USE_STB_TRUETYPE
	if((st = ttf_MwTextHeight(handle, c)) != -1) return st;
#endif
	return FontHeight * c;
}

void* MwFontLoad(unsigned char* data, unsigned int size) {
#ifdef USE_STB_TRUETYPE
	ttf_t* ttf = malloc(sizeof(*ttf));
	ttf->data  = malloc(size);
	memcpy(ttf->data, data, size);
	stbtt_InitFont(&ttf->font, ttf->data, 0);

	ttf->scale = stbtt_ScaleForPixelHeight(&ttf->font, 16);
	stbtt_GetFontVMetrics(&ttf->font, &ttf->ascent, &ttf->descent, 0);

	return ttf;
#else
	return NULL;
#endif
}

void MwFontFree(void* handle) {
#ifdef USE_STB_TRUETYPE
	ttf_t* ttf = handle;

	free(ttf->data);
	free(ttf);
#else
	(void)handle;
#endif
}
