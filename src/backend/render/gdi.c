/* $Id$ */
#include <Mw/Milsko.h>

static void MwLLPolygonImplNative(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	POINT* p   = malloc(sizeof(*p) * points_count);
	HPEN   pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	int    i;

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	SelectObject(handle->gdi.hDC, color->gdi.brush);
	SelectObject(handle->gdi.hDC, pen);
	SetPolyFillMode(handle->gdi.hDC, ALTERNATE);
	Polygon(handle->gdi.hDC, p, points_count);

	DeleteObject(pen);

	free(p);
}

static void MwLLLineImplNative(MwLL handle, MwPoint* points, MwLLColor color) {
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(color->common.red, color->common.green, color->common.blue));

	SelectObject(handle->gdi.hDC, pen);
	MoveToEx(handle->gdi.hDC, points[0].x, points[0].y, NULL);
	LineTo(handle->gdi.hDC, points[1].x, points[1].y);

	DeleteObject(pen);
}

static MwLLColor MwLLAllocColorImplNative(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	c->gdi.brush = NULL;

	MwLLColorUpdate(handle, c, r, g, b);

	return c;
}

static void MwLLColorUpdateImplNative(MwLL handle, MwLLColor c, int r, int g, int b) {
	HDC dc = GetDC(handle->gdi.hWnd);

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	if(c->gdi.brush != NULL) DeleteObject(c->gdi.brush);
	c->gdi.brush	= CreateSolidBrush(GetNearestColor(dc, RGB(r, g, b)));
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;

	ReleaseDC(handle->gdi.hWnd, dc);
}

static void MwLLFreeColorImplNative(MwLLColor color) {
	DeleteObject(color->gdi.brush);

	free(color);
}

static void MwLLSetBackgroundImplNative(MwLL handle, MwLLColor color) {
	(void)handle;
	(void)color;
}

static MwLLPixmap MwLLCreatePixmapImplNative(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap	 r  = malloc(sizeof(*r));
	HDC		 dc = GetDC(handle->gdi.hWnd);
	BITMAPINFOHEADER bmih;

	r->common.raw = malloc(width * height * 4);
	memcpy(r->common.raw, data, 4 * width * height);

	r->common.width	 = width;
	r->common.height = height;

	bmih.biSize	     = sizeof(bmih);
	bmih.biWidth	     = width;
	bmih.biHeight	     = -(LONG)height;
	bmih.biPlanes	     = 1;
	bmih.biBitCount	     = 32;
	bmih.biCompression   = BI_RGB;
	bmih.biSizeImage     = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed	     = 0;
	bmih.biClrImportant  = 0;

	r->gdi.hBitmap = CreateDIBSection(dc, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (void**)&r->gdi.quad, NULL, (DWORD)0);

	r->gdi.hMask  = NULL;
	r->gdi.hMask2 = NULL;

	ReleaseDC(handle->gdi.hWnd, dc);

	MwLLPixmapUpdate(r);

	return r;
}

static void MwLLPixmapUpdateImplNative(MwLLPixmap r) {
	int   y, x;
	int   w = (r->common.width + (16 - (r->common.width % 16))) / 8;
	WORD* words;
	WORD* words2;

	if(16 * (r->common.width / 16) == r->common.width) w -= 2;

	words  = malloc(w * r->common.height);
	words2 = malloc(w * r->common.height);
	memset(words, 0, w * r->common.height);
	memset(words2, 0, w * r->common.height);
	for(y = 0; y < r->common.height; y++) {
		BYTE* l	 = (BYTE*)&words[y * (w / 2)];
		BYTE* l2 = (BYTE*)&words2[y * (w / 2)];
		for(x = 0; x < r->common.width; x++) {
			RGBQUAD*       q  = &r->gdi.quad[y * r->common.width + x];
			unsigned char* px = &r->common.raw[(y * r->common.width + x) * 4];

			q->rgbRed   = px[0];
			q->rgbGreen = px[1];
			q->rgbBlue  = px[2];

			if(px[3]) {
				l[x / 8] |= 1 << (7 - (x % 8));
			} else {
				l2[x / 8] |= 1 << (7 - (x % 8));
			}
		}
	}

	if(r->gdi.hMask != NULL) DeleteObject(r->gdi.hMask);
	if(r->gdi.hMask2 != NULL) DeleteObject(r->gdi.hMask2);

	r->gdi.hMask  = CreateBitmap(r->common.width, r->common.height, 1, 1, words);
	r->gdi.hMask2 = CreateBitmap(r->common.width, r->common.height, 1, 1, words2);

	free(words);
	free(words2);
}

static void MwLLDestroyPixmapImplNative(MwLLPixmap pixmap) {
	free(pixmap->common.raw);
	DeleteObject(pixmap->gdi.hMask);
	DeleteObject(pixmap->gdi.hMask2);
	DeleteObject(pixmap->gdi.hBitmap);

	free(pixmap);
}

static void MwLLDrawPixmapImplNative(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	HDC   hmdc = CreateCompatibleDC(handle->gdi.hDC);
	POINT p[3];

	p[0].x = rect->x;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width;
	p[1].y = rect->y;

	p[2].x = rect->x;
	p[2].y = rect->y + rect->height;

	SelectObject(hmdc, pixmap->gdi.hBitmap);

	SetStretchBltMode(handle->gdi.hDC, HALFTONE);
	PlgBlt(handle->gdi.hDC, p, hmdc, 0, 0, pixmap->common.width, pixmap->common.height, pixmap->gdi.hMask, 0, 0);

	DeleteDC(hmdc);
}
