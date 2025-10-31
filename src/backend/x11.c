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

	XGetWindowAttributes(handle->display, handle->window, &attr);

	handle->pixmap = XCreatePixmap(handle->display, handle->window, w, h, attr.depth);
}

static void destroy_pixmap(MwLL handle) {
	XFreePixmap(handle->display, handle->pixmap);
}

static void wait_map(MwLL handle) {
	XEvent* queue = NULL;
	XEvent	ev;
	while(1) {
		XNextEvent(handle->display, &ev);
		if(ev.type == MapNotify && ev.xmap.window == handle->window) {
			break;
		} else {
			arrput(queue, ev);
		}
	}

	while(arrlen(queue) > 0) {
		XPutBackEvent(handle->display, &queue[0]);
		arrdel(queue, 0);
	}
	arrfree(queue);
}

static unsigned long generate_color(MwLL handle, unsigned long r, unsigned long g, unsigned long b) {
	unsigned long c = 0;

	c |= (r * handle->red_max / 255) << handle->red_shift;

	c |= (g * handle->green_max / 255) << handle->green_shift;

	c |= (b * handle->blue_max / 255) << handle->blue_shift;

	return c;
}

static XVisualInfo* get_visual_info(Display* display) {
	XVisualInfo xvi;
	int	    n;
	Visual*	    visual = DefaultVisual(display, DefaultScreen(display));

	xvi.visualid = XVisualIDFromVisual(visual);

	return XGetVisualInfo(display, VisualIDMask, &xvi, &n);
}

MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height) {
	MwLL	      r;
	Window	      p;
	XVisualInfo*  xvi;
	unsigned long n = 1;
	int	      i;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

	if(x == MwDEFAULT) x = 0;
	if(y == MwDEFAULT) y = 0;
	if(width < 1) width = 1;
	if(height < 1) height = 1;

	if(parent == NULL) {
		r->display = XOpenDisplay(NULL);
		p	   = XRootWindow(r->display, XDefaultScreen(r->display));
	} else {
		r->display = parent->display;
		p	   = parent->window;
	}
	r->window = XCreateSimpleWindow(r->display, p, x, y, width, height, 0, 0, WhitePixel(r->display, DefaultScreen(r->display)));

	xvi = get_visual_info(r->display);

	if(xvi->red_mask != 0) {
		i = 0;
		while(!((n << i) & xvi->red_mask)) i++;
		r->red_mask  = xvi->red_mask;
		r->red_max   = xvi->red_mask >> i;
		r->red_shift = i;

		i = 0;
		while(!((n << i) & xvi->green_mask)) i++;
		r->green_mask  = xvi->green_mask;
		r->green_max   = xvi->green_mask >> i;
		r->green_shift = i;

		i = 0;
		while(!((n << i) & xvi->blue_mask)) i++;
		r->blue_mask  = xvi->blue_mask;
		r->blue_max   = xvi->blue_mask >> i;
		r->blue_shift = i;
	}

	XFree(xvi);

	XSetLocaleModifiers("");
	if((r->xim = XOpenIM(r->display, 0, 0, 0)) == NULL) {
		XSetLocaleModifiers("@im=none");
		r->xim = XOpenIM(r->display, 0, 0, 0);
	}

	r->xic = XCreateIC(r->xim,
			   XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
			   XNClientWindow, r->window,
			   XNFocusWindow, r->window,
			   NULL);
	XSetICFocus(r->xic);

	r->copy_buffer = 1;

	r->width  = width;
	r->height = height;

	r->grabbed = 0;

	r->colormap  = DefaultColormap(r->display, XDefaultScreen(r->display));
	r->wm_delete = XInternAtom(r->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(r->display, r->window, &r->wm_delete, 1);

	r->gc = XCreateGC(r->display, r->window, 0, NULL);

	create_pixmap(r);

	XSetGraphicsExposures(r->display, r->gc, False);

	XSelectInput(r->display, r->window, mask);
	XMapWindow(r->display, r->window);

	XFlush(r->display);
	XSync(r->display, False);
	wait_map(r);

	return r;
}

void MwLLDestroy(MwLL handle) {
	MwLLDestroyCommon(handle);

	if(handle->xic) XDestroyIC(handle->xic);
	if(handle->xim) XCloseIM(handle->xim);

	destroy_pixmap(handle);
	XFreeGC(handle->display, handle->gc);
	XUnmapWindow(handle->display, handle->window);
	XDestroyWindow(handle->display, handle->window);

	XFlush(handle->display);
	free(handle);
}

void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int	i;
	XPoint* p = malloc(sizeof(*p) * points_count);

	XSetForeground(handle->display, handle->gc, color->pixel);

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	XFillPolygon(handle->display, handle->pixmap, handle->gc, p, points_count, Nonconvex, CoordModeOrigin);

	free(p);
}

void MwLLLine(MwLL handle, MwPoint* points, MwLLColor color) {
	XSetForeground(handle->display, handle->gc, color->pixel);

	XDrawLine(handle->display, handle->pixmap, handle->gc, points[0].x, points[0].y, points[1].x, points[1].y);
}

MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	XColor	  xc;

	if(handle->red_mask == 0) {
		if(r > 255) r = 255;
		if(g > 255) g = 255;
		if(b > 255) b = 255;
		if(r < 0) r = 0;
		if(g < 0) g = 0;
		if(b < 0) b = 0;

		xc.red	 = 256 * r;
		xc.green = 256 * g;
		xc.blue	 = 256 * b;
		XAllocColor(handle->display, handle->colormap, &xc);

		c->pixel = xc.pixel;
	} else {
		c->pixel = generate_color(handle, r, g, b);
	}
	c->red	 = r;
	c->green = g;
	c->blue	 = b;
	return c;
}

void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Window	     root;
	unsigned int border, depth;

	XGetGeometry(handle->display, handle->window, &root, x, y, w, h, &border, &depth);
}

void MwLLSetXY(MwLL handle, int x, int y) {
	XMoveWindow(handle->display, handle->window, x, y);
}

void MwLLSetWH(MwLL handle, int w, int h) {
	if(w < 1) w = 1;
	if(h < 1) h = 1;

	XResizeWindow(handle->display, handle->window, w, h);
}

void MwLLFreeColor(MwLLColor color) {
	free(color);
}

void MwLLSetBackground(MwLL handle, MwLLColor color) {
	XSetWindowBackground(handle->display, handle->window, color->pixel);
}

int MwLLPending(MwLL handle) {
	XEvent ev;
	if(XCheckTypedWindowEvent(handle->display, handle->window, ClientMessage, &ev) || XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		XPutBackEvent(handle->display, &ev);
		return 1;
	}
	return 0;
}

void MwLLNextEvent(MwLL handle) {
	XEvent ev;
	while(XCheckTypedWindowEvent(handle->display, handle->window, ClientMessage, &ev) || XCheckWindowEvent(handle->display, handle->window, mask, &ev)) {
		int render = 0;
		if(ev.type == Expose) {
			render = 1;
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

			XSetInputFocus(handle->display, handle->window, RevertToNone, CurrentTime);

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
			MwLLDispatch(handle, resize, NULL);

			if(handle->width != (unsigned int)ev.xconfigure.width || handle->height != (unsigned int)ev.xconfigure.height) {
				destroy_pixmap(handle);
				create_pixmap(handle);
				render = 1;
			}
			handle->width  = ev.xconfigure.width;
			handle->height = ev.xconfigure.height;
		} else if(ev.type == ClientMessage) {
			if(ev.xclient.data.l[0] == (long)handle->wm_delete) {
				MwLLDispatch(handle, close, NULL);
			}
		} else if(ev.type == FocusIn) {
			MwLLDispatch(handle, focus_in, NULL);
		} else if(ev.type == FocusOut) {
			MwLLDispatch(handle, focus_out, NULL);
		} else if(ev.type == MotionNotify) {
			MwPoint		  p;
			XWindowAttributes attr;

			XGetWindowAttributes(handle->display, handle->window, &attr);

			p.x = ev.xmotion.x;
			p.y = ev.xmotion.y;

			if(handle->grabbed) {
				p.x -= attr.width / 2;
				p.y -= attr.height / 2;
			}

			MwLLDispatch(handle, move, &p);
			if(handle->grabbed && (p.x != 0 || p.y != 0)) {
				XWarpPointer(handle->display, None, handle->window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
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

				if(ev.xkey.state & (ShiftMask | LockMask)) {
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
			if(handle->copy_buffer) {
				XCopyArea(handle->display, handle->pixmap, handle->window, handle->gc, 0, 0, w, h, 0, 0);
				XSetWindowBackgroundPixmap(handle->display, handle->window, handle->pixmap);
			}
		}
	}
}

void MwLLSleep(int ms) {
	struct timespec ts;

	ts.tv_sec  = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000 * 1000;

	nanosleep(&ts, NULL);
}

void MwLLSetTitle(MwLL handle, const char* title) {
	XSetStandardProperties(handle->display, handle->window, title, title, None, (char**)NULL, 0, NULL);
}

MwLLPixmap MwLLCreatePixmap(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap	  r  = malloc(sizeof(*r));
	char*		  di = malloc(4 * width * height);
	char*		  dm = malloc(4 * width * height);
	int		  y, x;
	int		  evbase, erbase;
	XWindowAttributes attr;

	XGetWindowAttributes(handle->display, handle->window, &attr);

	r->depth   = attr.depth;
	r->width   = width;
	r->height  = height;
	r->display = handle->display;
	r->data	   = malloc(sizeof(unsigned long) * width * height);

	r->use_render = XRenderQueryExtension(handle->display, &evbase, &erbase) ? 1 : 0;

	r->image = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), r->depth, ZPixmap, 0, di, width, height, 32, width * 4);
	r->mask	 = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), 1, ZPixmap, 0, dm, width, height, 32, width * 4);

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			unsigned char* px = &data[(y * width + x) * 4];
			MwLLColor      c  = MwLLAllocColor(handle, px[0], px[1], px[2]);
			unsigned long  p  = c->pixel;

			MwLLFreeColor(c);

			XPutPixel(r->image, x, y, p);
			*(unsigned long*)(&r->data[(y * width + x) * sizeof(unsigned long)]) = (px[3] << 24) | p;
		}
	}
	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			if(data[(y * width + x) * 4 + 3]) {
				XPutPixel(r->mask, x, y, 1);
			} else {
				XPutPixel(r->mask, x, y, 0);
			}
		}
	}

	return r;
}

void MwLLDestroyPixmap(MwLLPixmap pixmap) {
	if(pixmap->image != NULL) {
		XDestroyImage(pixmap->image);
		XDestroyImage(pixmap->mask);
		free(pixmap->data);
	}

	free(pixmap);
}

void MwLLDrawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	if(rect->width == 0 || rect->height == 0) return;
	if(pixmap->image != NULL && pixmap->use_render) {
		Pixmap			 px	= XCreatePixmap(handle->display, handle->window, pixmap->width, pixmap->height, pixmap->depth);
		Pixmap			 mask	= XCreatePixmap(handle->display, handle->window, rect->width, rect->height, 1);
		Pixmap			 pxsrc	= XCreatePixmap(handle->display, handle->window, rect->width, rect->height, pixmap->depth);
		GC			 maskgc = XCreateGC(handle->display, mask, 0, NULL);
		XRenderPictFormat*	 format = XRenderFindStandardFormat(handle->display, PictStandardRGB24);
		XRenderPictureAttributes attr;
		Picture			 src, dest;
		XTransform		 m;
		double			 xsc = (double)pixmap->width / rect->width;
		double			 ysc = (double)pixmap->height / rect->height;
		char*			 dm  = malloc(rect->width * rect->height * 4);
		XImage*			 destmask;
		int			 y, x;

		destmask = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int sy = y * pixmap->height / rect->height;
				int sx = x * pixmap->width / rect->width;
				sy     = (int)sy;
				sx     = (int)sx;

				XPutPixel(destmask, x, y, XGetPixel(pixmap->mask, sx, sy));
			}
		}

		XPutImage(handle->display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

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

		XPutImage(handle->display, px, handle->gc, pixmap->image, 0, 0, 0, 0, pixmap->width, pixmap->height);

		src  = XRenderCreatePicture(handle->display, px, format, 0, &attr);
		dest = XRenderCreatePicture(handle->display, pxsrc, format, 0, &attr);

		XRenderSetPictureTransform(handle->display, src, &m);
		XRenderComposite(handle->display, PictOpSrc, src, 0, dest, 0, 0, 0, 0, 0, 0, rect->width, rect->height);

		XRenderFreePicture(handle->display, src);
		XRenderFreePicture(handle->display, dest);

		XSetClipMask(handle->display, handle->gc, mask);
		XSetClipOrigin(handle->display, handle->gc, rect->x, rect->y);
		XCopyArea(handle->display, pxsrc, handle->pixmap, handle->gc, 0, 0, rect->width, rect->height, rect->x, rect->y);
		XSetClipMask(handle->display, handle->gc, None);

		XDestroyImage(destmask);

		XFreeGC(handle->display, maskgc);
		XFreePixmap(handle->display, mask);
		XFreePixmap(handle->display, px);
		XFreePixmap(handle->display, pxsrc);
	} else if(pixmap->image != NULL) {
		XImage* dest;
		XImage* destmask;
		Pixmap	mask   = XCreatePixmap(handle->display, handle->window, rect->width, rect->height, 1);
		GC	maskgc = XCreateGC(handle->display, mask, 0, NULL);
		char*	di     = malloc(rect->width * rect->height * 4);
		char*	dm     = malloc(rect->width * rect->height * 4);
		int	y, x;

		dest	 = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), pixmap->depth, ZPixmap, 0, di, rect->width, rect->height, 32, rect->width * 4);
		destmask = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int   sy = y * pixmap->height / rect->height;
				int   sx = x * pixmap->width / rect->width;
				char* ipx;
				char* opx;
				sy = (int)sy;
				sx = (int)sx;

				ipx = &pixmap->image->data[(pixmap->width * sy + sx) * (pixmap->image->bitmap_unit / 8)];
				opx = &di[(rect->width * y + x) * (pixmap->image->bitmap_unit / 8)];
				memcpy(opx, ipx, pixmap->image->bitmap_unit / 8);

				XPutPixel(destmask, x, y, XGetPixel(pixmap->mask, sx, sy));
			}
		}

		XPutImage(handle->display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

		XSetClipMask(handle->display, handle->gc, mask);
		XSetClipOrigin(handle->display, handle->gc, rect->x, rect->y);
		XPutImage(handle->display, handle->pixmap, handle->gc, dest, 0, 0, rect->x, rect->y, rect->width, rect->height);
		XSetClipMask(handle->display, handle->gc, None);

		XDestroyImage(dest);
		XDestroyImage(destmask);

		XFreeGC(handle->display, maskgc);
		XFreePixmap(handle->display, mask);
	}
}

void MwLLSetIcon(MwLL handle, MwLLPixmap pixmap) {
	unsigned long* icon = malloc((2 + pixmap->width * pixmap->height) * sizeof(*icon));
	int	       i;
	Atom	       atom = XInternAtom(handle->display, "_NET_WM_ICON", False);

	icon[0] = pixmap->width;
	icon[1] = pixmap->height;

	for(i = 0; i < pixmap->width * pixmap->height; i++) {
		icon[2 + i] = *(unsigned long*)(&pixmap->data[i * sizeof(unsigned long)]);
	}

	XChangeProperty(handle->display, handle->window, atom, 6, 32, PropModeReplace, (unsigned char*)icon, 2 + pixmap->width * pixmap->height);

	free(icon);
}

void MwLLForceRender(MwLL handle) {
	XEvent ev;
	memset(&ev, 0, sizeof(ev));

	ev.type		  = Expose;
	ev.xexpose.window = handle->window;
	XSendEvent(handle->display, handle->window, False, ExposureMask, &ev);
}

void MwLLSetCursor(MwLL handle, MwCursor* image, MwCursor* mask) {
	XcursorImage* img = XcursorImageCreate(MwCursorDataHeight, MwCursorDataHeight);
	Cursor	      cur;
	int	      y, x, ys, xs;

	xs = -mask->x + image->x;
	ys = MwCursorDataHeight + mask->y;
	ys = MwCursorDataHeight + image->y - ys;

	img->xhot = xs;
	img->yhot = ys;

	memset(img->pixels, 0, MwCursorDataHeight * MwCursorDataHeight * sizeof(XcursorPixel));
	for(y = 0; y < mask->height; y++) {
		unsigned int l = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			if(l & 1) {
				img->pixels[y * MwCursorDataHeight + x] = 0xff000000;
			}
			l = l >> 1;
		}
	}
	for(y = 0; y < image->height; y++) {
		unsigned int l = image->data[y];
		for(x = image->width - 1; x >= 0; x--) {
			int px = 0;
			if(l & 1) px = 255;
			img->pixels[(ys + y) * MwCursorDataHeight + (xs + x)] |= (px << 16) | (px << 8) | (px);

			l = l >> 1;
		}
	}

	cur = XcursorImageLoadCursor(handle->display, img);
	XDefineCursor(handle->display, handle->window, cur);
	XFreeCursor(handle->display, cur);

	XcursorImageDestroy(img);
}

void MwLLDetach(MwLL handle, MwPoint* point) {
	int	     x = 0, y = 0;
	Window	     child, root, parent;
	Window*	     children;
	unsigned int nchild;

	XQueryTree(handle->display, handle->window, &root, &parent, &children, &nchild);
	if(children != NULL) XFree(children);

	XTranslateCoordinates(handle->display, parent, RootWindow(handle->display, DefaultScreen(handle->display)), 0, 0, &x, &y, &child);

	XReparentWindow(handle->display, handle->window, RootWindow(handle->display, DefaultScreen(handle->display)), x + point->x, y + point->y);

	XFlush(handle->display);
	XSync(handle->display, False);
}

void MwLLShow(MwLL handle, int show) {
	if(show) {
		XMapWindow(handle->display, handle->window);
		wait_map(handle);

		XSetInputFocus(handle->display, handle->window, RevertToNone, CurrentTime);
	} else {
		XUnmapWindow(handle->display, handle->window);
	}
}

void MwLLMakePopup(MwLL handle, MwLL parent) {
	Atom wndtype  = XInternAtom(handle->display, "_NET_WM_WINDOW_TYPE", False);
	Atom wnddlg   = XInternAtom(handle->display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	Atom wndstate = XInternAtom(handle->display, "_NET_WM_STATE", False);
	Atom wndmodal = XInternAtom(handle->display, "_NET_WM_STATE_MODAL", False);

	XSetTransientForHint(handle->display, handle->window, parent->window);
	XChangeProperty(handle->display, handle->window, wndtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wnddlg, 1);
	XChangeProperty(handle->display, handle->window, wndstate, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wndmodal, 1);
}

void MwLLSetSizeHints(MwLL handle, int minx, int miny, int maxx, int maxy) {
	XSizeHints* hints = XAllocSizeHints();

	hints->flags	  = PMinSize | PMaxSize;
	hints->min_width  = minx;
	hints->min_height = miny;
	hints->max_width  = maxx;
	hints->max_height = maxy;
	XSetWMSizeHints(handle->display, handle->window, hints, XA_WM_NORMAL_HINTS);
	XFree(hints);

	XUnmapWindow(handle->display, handle->window);
	XMapWindow(handle->display, handle->window);

	XFlush(handle->display);
	XSync(handle->display, False);

	wait_map(handle);
}

void MwLLMakeBorderless(MwLL handle, int toggle) {
	Atom	    atom = XInternAtom(handle->display, "_MOTIF_WM_HINTS", 0);
	mwm_hints_t hints;

	hints.flags	  = MWM_HINTS_DECORATIONS;
	hints.decorations = toggle ? 0 : 1;
	XChangeProperty(handle->display, handle->window, atom, atom, 32, PropModeReplace, (unsigned char*)&hints, 5);

	XUnmapWindow(handle->display, handle->window);
	XMapWindow(handle->display, handle->window);

	XFlush(handle->display);
	XSync(handle->display, False);

	wait_map(handle);
}

long MwLLGetTick(void) {
	struct timespec ts;
	long		n = 0;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	n += ts.tv_nsec / 1000 / 1000;
	n += ts.tv_sec * 1000;

	return n;
}

void MwLLFocus(MwLL handle) {
	XSetInputFocus(handle->display, handle->window, RevertToNone, CurrentTime);
}

void MwLLGrabPointer(MwLL handle, int toggle) {
	XWindowAttributes attr;

	XGetWindowAttributes(handle->display, handle->window, &attr);

	if(toggle) {
		handle->grabbed = 1;

		XWarpPointer(handle->display, None, handle->window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
	} else {
		handle->grabbed = 0;
	}
}
