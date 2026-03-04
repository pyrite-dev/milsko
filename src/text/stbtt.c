#ifdef USE_STB_TRUETYPE
#include <stdlib.h>
#include <Mw/Milsko.h>

#include "../../external/stb_truetype.h"

struct _MwFLFont {
	stbtt_fontinfo font;
	void*	       data;
	float	       scale;
	int	       ascent;
	int	       descent;
};

static int stbtt_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	MwFLFont       ttf = MwGetVoid(handle, bold ? MwNboldFont : MwNfont);
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

static int stbtt_MwTextWidth(MwWidget handle, const char* text) {
	MwFLFont ttf = MwGetVoid(handle, MwNfont);
	int	 ax, lsb;
	int	 tw = 0;
	if(ttf == NULL) return -1;

	while(text[0] != 0) {
		int c;
		text += MwUTF8ToUTF32(text, &c);

		stbtt_GetCodepointHMetrics(&ttf->font, c, &ax, &lsb);

		tw += ax * ttf->scale;
	}

	return tw;
}

static int stbtt_MwTextHeight(MwWidget handle, int count) {
	MwFLFont ttf = MwGetVoid(handle, MwNfont);
	if(ttf == NULL) return -1;

	return (ttf->ascent - ttf->descent) * ttf->scale * count;
}

static void* stbtt_MwFontLoad(unsigned char* data, unsigned int size) {
	MwFLFont ttf = malloc(sizeof(*ttf));
	ttf->data    = malloc(size);
	memcpy(ttf->data, data, size);
	stbtt_InitFont(&ttf->font, ttf->data, 0);

	ttf->scale = stbtt_ScaleForPixelHeight(&ttf->font, 16);
	stbtt_GetFontVMetrics(&ttf->font, &ttf->ascent, &ttf->descent, 0);

	return ttf;
}

static void stbtt_MwFontFree(void* handle) {
	MwFLFont ttf = handle;

	free(ttf->data);
	free(ttf);
}

int MwFL_STBTTSetup() {
	MwFLDrawText   = stbtt_MwDrawText;
	MwFLTextWidth  = stbtt_MwTextWidth;
	MwFLTextHeight = stbtt_MwTextHeight;
	MwFLFontLoad   = stbtt_MwFontLoad;
	MwFLFontFree   = stbtt_MwFontFree;
	return 0;
}

#endif
