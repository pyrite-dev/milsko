/* $Id$ */
#include <Mw/Milsko.h>

typedef struct userdata {
	MwLL	ll;
	WNDPROC old;
} userdata_t;

static LRESULT CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	userdata_t* u = (userdata_t*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(u == NULL) return DefWindowProc(hWnd, msg, wp, lp);

	if(msg == WM_PAINT) {
		PAINTSTRUCT ps;
		u->ll->hDC = BeginPaint(hWnd, &ps);
		MwLLDispatch(u->ll, draw);
		EndPaint(hWnd, &ps);
	} else if(msg == WM_LBUTTONDOWN) {
		MwLLDispatch(u->ll, down);
		InvalidateRect(hWnd, NULL, FALSE);
	} else if(msg == WM_LBUTTONUP) {
		MwLLDispatch(u->ll, up);
		InvalidateRect(hWnd, NULL, FALSE);
	} else if(msg == WM_ERASEBKGND) {
	} else if(msg == WM_NCHITTEST) {
		return HTCLIENT;
	} else {
		return (u->old == NULL) ? DefWindowProc(hWnd, msg, wp, lp) : CallWindowProc(u->old, hWnd, msg, wp, lp);
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
	wc.hIconSm	 = LoadIcon(NULL, IDI_WINLOGO);

	MwLLCreateCommon(r);

	RegisterClassEx(&wc);

	r->hWnd = CreateWindow(parent == NULL ? "milsko" : "STATIC", "Milsko", parent == NULL ? (WS_OVERLAPPEDWINDOW) : (WS_CHILD | WS_VISIBLE), x, y, width, height, parent == NULL ? NULL : parent->hWnd, 0, wc.hInstance, NULL);

	u->ll = r;
	if(parent == NULL) {
		u->old = NULL;
	} else {
		u->old = (WNDPROC)SetWindowLongPtr(r->hWnd, GWLP_WNDPROC, (LONG_PTR)wndproc);
	}
	SetWindowLongPtr(r->hWnd, GWLP_USERDATA, (LONG_PTR)u);

	if(parent == NULL) {
		RECT rc;
		ShowWindow(r->hWnd, SW_NORMAL);
		UpdateWindow(r->hWnd);

		rc.left	  = 0;
		rc.top	  = 0;
		rc.right  = width;
		rc.bottom = height;
		AdjustWindowRect(&rc, GetWindowLongPtr(r->hWnd, GWL_STYLE), FALSE);
		SetWindowPos(r->hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);
	}

	return r;
}

void MwLLDestroy(MwLL handle) {
	MwLLDestroyCommon(handle);

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

	DeleteDC(dc);

	return c;
}

void MwLLFreeColor(MwLLColor color) {
	DeleteObject(color->brush);

	free(color);
}

void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	RECT rc;

	GetClientRect(handle->hWnd, &rc);

	*x = rc.left;
	*y = rc.top;
	*w = rc.right - rc.left;
	*h = rc.bottom - rc.top;
}

void MwLLSetXY(MwLL handle, int x, int y) {
	SetWindowPos(handle->hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void MwLLSetWH(MwLL handle, int w, int h) {
	SetWindowPos(handle->hWnd, NULL, 0, 0, w, h, SWP_NOMOVE);
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
