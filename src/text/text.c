#include <Mw/Milsko.h>

int (*MwFLDrawText)(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) = NULL;
int (*MwFLTextWidth)(MwWidget handle, const char* text)							     = NULL;
int (*MwFLTextHeight)(MwWidget handle, int count)							     = NULL;
void* (*MwFLFontLoad)(unsigned char* data, unsigned int size)						     = NULL;
void (*MwFLFontFree)(void* handle)									     = NULL;

#if defined(USE_FREETYPE2) || defined(USE_STB_TRUETYPE)
#define TTF
#endif

#define FontWidth 7
#define FontHeight 14

static void bitmap_MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color);

void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color) {
	if(strlen(text) == 0) return;
#ifdef TTF
	if(MwFLDrawText)
		if(MwGetInteger(handle, MwNbitmapFont) || MwFLDrawText(handle, point, text, bold, align, color))
#endif
			bitmap_MwDrawText(handle, point, text, bold, align, color);
}

int MwTextWidth(MwWidget handle, const char* text) {
	/* TODO: check newline */
#ifdef TTF
	int st;

	if(MwFLTextWidth)
		if(!MwGetInteger(handle, MwNbitmapFont) && (st = MwFLTextWidth(handle, text)) != -1) return st;
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
	if(MwFLTextHeight)
		if(!MwGetInteger(handle, MwNbitmapFont) && (st = MwFLTextHeight(handle, c)) != -1) return st;
#endif
	return FontHeight * c;
}

void* MwFontLoad(unsigned char* data, unsigned int size) {
	if(MwFLFontLoad)
		return MwFLFontLoad(data, size);
	return NULL;
}

void MwFontFree(void* handle) {
	if(MwFLFontFree)
		return MwFLFontFree(handle);
}

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

typedef int (*call_t)(void);
void MwFLSetup(void) {
	call_t calls[] = {
#ifdef USE_FREETYPE2
	    MWFL_FT2Setup,
#endif
#ifdef USE_STB_TRUETYPE
	    MwFL_STBTTSetup,
#endif
	    NULL};
	int i;

	for(i = 0; calls[i] != NULL; i++) {
		if(calls[i]() == 0) return;
	}
}
