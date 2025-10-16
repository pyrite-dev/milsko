/* $Id$ */
#include <Mw/Milsko.h>

typedef struct userdata {
	MwLL  ll;
	POINT min;
	POINT max;
	int   min_set;
	int   max_set;
} userdata_t;

static LRESULT CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	userdata_t* u = (userdata_t*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(u == NULL) return DefWindowProc(hWnd, msg, wp, lp);

	if(msg == WM_PAINT) {
		PAINTSTRUCT ps;
		RECT	    rc;
		HBITMAP	    hbmp;
		HDC	    dc, hbdc;

		if(u->ll->copy_buffer) {
			GetClientRect(hWnd, &rc);

			dc   = GetDC(hWnd);
			hbmp = CreateCompatibleBitmap(dc, rc.right - rc.left, rc.bottom - rc.top);
			hbdc = CreateCompatibleDC(dc);
			SelectObject(hbdc, hbmp);
			ReleaseDC(hWnd, dc);

			u->ll->hDC = hbdc;
			MwLLDispatch(u->ll, draw, NULL);

			dc = BeginPaint(hWnd, &ps);
			StretchBlt(dc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hbdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
			EndPaint(hWnd, &ps);

			DeleteDC(hbdc);
			DeleteObject(hbmp);
		} else {
			u->ll->hDC = BeginPaint(hWnd, &ps);
			MwLLDispatch(u->ll, draw, NULL);
			EndPaint(hWnd, &ps);
		}
	} else if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		MwLLMouse p;
		p.point.x = LOWORD(lp);
		p.point.y = HIWORD(lp);
		if(msg == WM_LBUTTONDOWN) {
			p.button = MwLLMouseLeft;
		} else if(msg == WM_MBUTTONDOWN) {
			p.button = MwLLMouseMiddle;
		} else if(msg == WM_RBUTTONDOWN) {
			p.button = MwLLMouseRight;
		}

		SetCapture(hWnd);
		SetFocus(hWnd);
		MwLLDispatch(u->ll, down, &p);
	} else if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
		MwLLMouse p;
		p.point.x = LOWORD(lp);
		p.point.y = HIWORD(lp);
		if(msg == WM_LBUTTONUP) {
			p.button = MwLLMouseLeft;
		} else if(msg == WM_MBUTTONUP) {
			p.button = MwLLMouseMiddle;
		} else if(msg == WM_RBUTTONUP) {
			p.button = MwLLMouseRight;
		}

		SetCapture(NULL);
		MwLLDispatch(u->ll, up, &p);
	} else if(msg == WM_MOUSEWHEEL) {
		int	  d = GET_WHEEL_DELTA_WPARAM(wp);
		MwLLMouse p;
		p.point.x = LOWORD(lp);
		p.point.y = HIWORD(lp);

		if(d > 0) {
			p.button = MwLLMouseWheelUp;
		} else if(d < 0) {
			p.button = MwLLMouseWheelDown;
		}

		MwLLDispatch(u->ll, down, &p);
		MwLLDispatch(u->ll, up, &p);
	} else if(msg == WM_MOUSEMOVE) {
		MwPoint p;
		p.x = LOWORD(lp);
		p.y = HIWORD(lp);

		MwLLDispatch(u->ll, move, &p);
	} else if(msg == WM_SIZE) {
		MwLLDispatch(u->ll, resize, NULL);
	} else if(msg == WM_ERASEBKGND) {
		return 1;
	} else if(msg == WM_NCHITTEST) {
		LPARAM style = GetWindowLongPtr(hWnd, GWL_STYLE);
		if(style & WS_CHILD) return HTCLIENT;
		return DefWindowProc(hWnd, msg, wp, lp);
	} else if(msg == WM_DESTROY) {
		PostQuitMessage(0);
	} else if(msg == WM_CLOSE) {
		MwLLDispatch(u->ll, close, NULL);
	} else if(msg == WM_CHAR) {
		int n = wp;

		MwLLDispatch(u->ll, key, &n);
	} else if(msg == WM_KEYDOWN || msg == WM_KEYUP) {
		int n = -1;
		if(wp == VK_LEFT) n = MwLLKeyLeft;
		if(wp == VK_RIGHT) n = MwLLKeyRight;
		if(wp == VK_UP) n = MwLLKeyUp;
		if(wp == VK_DOWN) n = MwLLKeyDown;
		if(n != -1) {
			if(msg == WM_KEYDOWN) {
				MwLLDispatch(u->ll, key, &n);
			} else {
				MwLLDispatch(u->ll, key_released, &n);
			}
		}
	} else if(msg == WM_GETMINMAXINFO) {
		if(u->min_set || u->max_set) {
			LPARAM	    style = GetWindowLongPtr(hWnd, GWL_STYLE);
			MINMAXINFO* mmi	  = (MINMAXINFO*)lp;
			if(u->min_set) {
				RECT rc;

				rc.left	  = 0;
				rc.top	  = 0;
				rc.right  = u->min.x;
				rc.bottom = u->min.y;
				AdjustWindowRect(&rc, style, FALSE);

				mmi->ptMinTrackSize.x = rc.right - rc.left;
				mmi->ptMinTrackSize.y = rc.bottom - rc.top;
			}
			if(u->max_set) {
				RECT rc;

				rc.left	  = 0;
				rc.top	  = 0;
				rc.right  = u->min.x;
				rc.bottom = u->min.y;
				AdjustWindowRect(&rc, style, FALSE);

				mmi->ptMaxTrackSize.x = rc.right - rc.left;
				mmi->ptMaxTrackSize.y = rc.bottom - rc.top;
			}
		} else {
			return DefWindowProc(hWnd, msg, wp, lp);
		}
	} else {
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height) {
	MwLL	    r = malloc(sizeof(*r));
	userdata_t* u = malloc(sizeof(*u));
	WNDCLASSEX  wc;

	memset(&wc, 0, sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.style	 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = wndproc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = (HINSTANCE)GetModuleHandle(NULL);
	wc.lpszClassName = "milsko";
	wc.lpszMenuName	 = NULL;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
	wc.hIcon	 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm	 = NULL;

	MwLLCreateCommon(r);

	RegisterClassEx(&wc);

	r->copy_buffer = 1;
	r->hWnd	       = CreateWindow("milsko", "Milsko", parent == NULL ? (WS_OVERLAPPEDWINDOW) : (WS_CHILD | WS_VISIBLE), x == MwDEFAULT ? CW_USEDEFAULT : x, y == MwDEFAULT ? CW_USEDEFAULT : y, width, height, parent == NULL ? NULL : parent->hWnd, 0, wc.hInstance, NULL);
	r->hInstance   = wc.hInstance;

	u->ll	   = r;
	u->min_set = 0;
	u->max_set = 0;
	SetWindowLongPtr(r->hWnd, GWLP_USERDATA, (LPARAM)u);

	if(parent == NULL) {
		RECT rc;

		rc.left	  = 0;
		rc.top	  = 0;
		rc.right  = width;
		rc.bottom = height;
		AdjustWindowRect(&rc, GetWindowLongPtr(r->hWnd, GWL_STYLE), FALSE);
		SetWindowPos(r->hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);

		ShowWindow(r->hWnd, SW_NORMAL);
		UpdateWindow(r->hWnd);

		InvalidateRect(r->hWnd, NULL, FALSE);
	}

	return r;
}

void MwLLDestroy(MwLL handle) {
	MwLLDestroyCommon(handle);

	/* for safety */
	SetWindowLongPtr(handle->hWnd, GWLP_USERDATA, (LPARAM)NULL);
	DestroyWindow(handle->hWnd);

	free(handle);
}

void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	POINT* p   = malloc(sizeof(*p) * points_count);
	HPEN   pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	int    i;

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	SelectObject(handle->hDC, color->brush);
	SelectObject(handle->hDC, pen);
	SetPolyFillMode(handle->hDC, ALTERNATE);
	Polygon(handle->hDC, p, points_count);

	DeleteObject(pen);

	free(p);
}

void MwLLLine(MwLL handle, MwPoint* points, MwLLColor color) {
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(color->red, color->green, color->blue));

	SelectObject(handle->hDC, pen);
	MoveToEx(handle->hDC, points[0].x, points[0].y, NULL);
	LineTo(handle->hDC, points[1].x, points[1].y);

	DeleteObject(pen);
}

MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b) {
	MwLLColor c  = malloc(sizeof(*c));
	HDC	  dc = GetDC(handle->hWnd);

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	c->brush = CreateSolidBrush(GetNearestColor(dc, RGB(r, g, b)));
	c->red	 = r;
	c->green = g;
	c->blue	 = b;

	ReleaseDC(handle->hWnd, dc);

	return c;
}

void MwLLFreeColor(MwLLColor color) {
	DeleteObject(color->brush);

	free(color);
}

void MwLLSetBackground(MwLL handle, MwLLColor color) {
	(void)handle;
	(void)color;
}

void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	RECT rc;

	GetClientRect(handle->hWnd, &rc);
	MapWindowPoints(handle->hWnd, GetParent(handle->hWnd), (LPPOINT)&rc, 2);

	*x = rc.left;
	*y = rc.top;
	*w = rc.right - rc.left;
	*h = rc.bottom - rc.top;
}

void MwLLSetXY(MwLL handle, int x, int y) {
	SetWindowPos(handle->hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
	InvalidateRect(handle->hWnd, NULL, FALSE);
}

void MwLLSetWH(MwLL handle, int w, int h) {
	SetWindowPos(handle->hWnd, NULL, 0, 0, w, h, SWP_NOMOVE);
	InvalidateRect(handle->hWnd, NULL, FALSE);
}

void MwLLSetTitle(MwLL handle, const char* title) {
	SetWindowText(handle->hWnd, title);
}

int MwLLPending(MwLL handle) {
	MSG msg;

	(void)handle;

	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) ? 1 : 0;
}

void MwLLNextEvent(MwLL handle) {
	MSG msg;

	(void)handle;

	while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void MwLLSleep(int ms) {
	Sleep(ms);
}

MwLLPixmap MwLLCreatePixmap(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap	 r  = malloc(sizeof(*r));
	HDC		 dc = GetDC(handle->hWnd);
	BITMAPINFOHEADER bmih;
	RGBQUAD*	 quad;
	int		 y, x;
	int		 w = (width + (16 - (width % 16))) / 8;
	WORD*		 words;

	if(16 * (width / 16) == width) w -= 2;

	words = malloc(w * height);

	r->width  = width;
	r->height = height;

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

	r->hBitmap = CreateDIBSection(dc, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (void**)&quad, NULL, (DWORD)0);

	memset(words, 0, w * height);
	for(y = 0; y < height; y++) {
		BYTE* l = (BYTE*)&words[y * (w / 2)];
		for(x = 0; x < width; x++) {
			RGBQUAD*       q  = &quad[y * width + x];
			unsigned char* px = &data[(y * width + x) * 4];

			q->rgbRed   = px[0];
			q->rgbGreen = px[1];
			q->rgbBlue  = px[2];

			if(px[3]) {
				l[x / 8] |= 1 << (7 - (x % 8));
			}
		}
	}

	r->hMask = CreateBitmap(width, height, 1, 1, words);

	free(words);

	ReleaseDC(handle->hWnd, dc);

	return r;
}

void MwLLDestroyPixmap(MwLLPixmap pixmap) {
	DeleteObject(pixmap->hBitmap);

	free(pixmap);
}

void MwLLDrawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	HDC   hmdc = CreateCompatibleDC(handle->hDC);
	POINT p[3];

	p[0].x = rect->x;
	p[0].y = rect->y;

	p[1].x = rect->x + rect->width;
	p[1].y = rect->y;

	p[2].x = rect->x;
	p[2].y = rect->y + rect->height;

	SelectObject(hmdc, pixmap->hBitmap);

	SetStretchBltMode(handle->hDC, HALFTONE);
	PlgBlt(handle->hDC, p, hmdc, 0, 0, pixmap->width, pixmap->height, pixmap->hMask, 0, 0);

	DeleteDC(hmdc);
}

void MwLLSetIcon(MwLL handle, MwLLPixmap pixmap) {
	ICONINFO ii;
	HICON	 ico;

	memset(&ii, 0, sizeof(ii));
	ii.fIcon    = TRUE;
	ii.xHotspot = 0;
	ii.yHotspot = 0;
	ii.hbmMask  = pixmap->hMask;
	ii.hbmColor = pixmap->hBitmap;

	ico = CreateIconIndirect(&ii);

	SetClassLongPtr(handle->hWnd, GCLP_HICON, (LPARAM)ico);

	DestroyIcon(ico);
}

void MwLLForceRender(MwLL handle) {
	InvalidateRect(handle->hWnd, NULL, FALSE);
}

void MwLLSetCursor(MwLL handle, MwCursor* image, MwCursor* mask) {
	HCURSOR cursor;
	BYTE*	dmask  = malloc((MwCursorDataHeight / 8) * MwCursorDataHeight);
	BYTE*	dimage = malloc((MwCursorDataHeight / 8) * MwCursorDataHeight);
	int	y, x, ys, xs;

	xs = -mask->x + image->x;
	ys = MwCursorDataHeight + mask->y;
	ys = MwCursorDataHeight + image->y - ys;

	memset(dmask, 0xff, (MwCursorDataHeight / 8) * MwCursorDataHeight);
	memset(dimage, 0, (MwCursorDataHeight / 8) * MwCursorDataHeight);

	for(y = 0; y < mask->height; y++) {
		BYTE*	     l = &dmask[y * (MwCursorDataHeight / 8)];
		unsigned int n = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			l[x / 8] = l[x / 8] >> 1;
			if(!(n & 1)) {
				l[x / 8] |= 1 << 7;
			}

			n = n >> 1;
		}
	}

	for(y = 0; y < image->height; y++) {
		BYTE*	     l = &dimage[(y + ys) * (MwCursorDataHeight / 8)];
		unsigned int n = image->data[y];

		for(x = image->width - 1; x >= 0; x--) {
			if(n & 1) {
				l[(x + xs) / 8] |= (1 << (7 - ((x + xs) % 8)));
			}

			n = n >> 1;
		}
	}

	cursor = CreateCursor(GetModuleHandle(NULL), xs, ys, MwCursorDataHeight, MwCursorDataHeight, dmask, dimage);

	SetClassLongPtr(handle->hWnd, GCLP_HCURSOR, (LPARAM)cursor);
	SetCursor(cursor);

	DestroyCursor(cursor);

	free(dimage);
	free(dmask);
}

void MwLLDetach(MwLL handle, MwPoint* point) {
	RECT   rc, rc2;
	LPARAM lp = GetWindowLongPtr(handle->hWnd, GWL_STYLE);

	lp &= WS_VISIBLE;

	GetWindowRect(GetParent(handle->hWnd), &rc);

	GetClientRect(handle->hWnd, &rc2);

	SetWindowLongPtr(handle->hWnd, GWL_STYLE, (LPARAM)WS_OVERLAPPEDWINDOW | lp);
	SetParent(handle->hWnd, NULL);

	rc.left += point->x;
	rc.top += point->y;

	AdjustWindowRect(&rc2, WS_OVERLAPPEDWINDOW | lp, FALSE);
	rc2.right -= rc2.left;
	rc2.bottom -= rc2.top;

	SetWindowPos(handle->hWnd, HWND_TOPMOST, rc.left, rc.top, rc2.right == 0 ? 1 : rc2.right, rc2.bottom == 0 ? 1 : rc2.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

void MwLLShow(MwLL handle, int show) {
	ShowWindow(handle->hWnd, show ? SW_NORMAL : SW_HIDE);
	if(show) SetFocus(handle->hWnd);
}

void MwLLMakePopup(MwLL handle, MwLL parent) {
	(void)handle;
	(void)parent;
	/* TODO */
}

void MwLLSetSizeHints(MwLL handle, int minx, int miny, int maxx, int maxy) {
	userdata_t* u = (userdata_t*)GetWindowLongPtr(handle->hWnd, GWLP_USERDATA);

	u->min_set = u->max_set = 1;
	u->min.x		= minx;
	u->min.y		= miny;
	u->max.x		= maxx;
	u->max.y		= maxy;
}

void MwLLMakeBorderless(MwLL handle, int toggle) {
	LPARAM lp = GetWindowLongPtr(handle->hWnd, GWL_STYLE);

	if(toggle) {
		lp &= ~WS_CAPTION;
	} else {
		lp |= WS_CAPTION;
	}

	SetWindowLongPtr(handle->hWnd, GWL_STYLE, lp);

	SetWindowPos(handle->hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

long MwLLGetTick(void) {
	return GetTickCount();
}
