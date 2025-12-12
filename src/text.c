#include <Mw/Milsko.h>

#if defined(USE_FREETYPE2)
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct ttf {
	FT_Library library;
	FT_Face	   face;
	void*	   data;
} ttf_t;

#define TTF
#elif defined(USE_STB_TRUETYPE)
#include "../external/stb_truetype.h"

typedef struct ttf {
	stbtt_fontinfo font;
	void*	       data;
	float	       scale;
	int	       ascent;
	int	       descent;
} ttf_t;

#define TTF
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

		if(out > 0xff) {
			out = 0;
		}

		if(out == '\n') {
			sx = 0;
			sy += FontHeight;
		} else {
			for(y = 0; y < FontHeight; y++) {
				for(x = 0; x < FontWidth; x++) {
					unsigned char* ppx = &px[((sy + y) * tw + sx + x) * 4];
					if((bold ? MwBoldFontData : MwFontData)[out].data[y] & (1 << ((FontWidth - 1) - x))) {
						ppx[0] = color->common.red;
						ppx[1] = color->common.green;
						ppx[2] = color->common.blue;
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

#if defined(USE_STB_TRUETYPE)
static int ttf_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	ttf_t*	       ttf = MwGetVoid(handle, bold ? MwNboldFont : MwNfont);
	unsigned char* px;
	int	       tw, th;
	MwRect	       r;
	MwLLPixmap     p;
	int	       ax, lsb;
	int	       x = 0, y = 0;
	if(ttf == NULL) return 1;

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
		if(c == '\n') {
			x = 0;
			y += (ttf->ascent - ttf->descent) * ttf->scale;
			continue;
		}

		stbtt_GetCodepointHMetrics(&ttf->font, c, &ax, &lsb);

		stbtt_GetCodepointBitmapBox(&ttf->font, c, ttf->scale, ttf->scale, &x0, &y0, &x1, &y1);
		ow  = x1 - x0;
		oh  = y1 - y0;
		out = malloc(ow * oh);
		stbtt_MakeCodepointBitmap(&ttf->font, out, ow, oh, ow, ttf->scale, ttf->scale, c);

		for(cy = 0; cy < oh; cy++) {
			for(cx = 0; cx < ow; cx++) {
				int	       ox  = x + (lsb * ttf->scale) + cx;
				int	       oy  = y + (ttf->ascent * ttf->scale) + y0 + cy;
				unsigned char* opx = &px[(oy * tw + ox) * 4];

				opx[0] = color->common.red;
				opx[1] = color->common.green;
				opx[2] = color->common.blue;
				opx[3] = out[cy * ow + cx];
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

	return 0;
}

static int ttf_MwTextWidth(MwWidget handle, const char* text) {
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

static int ttf_MwTextHeight(MwWidget handle, int count) {
	ttf_t* ttf = MwGetVoid(handle, MwNfont);
	if(ttf == NULL) return -1;

	return (ttf->ascent - ttf->descent) * ttf->scale * count;
}
#elif defined(USE_FREETYPE2)
static int ttf_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	ttf_t*	       ttf = MwGetVoid(handle, bold ? MwNboldFont : MwNfont);
	int	       tw, th;
	unsigned char* px;
	MwLLPixmap     p;
	MwRect	       r;
	int	       x = 0, y = 0;
	if(ttf == NULL) return 1;

	tw = MwTextWidth(handle, text);
	th = MwTextHeight(handle, text);
	px = malloc(tw * th * 4);

	memset(px, 0, tw * th * 4);
	while(text[0] != 0) {
		int	   c;
		FT_Bitmap* bmp;
		int	   cy, cx;
		int	   l = MwUTF8ToUTF32(text, &c);
		if(l <= 0) break;
		text += l;

		if(c == '\n') {
			x = 0;
			y += ttf->face->height * 14 / ttf->face->units_per_EM;
			continue;
		}

		FT_Load_Char(ttf->face, c, FT_LOAD_RENDER);
		bmp = &ttf->face->glyph->bitmap;

		for(cy = 0; cy < bmp->rows; cy++) {
			for(cx = 0; cx < bmp->width; cx++) {
				int	       ox  = x + cx + ttf->face->glyph->bitmap_left;
				int	       oy  = y + (ttf->face->height * 14 / ttf->face->units_per_EM) - ttf->face->glyph->bitmap_top + cy + (ttf->face->descender * 14 / ttf->face->units_per_EM);
				unsigned char* opx = &px[(oy * tw + ox) * 4];

				opx[0] = color->common.red;
				opx[1] = color->common.green;
				opx[2] = color->common.blue;
				opx[3] = bmp->buffer[cy * bmp->pitch + cx];
			}
		}

		x += ttf->face->glyph->metrics.horiAdvance / 64;
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

	return 0;
}

static int ttf_MwTextWidth(MwWidget handle, const char* text) {
	ttf_t* ttf = MwGetVoid(handle, MwNfont);
	int    tw = 0, mtw = 0;
	if(ttf == NULL) return -1;

	while(text[0] != 0) {
		int c;
		int l = MwUTF8ToUTF32(text, &c);
		if(l <= 0) break;
		text += l;
		if(c == '\n') {
			tw = 0;
			continue;
		}

		FT_Load_Char(ttf->face, c, FT_LOAD_RENDER);

		tw += ttf->face->glyph->metrics.horiAdvance / 64;
		if(tw > mtw) mtw = tw;
	}

	return mtw;
}

static int ttf_MwTextHeight(MwWidget handle, int count) {
	ttf_t* ttf = MwGetVoid(handle, MwNfont);
	if(ttf == NULL) return -1;

	return (ttf->face->height * 14 / ttf->face->units_per_EM) * count;
}
#endif

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	if(strlen(text) == 0) return;
#ifdef TTF
	if(ttf_MwDrawText(handle, point, text, bold, align, color))
#endif
		bitmap_MwDrawText(handle, point, text, bold, align, color);
}

int MwTextWidth(MwWidget handle, const char* text) {
	/* TODO: check newline */
#ifdef TTF
	int st;

	if((st = ttf_MwTextWidth(handle, text)) != -1) return st;
#else
	(void)handle;

#endif
	return MwUTF8Length(text) * FontWidth;
}

int MwTextHeight(MwWidget handle, const char* text) {
	int c = 1;
	int i = 0;
#ifdef TTF
	int st;
#endif

	(void)handle;
	(void)text;

	while(text[i] != 0) {
		int out;
		int l = MwUTF8ToUTF32(text + i, &out);
		if(l == 0) break;
		i += l;

		if(out == '\n') c++;
	}
#ifdef TTF
	if((st = ttf_MwTextHeight(handle, c)) != -1) return st;
#endif
	return FontHeight * c;
}

void* MwFontLoad(unsigned char* data, unsigned int size) {
#if defined(USE_FREETYPE2)
	ttf_t* ttf = malloc(sizeof(*ttf));
	ttf->data  = malloc(size);
	memcpy(ttf->data, data, size);
	FT_Init_FreeType(&ttf->library);
	FT_New_Memory_Face(ttf->library, ttf->data, size, 0, &ttf->face);

	FT_Set_Pixel_Sizes(ttf->face, 0, 14);

	return ttf;
#elif defined(USE_STB_TRUETYPE)
	ttf_t* ttf = malloc(sizeof(*ttf));
	ttf->data  = malloc(size);
	memcpy(ttf->data, data, size);
	stbtt_InitFont(&ttf->font, ttf->data, 0);

	ttf->scale = stbtt_ScaleForPixelHeight(&ttf->font, 16);
	stbtt_GetFontVMetrics(&ttf->font, &ttf->ascent, &ttf->descent, 0);

	return ttf;
#else
	(void)data;
	(void)size;
	return NULL;
#endif
}

void MwFontFree(void* handle) {
#if defined(USE_FREETYPE2)
	ttf_t* ttf = handle;

	FT_Done_Face(ttf->face);
	FT_Done_FreeType(ttf->library);

	free(ttf->data);
	free(ttf);
#elif defined(USE_STB_TRUETYPE)
	ttf_t* ttf = handle;

	free(ttf->data);
	free(ttf);
#else
	(void)handle;
#endif
}
