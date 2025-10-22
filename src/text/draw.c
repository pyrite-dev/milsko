/* $Id$ */
#include <Mw/Milsko.h>

#ifdef HAS_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#define bitmap_FontWidth 7
#define bitmap_FontHeight 14

#define ft_FontWidth 14
#define ft_FontHeight 14

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
			sy += bitmap_FontHeight;
		} else {
			for(y = 0; y < bitmap_FontHeight; y++) {
				for(x = 0; x < bitmap_FontWidth; x++) {
					unsigned char* ppx = &px[((sy + y) * tw + sx + x) * 4];
					if((bold ? MwBoldFontData : MwFontData)[out].data[y] & (1 << ((bitmap_FontWidth - 1) - x))) {
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
			sx += bitmap_FontWidth;
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

#ifdef HAS_FREETYPE
static void ft_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	FT_Error err;

	FT_Error (*_FT_Load_Char)(FT_Face  face,
				  FT_ULong char_code,
				  FT_Int32 load_flags);
	FT_Error (*_FT_Set_Pixel_Sizes)(FT_Face face,
					FT_UInt pixel_width,
					FT_UInt pixel_height);

	MwLL l = handle->lowlevel;

	int	       i = 0, x, y, sx, sy;
	int	       tw;
	int	       th;
	unsigned char* px;
	MwRect	       r;
	MwLLPixmap     p;

	if(strlen(text) == 0) text = " ";
	tw = MwTextWidth(handle, text);
	th = MwTextHeight(handle, text);

	// allocate 4x memory then what we need to account for spacing, italics, etc.
	px = malloc(tw * th * 16);
	memset(px, 0, tw * th * 16);

	sx = 0;
	sy = 0;

	FT_WITH_FUNC(l, FT_Set_Pixel_Sizes, {
		if((err = _FT_Set_Pixel_Sizes(l->ftFace, ft_FontWidth, ft_FontHeight)) != 0) {
			if(r.width == 0 || r.height == 0) {
				printf("failed to set pixel sizes");
				print_ft_error(l->ftLib, err);
				return;
			}
		}
	});

	FT_WITH_FUNC(l, FT_Load_Char, {
		while(text[i] != 0) {
			int out;
			i += MwUTF8ToUTF32(text + i, &out);

			if(out >= 0x80) out = 0;

			if((err = _FT_Load_Char(l->ftFace, out, FT_LOAD_RENDER)) != 0) {
				printf("[WARNING] Failed to render %c. ", out);
				print_ft_error(l->ftLib, err);
				continue;
			}

			FT_GlyphSlot glyph  = l->ftFace->glyph;
			FT_Bitmap    bitmap = glyph->bitmap;

			r.width	 = bitmap.width;
			r.height = bitmap.rows;

			if(sy == 0) {
				sy = r.height;
			}

			if(r.width == 0 || r.height == 0) {
				sx += ft_FontWidth;
				continue;
			}

			if(out == '\n') {
				sx = 0;
				sy += r.height;
			} else {
				// printf("%ld\n", (glyph->advance.x / r.width));
				int gsx = sx + glyph->bitmap_left;
				int gsy = (sy - glyph->bitmap_top);
				for(y = 0; y < r.height; y++) {
					for(x = 0; x < r.width; x++) {
						unsigned char* ppx  = &px[((gsy + y) * tw + gsx + x) * 4];
						unsigned char  gray = bitmap.buffer[(y * r.width) + x];
						ppx[0]		    = color->red - gray;
						ppx[1]		    = color->green - gray;
						ppx[2]		    = color->blue - gray;
						ppx[3]		    = gray;
					}
				}
				sx += r.width;
				// sx += glyph->advance.x;
			}
		}
	});

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

#endif

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
#ifdef HAS_FREETYPE
	if(handle->lowlevel->ftLib != NULL) {
		ft_MwDrawText(handle, point, text, bold, align, color);
	} else {
#endif
		bitmap_MwDrawText(handle, point, text, bold, align, color);
#ifdef HAS_FREETYPE
	}
#endif
}

int MwTextWidth(MwWidget handle, const char* text) {
	(void)handle;

/* TODO: check newline */
#ifdef HAS_FREETYPE
	if(handle->lowlevel->ftLib != NULL) {
		return strlen(text) * ft_FontWidth;
	} else {
#endif
		return strlen(text) * bitmap_FontWidth;
#ifdef HAS_FREETYPE
	}
#endif
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
#ifdef HAS_FREETYPE
	if(handle->lowlevel->ftLib != NULL) {
		return ft_FontHeight * c;
	} else {
#endif
		return bitmap_FontHeight * c;
#ifdef HAS_FREETYPE
	}
#endif
}
