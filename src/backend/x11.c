/* $Id$ */
#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

typedef struct mwm_hints {
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long	      input_mode;
	unsigned long status;
} mwm_hints_t;
enum mwm_hints_enum {
	MWM_HINTS_FUNCTIONS   = (1L << 0),
	MWM_HINTS_DECORATIONS = (1L << 1),

	MWM_FUNC_ALL	  = (1L << 0),
	MWM_FUNC_RESIZE	  = (1L << 1),
	MWM_FUNC_MOVE	  = (1L << 2),
	MWM_FUNC_MINIMIZE = (1L << 3),
	MWM_FUNC_MAXIMIZE = (1L << 4),
	MWM_FUNC_CLOSE	  = (1L << 5)
};

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | KeymapNotify | FocusChangeMask;

static void create_pixmap(MwLL handle) {
	XWindowAttributes attr;
	int		  x, y;
	unsigned int	  w, h;

	MwLLGetXYWH(handle, &x, &y, &w, &h);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	handle->x11.pixmap = XCreatePixmap(handle->x11.display, handle->x11.window, w, h, attr.depth);
}

static void destroy_pixmap(MwLL handle) {
	XFreePixmap(handle->x11.display, handle->x11.pixmap);
}

static void sync_move(MwLL handle, int x, int y) {
	XEvent*		  queue = NULL;
	XEvent		  ev;
	unsigned long	  n = MwTimeGetTick() + 100;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);

	XSync(handle->x11.display, False);

	while(!xwa.override_redirect && n > MwTimeGetTick()) {
		XSync(handle->x11.display, False);
		if(!XPending(handle->x11.display)) continue;
		XNextEvent(handle->x11.display, &ev);
		if(ev.type == ReparentNotify && ev.xreparent.window == handle->x11.window && ev.xreparent.window != RootWindow(handle->x11.display, DefaultScreen(handle->x11.display))) {
			break;
		} else {
			arrput(queue, ev);
		}
	}

	while(arrlen(queue) > 0) {
		XPutBackEvent(handle->x11.display, &queue[0]);
		arrdel(queue, 0);
	}
	arrfree(queue);

	MwLLSetXY(handle, x, y);
}

static void wait_map(MwLL handle, int sync) {
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.map_state != IsViewable) {
		XSync(handle->x11.display, False);

		XMapWindow(handle->x11.display, handle->x11.window);
		XSync(handle->x11.display, False);

		if(!sync) return;
		do {
			XMapWindow(handle->x11.display, handle->x11.window);
			XSync(handle->x11.display, False);
			XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
		} while(xwa.map_state != IsViewable);
	}
}

static void wait_unmap(MwLL handle, int sync) {
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.map_state != IsUnmapped) {
		XSync(handle->x11.display, False);

		XUnmapWindow(handle->x11.display, handle->x11.window);
		XSync(handle->x11.display, False);

		if(!sync) return;
		do {
			XUnmapWindow(handle->x11.display, handle->x11.window);
			XSync(handle->x11.display, False);
			XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
		} while(xwa.map_state != IsUnmapped);
	}
}

static unsigned long generate_color(MwLL handle, unsigned long r, unsigned long g, unsigned long b) {
	unsigned long c = 0;

	c |= (r * handle->x11.red_max / 255) << handle->x11.red_shift;

	c |= (g * handle->x11.green_max / 255) << handle->x11.green_shift;

	c |= (b * handle->x11.blue_max / 255) << handle->x11.blue_shift;

	return c;
}

static XVisualInfo* get_visual_info(Display* display) {
	XVisualInfo xvi;
	int	    n;
	Visual*	    visual = DefaultVisual(display, DefaultScreen(display));

	xvi.visualid = XVisualIDFromVisual(visual);

	return XGetVisualInfo(display, VisualIDMask, &xvi, &n);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL	      r;
	Window	      p;
	XVisualInfo*  xvi;
	unsigned long n = 1;
	int	      i;
	int	      px = x, py = y;
	XSizeHints    sh;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

	if(px == MwDEFAULT) px = 0;
	if(py == MwDEFAULT) py = 0;
	if(width < 2) width = 2;
	if(height < 2) height = 2;

	if(parent == NULL) {
		r->x11.display = XOpenDisplay(NULL);
		p	       = XRootWindow(r->x11.display, XDefaultScreen(r->x11.display));
		r->x11.top     = 1;
	} else {
		r->x11.display = parent->x11.display;
		p	       = parent->x11.window;
		r->x11.top     = 0;
	}
	r->x11.window  = XCreateSimpleWindow(r->x11.display, p, px, py, width, height, 0, 0, WhitePixel(r->x11.display, DefaultScreen(r->x11.display)));
	sh.flags       = PWinGravity;
	sh.win_gravity = StaticGravity;
	XSetWMNormalHints(r->x11.display, r->x11.window, &sh);

	xvi = get_visual_info(r->x11.display);

	if(xvi->red_mask != 0) {
		i = 0;
		while(!((n << i) & xvi->red_mask)) i++;
		r->x11.red_mask	 = xvi->red_mask;
		r->x11.red_max	 = xvi->red_mask >> i;
		r->x11.red_shift = i;

		i = 0;
		while(!((n << i) & xvi->green_mask)) i++;
		r->x11.green_mask  = xvi->green_mask;
		r->x11.green_max   = xvi->green_mask >> i;
		r->x11.green_shift = i;

		i = 0;
		while(!((n << i) & xvi->blue_mask)) i++;
		r->x11.blue_mask  = xvi->blue_mask;
		r->x11.blue_max	  = xvi->blue_mask >> i;
		r->x11.blue_shift = i;
	}

	XFree(xvi);

	XSetLocaleModifiers("");
	if((r->x11.xim = XOpenIM(r->x11.display, 0, 0, 0)) == NULL) {
		XSetLocaleModifiers("@im=none");
		r->x11.xim = XOpenIM(r->x11.display, 0, 0, 0);
	}

	r->x11.xic = XCreateIC(r->x11.xim,
			       XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
			       XNClientWindow, r->x11.window,
			       XNFocusWindow, r->x11.window,
			       NULL);
	XSetICFocus(r->x11.xic);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendX11;

	r->x11.width  = width;
	r->x11.height = height;

	r->x11.grabbed	    = 0;
	r->x11.force_render = 0;

	r->x11.colormap	 = DefaultColormap(r->x11.display, XDefaultScreen(r->x11.display));
	r->x11.wm_delete = XInternAtom(r->x11.display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(r->x11.display, r->x11.window, &r->x11.wm_delete, 1);

	r->x11.gc = XCreateGC(r->x11.display, r->x11.window, 0, NULL);

	create_pixmap(r);

	XSetGraphicsExposures(r->x11.display, r->x11.gc, False);

	XSelectInput(r->x11.display, r->x11.window, mask);

	wait_map(r, 0);

	if(x != MwDEFAULT || y != MwDEFAULT) {
		unsigned int dummy;

		MwLLGetXYWH(r, &px, &py, &dummy, &dummy);

		if(x == MwDEFAULT) x = px;
		if(y == MwDEFAULT) y = py;

		if(parent == NULL) {
			sync_move(r, x, y);
		} else {
			MwLLSetXY(r, x, y);
		}
	}

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	if(handle->x11.xic) XDestroyIC(handle->x11.xic);
	if(handle->x11.xim) XCloseIM(handle->x11.xim);

	destroy_pixmap(handle);
	XFreeGC(handle->x11.display, handle->x11.gc);
	XUnmapWindow(handle->x11.display, handle->x11.window);
	XDestroyWindow(handle->x11.display, handle->x11.window);

	XSync(handle->x11.display, False);

	free(handle);
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int	i;
	XPoint* p = malloc(sizeof(*p) * points_count);

	XSetForeground(handle->x11.display, handle->x11.gc, color->x11.pixel);

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	XFillPolygon(handle->x11.display, handle->x11.pixmap, handle->x11.gc, p, points_count, Nonconvex, CoordModeOrigin);

	free(p);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	XSetForeground(handle->x11.display, handle->x11.gc, color->x11.pixel);

	XDrawLine(handle->x11.display, handle->x11.pixmap, handle->x11.gc, points[0].x, points[0].y, points[1].x, points[1].y);
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	XColor xc;

	if(handle->x11.red_mask == 0) {
		if(r > 255) r = 255;
		if(g > 255) g = 255;
		if(b > 255) b = 255;
		if(r < 0) r = 0;
		if(g < 0) g = 0;
		if(b < 0) b = 0;

		xc.red	 = 256 * r;
		xc.green = 256 * g;
		xc.blue	 = 256 * b;
		XAllocColor(handle->x11.display, handle->x11.colormap, &xc);

		c->x11.pixel = xc.pixel;
	} else {
		c->x11.pixel = generate_color(handle, r, g, b);
	}
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Window	     root;
	unsigned int border, depth;

	XGetGeometry(handle->x11.display, handle->x11.window, &root, x, y, w, h, &border, &depth);
	if(handle->x11.top) {
		int    rx, ry;
		Window child;

		XTranslateCoordinates(handle->x11.display, handle->x11.window, root, 0, 0, &rx, &ry, &child);

		*x += rx;
		*y += ry;
	}
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	XSizeHints	  sh;
	long		  r;
	XWindowChanges	  xwc;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.x == x && xwa.y == y) return;

	sh.flags = 0;
	XGetWMNormalHints(handle->x11.display, handle->x11.window, &sh, &r);

	sh.flags |= USPosition;
	sh.x = x;
	sh.y = y;

#if 1
	xwc.x = x;
	xwc.y = y;
	XConfigureWindow(handle->x11.display, handle->x11.window, CWX | CWY, &xwc);
#else
	XMoveWindow(handle->x11.display, handle->x11.window, x, y);
#endif
	XSetWMNormalHints(handle->x11.display, handle->x11.window, &sh);

	XSync(handle->x11.display, False);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	XSizeHints	  sh;
	long		  r;
	XWindowChanges	  xwc;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.width == w && xwa.height == h) return;

	sh.flags = 0;
	XGetWMNormalHints(handle->x11.display, handle->x11.window, &sh, &r);

	if(w < 2) w = 2;
	if(h < 2) h = 2;

	sh.flags |= PSize;
	sh.width  = w;
	sh.height = h;

#if 1
	xwc.width  = w;
	xwc.height = h;
	XConfigureWindow(handle->x11.display, handle->x11.window, CWWidth | CWHeight, &xwc);
#else
	XResizeWindow(handle->x11.display, handle->x11.window, w, h);
#endif
	XSetWMNormalHints(handle->x11.display, handle->x11.window, &sh);

	destroy_pixmap(handle);
	create_pixmap(handle);

	handle->x11.width  = w;
	handle->x11.height = h;

	XSync(handle->x11.display, False);

	MwLLForceRender(handle);
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static void MwLLSetBackgroundImpl(MwLL handle, MwLLColor color) {
	XSetWindowBackground(handle->x11.display, handle->x11.window, color->x11.pixel);
}

static int MwLLPendingImpl(MwLL handle) {
	XEvent ev;
	if(XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, ClientMessage, &ev) || XCheckWindowEvent(handle->x11.display, handle->x11.window, mask, &ev)) {
		XPutBackEvent(handle->x11.display, &ev);
		return 1;
	}
	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	XEvent ev;
	while(XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, ClientMessage, &ev) || XCheckWindowEvent(handle->x11.display, handle->x11.window, mask, &ev)) {
		int render = 0;
		if(ev.type == Expose) {
			handle->x11.force_render = 0;
			render			 = 1;
		} else if(ev.type == ButtonPress) {
			MwLLMouse p;
			p.point.x = ev.xbutton.x;
			p.point.y = ev.xbutton.y;
			if(ev.xbutton.button == Button1) {
				p.button = MwLLMouseLeft;
			} else if(ev.xbutton.button == Button2) {
				p.button = MwLLMouseMiddle;
			} else if(ev.xbutton.button == Button3) {
				p.button = MwLLMouseRight;
			} else if(ev.xbutton.button == Button4) {
				p.button = MwLLMouseWheelUp;
			} else if(ev.xbutton.button == Button5) {
				p.button = MwLLMouseWheelDown;
			}

			XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);

			MwLLDispatch(handle, down, &p);
		} else if(ev.type == ButtonRelease) {
			MwLLMouse p;
			p.point.x = ev.xbutton.x;
			p.point.y = ev.xbutton.y;
			if(ev.xbutton.button == Button1) {
				p.button = MwLLMouseLeft;
			} else if(ev.xbutton.button == Button2) {
				p.button = MwLLMouseMiddle;
			} else if(ev.xbutton.button == Button3) {
				p.button = MwLLMouseRight;
			} else if(ev.xbutton.button == Button4) {
				p.button = MwLLMouseWheelUp;
			} else if(ev.xbutton.button == Button5) {
				p.button = MwLLMouseWheelDown;
			}

			MwLLDispatch(handle, up, &p);
		} else if(ev.type == ConfigureNotify) {
			if(handle->x11.width != (unsigned int)ev.xconfigure.width || handle->x11.height != (unsigned int)ev.xconfigure.height) {
				MwLLDispatch(handle, resize, NULL);
				destroy_pixmap(handle);
				create_pixmap(handle);
				render = 1;
			}
			handle->x11.width  = ev.xconfigure.width;
			handle->x11.height = ev.xconfigure.height;
		} else if(ev.type == ClientMessage) {
			if(ev.xclient.data.l[0] == (long)handle->x11.wm_delete) {
				MwLLDispatch(handle, close, NULL);
			}
		} else if(ev.type == FocusIn) {
			MwLLDispatch(handle, focus_in, NULL);
		} else if(ev.type == FocusOut) {
			MwLLDispatch(handle, focus_out, NULL);
		} else if(ev.type == MotionNotify) {
			MwPoint		  p;
			XWindowAttributes attr;

			XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

			p.x = ev.xmotion.x;
			p.y = ev.xmotion.y;

			if(handle->x11.grabbed) {
				p.x -= attr.width / 2;
				p.y -= attr.height / 2;
			}

			MwLLDispatch(handle, move, &p);
			if(handle->x11.grabbed && (p.x != 0 || p.y != 0)) {
				XWarpPointer(handle->x11.display, None, handle->x11.window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
			}
		} else if(ev.type == KeyPress || ev.type == KeyRelease) {
			int    n = -1;
			char   str[512];
			KeySym sym;

			str[0] = 0;

			XLookupString(&ev.xkey, str, 512, &sym, NULL);

			/* wtf is wrong with xlib? */
			if(strlen(str) == 0 || (str[0] < 0x20)) {
				char* s = XKeysymToString(sym);

				strcpy(str, s);
			}

			/* HACK: this is bad, you can guess why */
			if(strlen(str) == 1) {
				char s = str[0];

				if(ev.xkey.state & (ShiftMask | LockMask) && !(ev.xkey.state & (ControlMask | Mod1Mask))) {
					n = toupper((int)s);
				} else {
					n = s;
				}
			} else if(strcmp(str, "BackSpace") == 0) {
				n = MwLLKeyBackSpace;
			} else if(strcmp(str, "Left") == 0) {
				n = MwLLKeyLeft;
			} else if(strcmp(str, "Right") == 0) {
				n = MwLLKeyRight;
			} else if(strcmp(str, "Up") == 0) {
				n = MwLLKeyUp;
			} else if(strcmp(str, "Down") == 0) {
				n = MwLLKeyDown;
			} else if(strcmp(str, "Return") == 0) {
				n = MwLLKeyEnter;
			} else if(strcmp(str, "Escape") == 0) {
				n = MwLLKeyEscape;
			} else if(strcmp(str, "Shift_L") == 0) {
				n = MwLLKeyLeftShift;
			} else if(strcmp(str, "Shift_R") == 0) {
				n = MwLLKeyRightShift;
			} else if(strcmp(str, "Alt_L") == 0 || strcmp(str, "Alt_R") == 0) {
				n = MwLLKeyAlt;
			} else if(strcmp(str, "Control_R") == 0 || strcmp(str, "Control_R") == 0) {
				n = MwLLKeyControl;
			}

			if(n != MwLLKeyControl && ev.xkey.state & ControlMask) {
				n |= MwLLControlMask;
			}
			if(n != MwLLKeyAlt && ev.xkey.state & Mod1Mask) {
				n |= MwLLAltMask;
			}

			if(n != -1) {
				if(ev.type == KeyPress) {
					MwLLDispatch(handle, key, &n);
				} else {
					MwLLDispatch(handle, key_released, &n);
				}
			}
		}
		if(render) {
			int	     x, y;
			unsigned int w, h;

			MwLLGetXYWH(handle, &x, &y, &w, &h);

			MwLLDispatch(handle, draw, NULL);
			if(handle->common.copy_buffer) {
				XCopyArea(handle->x11.display, handle->x11.pixmap, handle->x11.window, handle->x11.gc, 0, 0, w, h, 0, 0);
				XSetWindowBackgroundPixmap(handle->x11.display, handle->x11.window, handle->x11.pixmap);
			}
		}
	}
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	XSetStandardProperties(handle->x11.display, handle->x11.window, title, title, None, (char**)NULL, 0, NULL);
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap	  r  = malloc(sizeof(*r));
	char*		  di = malloc(4 * width * height);
	char*		  dm = malloc(4 * width * height);
	int		  evbase, erbase;
	XWindowAttributes attr;

	r->common.raw = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	r->common.width	 = width;
	r->common.height = height;

	r->x11.depth   = attr.depth;
	r->x11.display = handle->x11.display;
	r->x11.data    = malloc(sizeof(unsigned long) * width * height);
	r->x11.handle  = handle;

#ifdef USE_XRENDER
	r->x11.use_xrender = XRenderQueryExtension(handle->x11.display, &evbase, &erbase) ? 1 : 0;
#endif

	r->x11.image = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), r->x11.depth, ZPixmap, 0, di, width, height, 32, width * 4);
	r->x11.mask  = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, width, height, 32, width * 4);

	MwLLPixmapUpdate(r);
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	int y, x;
	for(y = 0; y < r->common.height; y++) {
		for(x = 0; x < r->common.width; x++) {
			unsigned char* px = &r->common.raw[(y * r->common.width + x) * 4];
			MwLLColor      c  = NULL;
			unsigned long  p;

			c = MwLLAllocColor(r->x11.handle, px[0], px[1], px[2]);
			p = c->x11.pixel;
			MwLLFreeColor(c);

			XPutPixel(r->x11.image, x, y, p);
			*(unsigned long*)(&r->x11.data[(y * r->common.width + x) * sizeof(unsigned long)]) = (px[3] << 24) | p;
		}
	}

	for(y = 0; y < r->common.height; y++) {
		for(x = 0; x < r->common.width; x++) {
			if(r->common.raw[(y * r->common.width + x) * 4 + 3]) {
				XPutPixel(r->x11.mask, x, y, 1);
			} else {
				XPutPixel(r->x11.mask, x, y, 0);
			}
		}
	}
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap->common.raw);
	XDestroyImage(pixmap->x11.image);
	XDestroyImage(pixmap->x11.mask);
	free(pixmap->x11.data);

	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	if(rect->width == 0 || rect->height == 0 || pixmap->common.width == 0 || pixmap->common.height == 0) return;
#ifdef USE_XRENDER
	if(pixmap->x11.image != NULL && pixmap->x11.use_xrender) {
		Pixmap			 px	= XCreatePixmap(handle->x11.display, handle->x11.window, pixmap->common.width, pixmap->common.height, pixmap->x11.depth);
		Pixmap			 mask	= XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, 1);
		Pixmap			 pxsrc	= XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, pixmap->x11.depth);
		GC			 maskgc = XCreateGC(handle->x11.display, mask, 0, NULL);
		XRenderPictFormat*	 format = XRenderFindStandardFormat(handle->x11.display, PictStandardRGB24);
		XRenderPictureAttributes attr;
		Picture			 src, dest;
		XTransform		 m;
		double			 xsc = (double)pixmap->common.width / rect->width;
		double			 ysc = (double)pixmap->common.height / rect->height;
		char*			 dm  = malloc(rect->width * rect->height * 4);
		XImage*			 destmask;
		int			 y, x;

		destmask = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int sy = y * pixmap->common.height / rect->height;
				int sx = x * pixmap->common.width / rect->width;
				sy     = (int)sy;
				sx     = (int)sx;

				XPutPixel(destmask, x, y, XGetPixel(pixmap->x11.mask, sx, sy));
			}
		}

		XPutImage(handle->x11.display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

		m.matrix[0][0] = XDoubleToFixed(xsc);
		m.matrix[0][1] = XDoubleToFixed(0);
		m.matrix[0][2] = XDoubleToFixed(0);

		m.matrix[1][0] = XDoubleToFixed(0);
		m.matrix[1][1] = XDoubleToFixed(ysc);
		m.matrix[1][2] = XDoubleToFixed(0);

		m.matrix[2][0] = XDoubleToFixed(0);
		m.matrix[2][1] = XDoubleToFixed(0);
		m.matrix[2][2] = XDoubleToFixed(1.0);

		memset(&attr, 0, sizeof(attr));

		XPutImage(handle->x11.display, px, handle->x11.gc, pixmap->x11.image, 0, 0, 0, 0, pixmap->common.width, pixmap->common.height);

		src  = XRenderCreatePicture(handle->x11.display, px, format, 0, &attr);
		dest = XRenderCreatePicture(handle->x11.display, pxsrc, format, 0, &attr);

		XRenderSetPictureTransform(handle->x11.display, src, &m);
		XRenderComposite(handle->x11.display, PictOpSrc, src, 0, dest, 0, 0, 0, 0, 0, 0, rect->width, rect->height);

		XRenderFreePicture(handle->x11.display, src);
		XRenderFreePicture(handle->x11.display, dest);

		XSetClipMask(handle->x11.display, handle->x11.gc, mask);
		XSetClipOrigin(handle->x11.display, handle->x11.gc, rect->x, rect->y);
		XCopyArea(handle->x11.display, pxsrc, handle->x11.pixmap, handle->x11.gc, 0, 0, rect->width, rect->height, rect->x, rect->y);
		XSetClipMask(handle->x11.display, handle->x11.gc, None);

		XDestroyImage(destmask);

		XFreeGC(handle->x11.display, maskgc);
		XFreePixmap(handle->x11.display, mask);
		XFreePixmap(handle->x11.display, px);
		XFreePixmap(handle->x11.display, pxsrc);
	} else
#endif
	    if(pixmap->x11.image != NULL) {
		XImage* dest;
		XImage* destmask;
		Pixmap	mask   = XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, 1);
		GC	maskgc = XCreateGC(handle->x11.display, mask, 0, NULL);
		char*	di     = malloc(rect->width * rect->height * 4);
		char*	dm     = malloc(rect->width * rect->height * 4);
		int	y, x;

		dest	 = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), pixmap->x11.depth, ZPixmap, 0, di, rect->width, rect->height, 32, rect->width * 4);
		destmask = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int   sy = y * pixmap->common.height / rect->height;
				int   sx = x * pixmap->common.width / rect->width;
				char* ipx;
				char* opx;
				sy = (int)sy;
				sx = (int)sx;

				ipx = &pixmap->x11.image->data[(pixmap->common.width * sy + sx) * (pixmap->x11.image->bitmap_unit / 8)];
				opx = &di[(rect->width * y + x) * (pixmap->x11.image->bitmap_unit / 8)];
				memcpy(opx, ipx, pixmap->x11.image->bitmap_unit / 8);

				XPutPixel(destmask, x, y, XGetPixel(pixmap->x11.mask, sx, sy));
			}
		}

		XPutImage(handle->x11.display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

		XSetClipMask(handle->x11.display, handle->x11.gc, mask);
		XSetClipOrigin(handle->x11.display, handle->x11.gc, rect->x, rect->y);
		XPutImage(handle->x11.display, handle->x11.pixmap, handle->x11.gc, dest, 0, 0, rect->x, rect->y, rect->width, rect->height);
		XSetClipMask(handle->x11.display, handle->x11.gc, None);

		XDestroyImage(dest);
		XDestroyImage(destmask);

		XFreeGC(handle->x11.display, maskgc);
		XFreePixmap(handle->x11.display, mask);
	}
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	unsigned long* icon = malloc((2 + pixmap->common.width * pixmap->common.height) * sizeof(*icon));
	int	       i;
	Atom	       atom = XInternAtom(handle->x11.display, "_NET_WM_ICON", False);

	icon[0] = pixmap->common.width;
	icon[1] = pixmap->common.height;

	for(i = 0; i < pixmap->common.width * pixmap->common.height; i++) {
		icon[2 + i] = *(unsigned long*)(&pixmap->x11.data[i * sizeof(unsigned long)]);
	}

	XChangeProperty(handle->x11.display, handle->x11.window, atom, 6, 32, PropModeReplace, (unsigned char*)icon, 2 + pixmap->common.width * pixmap->common.height);

	free(icon);
}

static void MwLLForceRenderImpl(MwLL handle) {
	XEvent ev;
	memset(&ev, 0, sizeof(ev));

	if(!handle->x11.force_render) {
		ev.type		  = Expose;
		ev.xexpose.window = handle->x11.window;
		XSendEvent(handle->x11.display, handle->x11.window, False, ExposureMask, &ev);

		handle->x11.force_render = 1;
	}
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	Cursor	cur;
	int	y, x, ys, xs;
	char*	di	= malloc(MwCursorDataHeight * MwCursorDataHeight * 4);
	char*	dm	= malloc(MwCursorDataHeight * MwCursorDataHeight * 4);
	XImage* cimage	= XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, di, MwCursorDataHeight, MwCursorDataHeight, 32, MwCursorDataHeight * 4);
	XImage* cmask	= XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, MwCursorDataHeight, MwCursorDataHeight, 32, MwCursorDataHeight * 4);
	Pixmap	pimage	= XCreatePixmap(handle->x11.display, handle->x11.window, MwCursorDataHeight, MwCursorDataHeight, 1);
	Pixmap	pmask	= XCreatePixmap(handle->x11.display, handle->x11.window, MwCursorDataHeight, MwCursorDataHeight, 1);
	GC	imagegc = XCreateGC(handle->x11.display, pimage, 0, NULL);
	GC	maskgc	= XCreateGC(handle->x11.display, pmask, 0, NULL);
	XColor	cfg, cbg;

	xs = -mask->x + image->x;
	ys = MwCursorDataHeight + mask->y;
	ys = MwCursorDataHeight + image->y - ys;

	memset(cimage->data, 0, cimage->bytes_per_line * cimage->height);
	memset(cmask->data, 0, cmask->bytes_per_line * cmask->height);
	for(y = 0; y < mask->height; y++) {
		unsigned int l = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			if(l & 1) {
				XPutPixel(cmask, x, y, 1);
			}
			l = l >> 1;
		}
	}
	for(y = 0; y < image->height; y++) {
		unsigned int l = image->data[y];
		for(x = image->width - 1; x >= 0; x--) {
			int px = 0;
			if(l & 1) px = 1;
			XPutPixel(cimage, xs + x, ys + y, px);

			l = l >> 1;
		}
	}

	cfg.red	  = 65535;
	cfg.green = 65535;
	cfg.blue  = 65535;
	XAllocColor(handle->x11.display, handle->x11.colormap, &cfg);

	cbg.red	  = 0;
	cbg.green = 0;
	cbg.blue  = 0;
	XAllocColor(handle->x11.display, handle->x11.colormap, &cbg);

	XPutImage(handle->x11.display, pimage, imagegc, cimage, 0, 0, 0, 0, MwCursorDataHeight, MwCursorDataHeight);
	XPutImage(handle->x11.display, pmask, maskgc, cmask, 0, 0, 0, 0, MwCursorDataHeight, MwCursorDataHeight);

	cur = XCreatePixmapCursor(handle->x11.display, pimage, pmask, &cfg, &cbg, xs, ys);
	XDefineCursor(handle->x11.display, handle->x11.window, cur);
	XFreeCursor(handle->x11.display, cur);

	XFreePixmap(handle->x11.display, pimage);
	XFreePixmap(handle->x11.display, pmask);

	XDestroyImage(cimage);
	XDestroyImage(cmask);
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	int		  x = 0, y = 0;
	Window		  child, root, parent;
	Window*		  children;
	unsigned int	  nchild;
	XWindowAttributes xwa;

	handle->x11.top = 1;

	XQueryTree(handle->x11.display, handle->x11.window, &root, &parent, &children, &nchild);
	if(children != NULL) XFree(children);

	XTranslateCoordinates(handle->x11.display, parent, RootWindow(handle->x11.display, DefaultScreen(handle->x11.display)), 0, 0, &x, &y, &child);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);

	XReparentWindow(handle->x11.display, handle->x11.window, RootWindow(handle->x11.display, DefaultScreen(handle->x11.display)), x + point->x, y + point->y);

	if(xwa.map_state == IsViewable) {
		sync_move(handle, x + point->x, y + point->y);
	}
}

static void MwLLShowImpl(MwLL handle, int show) {
	if(show) {
		wait_map(handle, 1);

		XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);
	} else {
		wait_unmap(handle, 1);
	}
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	Atom wndtype  = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE", False);
	Atom wnddlg   = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	Atom wndstate = XInternAtom(handle->x11.display, "_NET_WM_STATE", False);
	Atom wndmodal = XInternAtom(handle->x11.display, "_NET_WM_STATE_MODAL", False);

	if(parent != NULL) XSetTransientForHint(handle->x11.display, handle->x11.window, parent->x11.window);
	XChangeProperty(handle->x11.display, handle->x11.window, wndtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wnddlg, 1);
	XChangeProperty(handle->x11.display, handle->x11.window, wndstate, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wndmodal, 1);
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	XSizeHints* hints = XAllocSizeHints();
	long	    ret;

	XGetWMSizeHints(handle->x11.display, handle->x11.window, hints, &ret, XA_WM_NORMAL_HINTS);

	hints->flags |= PMinSize | PMaxSize;
	hints->min_width  = minx;
	hints->min_height = miny;
	hints->max_width  = maxx;
	hints->max_height = maxy;
	XSetWMSizeHints(handle->x11.display, handle->x11.window, hints, XA_WM_NORMAL_HINTS);
	XFree(hints);
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	Atom	    atom = XInternAtom(handle->x11.display, "_MOTIF_WM_HINTS", 0);
	mwm_hints_t hints;

	hints.flags	  = MWM_HINTS_DECORATIONS;
	hints.decorations = toggle ? 0 : 1;
	XChangeProperty(handle->x11.display, handle->x11.window, atom, atom, 32, PropModeReplace, (unsigned char*)&hints, 5);
}

static void MwLLFocusImpl(MwLL handle) {
	XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	XWindowAttributes attr;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	if(toggle) {
		handle->x11.grabbed = 1;

		XWarpPointer(handle->x11.display, None, handle->x11.window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
	} else {
		handle->x11.grabbed = 0;
	}
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	/* TODO */

	(void)handle;
	(void)text;
}

static char* MwLLGetClipboardImpl(MwLL handle) {
	Atom	clip, target, prop;
	XEvent	ev;
	XEvent* queue = NULL;
	char*	r     = NULL;

	clip   = XInternAtom(handle->x11.display, "CLIPBOARD", 0);
	target = XA_STRING;
	prop   = XInternAtom(handle->x11.display, "XSEL_DATA", 0);

	XConvertSelection(handle->x11.display, clip, target, prop, handle->x11.window, CurrentTime);

	while(1) {
		XNextEvent(handle->x11.display, &ev);
		if(ev.type == SelectionNotify) {
			if(ev.xselection.selection == clip && ev.xselection.property != 0) {
				Atom	      t;
				unsigned long size, N;
				char*	      data;
				int	      format;

				XGetWindowProperty(ev.xselection.display, ev.xselection.requestor, ev.xselection.property, 0, (~0L), 0, AnyPropertyType, &t, &format, &size, &N, (unsigned char**)&data);
				if(t == target) {
					r = MwStringDuplicate(data);
					XFree(data);
				}
				XDeleteProperty(ev.xselection.display, ev.xselection.requestor, ev.xselection.property);
			}
			break;
		}
	}

	while(arrlen(queue) > 0) {
		XPutBackEvent(handle->x11.display, &queue[0]);
		arrdel(queue, 0);
	}
	arrfree(queue);

	return r;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	XSetWindowAttributes xswa;
	Atom		     wndtype = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE", False);
	Atom		     wndmenu = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE_MENU", False);

	xswa.override_redirect = True;

	XChangeWindowAttributes(handle->x11.display, handle->x11.window, CWOverrideRedirect, &xswa);
	XChangeProperty(handle->x11.display, handle->x11.window, wndtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wndmenu, 1);
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	Window	     root, child;
	int	     rx, ry, wx, wy;
	unsigned int m;

	XQueryPointer(handle->x11.display, DefaultRootWindow(handle->x11.display), &root, &child, &rx, &ry, &wx, &wy, &m);

	point->x = rx;
	point->y = ry;
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static int MwLLX11CallInitImpl(void) {
	/* TODO: check properly */
	return 0;
}

#include "call.c"
CALL(X11);
