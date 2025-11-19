/* $Id$ */
#include "Mw/LowLevel/GDI.h"
#include "Mw/LowLevel.h"
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

		if(u->ll->common.copy_buffer) {
			GetClientRect(hWnd, &rc);

			dc   = GetDC(hWnd);
			hbmp = CreateCompatibleBitmap(dc, rc.right - rc.left, rc.bottom - rc.top);
			hbdc = CreateCompatibleDC(dc);
			SelectObject(hbdc, hbmp);
			ReleaseDC(hWnd, dc);

			u->ll->gdi.hDC = hbdc;
			MwLLDispatch(u->ll, draw, NULL);

			dc = BeginPaint(hWnd, &ps);
			StretchBlt(dc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hbdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
			EndPaint(hWnd, &ps);

			DeleteDC(hbdc);
			DeleteObject(hbmp);
		} else {
			u->ll->gdi.hDC = BeginPaint(hWnd, &ps);
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

		if(u->ll->gdi.grabbed) {
			RECT rc;

			GetClientRect(hWnd, &rc);
			p.x -= (rc.right - rc.left) / 2;
			p.y -= (rc.bottom - rc.top) / 2;
		}

		MwLLDispatch(u->ll, move, &p);

		if(u->ll->gdi.grabbed && (p.x != 0 || p.y != 0)) {
			RECT  rc;
			POINT p;

			GetClientRect(hWnd, &rc);

			p.x = (rc.right - rc.left) / 2;
			p.y = (rc.bottom - rc.top) / 2;

			MapWindowPoints(hWnd, HWND_DESKTOP, &p, 1);

			SetCursorPos(p.x, p.y);
		}
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
	} else if(msg == WM_CHAR || msg == WM_SYSCHAR) {
		int	  n    = wp;
		const int base = 'A' - 1;

		if(n != 0x1b && n <= 0x1f) {
			n = (n + base) | MwLLControlMask;
			if(!(GetKeyState(VK_LSHIFT) || GetKeyState(VK_RSHIFT))) n += 0x20;
		}
		if(HIBYTE(VkKeyScan(wp)) & 2) n |= MwLLControlMask;
		if(msg == WM_SYSCHAR) n |= MwLLAltMask;

		if((0x20 <= n && n <= 0x7f) || (n & MwLLKeyMask)) MwLLDispatch(u->ll, key, &n);
	} else if(msg == WM_SETFOCUS) {
		MwLLDispatch(u->ll, focus_in, NULL);
	} else if(msg == WM_KILLFOCUS) {
		MwLLDispatch(u->ll, focus_out, NULL);
	} else if(msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) {
		int n = -1;
		if(wp == VK_MENU && msg == WM_KEYUP) return 0;
		if(wp == VK_LEFT) n = MwLLKeyLeft;
		if(wp == VK_RIGHT) n = MwLLKeyRight;
		if(wp == VK_UP) n = MwLLKeyUp;
		if(wp == VK_DOWN) n = MwLLKeyDown;
		if(wp == VK_RETURN) n = MwLLKeyEnter;
		if(wp == VK_BACK) n = MwLLKeyBackSpace;
		if(wp == VK_ESCAPE) n = MwLLKeyEscape;
		if(wp == VK_LSHIFT) n = MwLLKeyLeftShift;
		if(wp == VK_RSHIFT) n = MwLLKeyRightShift;
		if(wp == VK_MENU) n = MwLLKeyAlt;
		if(wp == VK_CONTROL) n = MwLLKeyControl;

		if((msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) && n != -1 && wp != VK_MENU) {
			n |= MwLLAltMask;
		}
		if(n != -1) {
			if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) {
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
	} else if(msg == WM_SETCURSOR) {
		if(LOWORD(lp) != HTCLIENT) return DefWindowProc(hWnd, msg, wp, lp);
		if(u->ll->gdi.cursor != NULL) SetCursor(u->ll->gdi.cursor);
	} else if(msg == WM_USER) {
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
	} else {
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
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

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendGDI;

	r->gdi.grabbed	 = 0;
	r->gdi.hWnd	 = CreateWindow("milsko", "Milsko", parent == NULL ? (WS_OVERLAPPEDWINDOW) : (WS_CHILD | WS_VISIBLE), x == MwDEFAULT ? CW_USEDEFAULT : x, y == MwDEFAULT ? CW_USEDEFAULT : y, width, height, parent == NULL ? NULL : parent->gdi.hWnd, 0, wc.hInstance, NULL);
	r->gdi.hInstance = wc.hInstance;
	r->gdi.cursor	 = NULL;

	u->ll	   = r;
	u->min_set = 0;
	u->max_set = 0;
	SetWindowLongPtr(r->gdi.hWnd, GWLP_USERDATA, (LPARAM)u);

	if(parent == NULL) {
		RECT rc;

		rc.left	  = 0;
		rc.top	  = 0;
		rc.right  = width;
		rc.bottom = height;
		AdjustWindowRect(&rc, GetWindowLongPtr(r->gdi.hWnd, GWL_STYLE), FALSE);
		SetWindowPos(r->gdi.hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);

		ShowWindow(r->gdi.hWnd, SW_NORMAL);
		UpdateWindow(r->gdi.hWnd);

		InvalidateRect(r->gdi.hWnd, NULL, FALSE);
	}

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	/* for safety */
	SetWindowLongPtr(handle->gdi.hWnd, GWLP_USERDATA, (LPARAM)NULL);
	DestroyWindow(handle->gdi.hWnd);

	if(handle->gdi.cursor != NULL) DestroyCursor(handle->gdi.cursor);

	free(handle);
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	RECT rc;

	GetClientRect(handle->gdi.hWnd, &rc);
	MapWindowPoints(handle->gdi.hWnd, GetParent(handle->gdi.hWnd), (LPPOINT)&rc, 2);

	*x = rc.left;
	*y = rc.top;
	*w = rc.right - rc.left;
	*h = rc.bottom - rc.top;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	SetWindowPos(handle->gdi.hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
	InvalidateRect(handle->gdi.hWnd, NULL, FALSE);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	SetWindowPos(handle->gdi.hWnd, NULL, 0, 0, w, h, SWP_NOMOVE);
	InvalidateRect(handle->gdi.hWnd, NULL, FALSE);
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	SetWindowText(handle->gdi.hWnd, title);
}

static int MwLLPendingImpl(MwLL handle) {
	MSG msg;

	(void)handle;

	return PeekMessage(&msg, handle->gdi.hWnd, 0, 0, PM_NOREMOVE) ? 1 : 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	MSG msg;

	(void)handle;

	while(PeekMessage(&msg, handle->gdi.hWnd, 0, 0, PM_NOREMOVE)) {
		GetMessage(&msg, handle->gdi.hWnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	ICONINFO ii;
	HICON	 ico;

	memset(&ii, 0, sizeof(ii));
	ii.fIcon    = TRUE;
	ii.xHotspot = 0;
	ii.yHotspot = 0;
	ii.hbmMask  = pixmap->gdi.hMask2;
	ii.hbmColor = pixmap->gdi.hBitmap;

	ico = CreateIconIndirect(&ii);

	SetClassLongPtr(handle->gdi.hWnd, GCLP_HICON, (LPARAM)ico);

	DestroyIcon(ico);
}

static void MwLLForceRenderImpl(MwLL handle) {
	PostMessage(handle->gdi.hWnd, WM_USER, 0, 0);
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
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

	if(handle->gdi.cursor != NULL) DestroyCursor(handle->gdi.cursor);
	handle->gdi.cursor = cursor;

	free(dimage);
	free(dmask);
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	RECT   rc, rc2;
	LPARAM lp  = GetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE);
	LPARAM lp2 = GetWindowLongPtr(handle->gdi.hWnd, GWL_EXSTYLE);

	lp &= WS_VISIBLE;

	GetWindowRect(GetParent(handle->gdi.hWnd), &rc);

	GetClientRect(handle->gdi.hWnd, &rc2);

	if(lp2 & WS_EX_TOOLWINDOW) {
		SetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE, (LPARAM)lp);
	} else {
		SetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE, (LPARAM)WS_OVERLAPPEDWINDOW | lp);
	}
	SetParent(handle->gdi.hWnd, NULL);

	rc.left += point->x;
	rc.top += point->y;

	AdjustWindowRect(&rc2, GetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE), FALSE);
	rc2.right -= rc2.left;
	rc2.bottom -= rc2.top;

	SetWindowPos(handle->gdi.hWnd, HWND_TOPMOST, rc.left, rc.top, rc2.right == 0 ? 1 : rc2.right, rc2.bottom == 0 ? 1 : rc2.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);
}

static void MwLLShowImpl(MwLL handle, int show) {
	ShowWindow(handle->gdi.hWnd, show ? SW_NORMAL : SW_HIDE);
	if(show) SetFocus(handle->gdi.hWnd);
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	(void)handle;
	(void)parent;
	/* TODO */
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	userdata_t* u = (userdata_t*)GetWindowLongPtr(handle->gdi.hWnd, GWLP_USERDATA);

	u->min_set = u->max_set = 1;
	u->min.x		= minx;
	u->min.y		= miny;
	u->max.x		= maxx;
	u->max.y		= maxy;
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	LPARAM lp = GetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE);

	if(toggle) {
		lp &= ~WS_CAPTION;
	} else {
		lp |= WS_CAPTION;
	}

	SetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE, lp);

	SetWindowPos(handle->gdi.hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

static void MwLLFocusImpl(MwLL handle) {
	SetFocus(handle->gdi.hWnd);
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	if(toggle) {
		POINT p;
		RECT  rc;

		GetClientRect(handle->gdi.hWnd, &rc);

		rc.right -= rc.left;
		rc.bottom -= rc.top;

		p.x = (rc.right - rc.left) / 2;
		p.y = (rc.bottom - rc.top) / 2;

		MapWindowPoints(handle->gdi.hWnd, HWND_DESKTOP, &p, 1);

		handle->gdi.grabbed = 1;

		SetFocus(handle->gdi.hWnd);

		GetWindowRect(handle->gdi.hWnd, &rc);
		ClipCursor(&rc);

		SetCursorPos(p.x, p.y);
	} else {
		handle->gdi.grabbed = 0;
		ClipCursor(NULL);
	}
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	HGLOBAL hg;
	if(OpenClipboard(handle->gdi.hWnd) != 0) {
		char* lock;

		EmptyClipboard();
		hg = GlobalAlloc(GHND | GMEM_SHARE, strlen(text) + 1);

		lock = GlobalLock(hg);
		strcpy(lock, text);
		GlobalUnlock(hg);

		SetClipboardData(CF_TEXT, hg);

		CloseClipboard();
	}
}

static char* MwLLGetClipboardImpl(MwLL handle) {
	HGLOBAL hg;
	char*	r = NULL;
	if(OpenClipboard(handle->gdi.hWnd) != 0 && (hg = GetClipboardData(CF_TEXT)) != NULL) {
		char* lock;

		r = malloc(GlobalSize(hg));

		lock = GlobalLock(hg);
		strcpy(r, lock);
		GlobalUnlock(hg);

		CloseClipboard();
	}
	return r;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	LPARAM lp = GetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE) & (WS_VISIBLE | WS_CHILD);
	RECT   rc;
	int    w, h;

	SetWindowLongPtr(handle->gdi.hWnd, GWL_STYLE, (LPARAM)lp);
	SetWindowLongPtr(handle->gdi.hWnd, GWL_EXSTYLE, (LPARAM)WS_EX_TOOLWINDOW);

	GetClientRect(handle->gdi.hWnd, &rc);

	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	SetWindowPos(handle->gdi.hWnd, NULL, 0, 0, w, h, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	(void)handle;
}

static int MwLLGDICallInitImpl(void) {
	/* TODO: check properly */
	return 0;
}

#ifdef BUILD_OPENGL
static MwLLGL MwLLGLCreateImpl(MwLL handle) {
	MwLLGL		      r = malloc(sizeof(struct _MwLLGDIGL));
	MwLLGL		      r = NULL;
	PIXELFORMATDESCRIPTOR pfd;
	int		      pf;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 32;

	r->gdi.dc = GetDC(handle->gdi.hWnd);

	pf = ChoosePixelFormat(r->gdi.dc, &pfd);
	SetPixelFormat(r->gdi.dc, pf, &pfd);

	r->gdi.lib = MwDynamicOpen("opengl32.dll");

	r->gdi.wglCreateContext	 = (MWwglCreateContext)(void*)MwDynamicSymbol(r->gdi.lib, "wglCreateContext");
	r->gdi.wglMakeCurrent	 = (MWwglMakeCurrent)(void*)MwDynamicSymbol(r->gdi.lib, "wglMakeCurrent");
	r->gdi.wglDeleteContext	 = (MWwglDeleteContext)(void*)MwDynamicSymbol(r->gdi.lib, "wglDeleteContext");
	r->gdi.wglGetProcAddress = (MWwglGetProcAddress)(void*)MwDynamicSymbol(r->gdi.lib, "wglGetProcAddress");

	r->gdi.gl = r->gdi.wglCreateContext(r->gdi.dc);

	return r;
}

static void MwLLGLDestroyImpl(MwLL ll, MwLLGL gl) {
	gl->gdi.wglMakeCurrent(NULL, NULL);
	DeleteDC(gl->gdi.dc);
	gl->gdi.wglDeleteContext(gl->gdi.gl);

	MwDynamicClose(gl->gdi.lib);

	free(gl);
}

static void MwLLGLMakeCurrentImpl(MwLL ll,MwLLGL gl {
	o->wglMakeCurrent(gl->gdi.dc, gl->gdi.gl);
}

static void MwLLGLSwapBufferImpl(MwLL ll,MwLLGL gl) {
	SwapBuffers(gl->gdi.dc);
}

static void* MwLLGLGetProcAddressImpl(MwLL ll,MwLLGL gl, const char* name) {
	return gl->gdi.wglGetProcAddress(name);
}
#endif

#include "call.c"
CALL(GDI);
