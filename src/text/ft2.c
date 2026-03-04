#ifdef USE_FREETYPE2
#include <Mw/Milsko.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct _MwFLFont {
	FT_Library library;
	FT_Face	   face;
	void*	   data;
};
static int ft2_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	MwFLFont       ttf = MwGetVoid(handle, bold ? MwNboldFont : MwNfont);
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

static int ft2_MwTextWidth(MwWidget handle, const char* text) {
	MwFLFont ttf = MwGetVoid(handle, MwNfont);
	int	 tw = 0, mtw = 0;
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

static int ft2_MwTextHeight(MwWidget handle, int count) {
	MwFLFont ttf = MwGetVoid(handle, MwNfont);
	if(ttf == NULL) return -1;

	return (ttf->face->height * 14 / ttf->face->units_per_EM) * count;
}

static void* ft2_MwFontLoad(unsigned char* data, unsigned int size) {
	MwFLFont ttf = malloc(sizeof(*ttf));
	ttf->data    = malloc(size);
	memcpy(ttf->data, data, size);
	FT_Init_FreeType(&ttf->library);
	FT_New_Memory_Face(ttf->library, ttf->data, size, 0, &ttf->face);

	FT_Set_Pixel_Sizes(ttf->face, 0, 14);

	return ttf;
}

static void ft2_MwFontFree(void* handle) {
	MwFLFont ttf = handle;

	FT_Done_Face(ttf->face);
	FT_Done_FreeType(ttf->library);

	free(ttf->data);
	free(ttf);
}

int MWFL_FT2Setup() {
	MwFLDrawText   = ft2_MwDrawText;
	MwFLTextWidth  = ft2_MwTextWidth;
	MwFLTextHeight = ft2_MwTextHeight;
	MwFLFontLoad   = ft2_MwFontLoad;
	MwFLFontFree   = ft2_MwFontFree;
	return 0;
}
#endif
