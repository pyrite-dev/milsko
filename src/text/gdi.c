#ifdef USE_GDI_TEXT
#include <Mw/Milsko.h>

struct _MwFLFont {
	HANDLE handle;
	HFONT  font;
	HDC    dc;
};

static int GDI_MwDrawText(MwWidget handle, MwFLFont ttf, MwPoint* point, const char* text, MwLLColor color) {
	BITMAPINFOHEADER bmih;
	int		 tw;
	int		 th;
	HBITMAP		 hbm;
	RGBQUAD*	 bits = NULL;
	HDC		 hmdc;
	char*		 t = MwUTF8ToACP(text);
	unsigned char*	 px;
	int		 y, x;
	MwRect		 r;
	MwLLPixmap	 p;

	tw = MwTextWidth(handle, ttf, text);
	th = MwTextHeight(handle, ttf, text);

	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		HFONT	 old_font   = SelectObject(handle->lowlevel->gdi.hDC, ttf->font);
		int	 old_bkmode = SetBkMode(handle->lowlevel->gdi.hDC, TRANSPARENT);
		COLORREF old_color  = SetTextColor(handle->lowlevel->gdi.hDC, color->gdi.color);

		TextOut(handle->lowlevel->gdi.hDC, point->x, point->y - th / 2, t, strlen(t));

		SetTextColor(handle->lowlevel->gdi.hDC, old_color);
		SetBkMode(handle->lowlevel->gdi.hDC, old_bkmode);
		SelectObject(handle->lowlevel->gdi.hDC, old_font);

		return 0;
	}

	px = malloc(tw * th * 4);

	bmih.biSize	     = sizeof(bmih);
	bmih.biWidth	     = tw;
	bmih.biHeight	     = -(LONG)th;
	bmih.biPlanes	     = 1;
	bmih.biBitCount	     = 32;
	bmih.biCompression   = BI_RGB;
	bmih.biSizeImage     = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed	     = 0;
	bmih.biClrImportant  = 0;

	hbm = CreateDIBSection(ttf->dc, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (void**)&bits, NULL, 0);

	hmdc = CreateCompatibleDC(ttf->dc);
	SelectObject(hmdc, hbm);
	SelectObject(hmdc, ttf->font);

	TextOut(hmdc, 0, 0, t, strlen(t));

	DeleteDC(hmdc);

	for(y = 0; y < th; y++) {
		for(x = 0; x < tw; x++) {
			unsigned char* opx = &px[(y * tw + x) * 4];

			opx[0] = color->common.red;
			opx[1] = color->common.green;
			opx[2] = color->common.blue;
			opx[3] = 255 - bits[y * tw + x].rgbRed;
		}
	}

	p	 = MwLoadRaw(handle, px, tw, th);
	r.x	 = point->x;
	r.y	 = point->y - th / 2;
	r.width	 = tw;
	r.height = th;

	MwLLDrawPixmap(handle->lowlevel, &r, p);
	MwLLDestroyPixmap(p);
	free(px);

	DeleteObject(hbm);
	free(t);

	return 0;
}

static int GDI_MwTextWidth(MwFLFont ttf, const char* text) {
	char* t = MwUTF8ToACP(text);
	SIZE  sz;

	GetTextExtentPoint32(ttf->dc, t, strlen(t), &sz);

	free(t);

	return sz.cx;
}

static int GDI_MwTextHeight(MwFLFont ttf, int count) {
	TEXTMETRIC tm;

	GetTextMetrics(ttf->dc, &tm);

	return tm.tmHeight * count;
}

static void* GDI_MwFontLoad(unsigned char* data, unsigned int size, int px) {
	MwFLFont  ttf = malloc(sizeof(*ttf));
	DWORD	  c;
	MwTTFInfo info;

	if((info = MwTTFGetInfo(data, size)) == NULL) {
		free(ttf);
		return NULL;
	}

	ttf->dc = GetDC(NULL);

	if((ttf->handle = AddFontMemResourceEx(data, size, 0, &c)) == NULL) {
		ReleaseDC(NULL, ttf->dc);
		MwTTFFreeInfo(info);
		free(ttf);
		return NULL;
	}

	if((ttf->font = CreateFont(-px, 0, 0, 0, info->weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, info->family)) == NULL) {
		RemoveFontMemResourceEx(ttf->handle);
		ReleaseDC(NULL, ttf->dc);
		MwTTFFreeInfo(info);
		free(ttf);
		return NULL;
	}

	MwTTFFreeInfo(info);

	SelectObject(ttf->dc, ttf->font);

	return ttf;
}

static void GDI_MwFontFree(void* handle) {
	MwFLFont ttf = handle;

	DeleteObject(ttf->font);
	RemoveFontMemResourceEx(ttf->handle);
	ReleaseDC(NULL, ttf->dc);
	free(ttf);
}

int MwFL_GDISetup(void) {
	MwFLDrawText   = GDI_MwDrawText;
	MwFLTextWidth  = GDI_MwTextWidth;
	MwFLTextHeight = GDI_MwTextHeight;
	MwFLFontLoad   = GDI_MwFontLoad;
	MwFLFontFree   = GDI_MwFontFree;
	return 0;
}
#endif
