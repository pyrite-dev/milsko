/* $Id$ */
#include <Mw/Milsko.h>

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | KeymapNotify;

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

MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height) {
	MwLL   r;
	Window p;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

	if(width < 1) width = 1;
	if(height < 1) height = 1;

	if(parent == NULL) {
		r->display = XOpenDisplay(NULL);
		p	   = XRootWindow(r->display, XDefaultScreen(r->display));
	} else {
		r->display = parent->display;
		p	   = parent->window;
	}
	r->window      = XCreateSimpleWindow(r->display, p, x, y, width, height, 0, 0, WhitePixel(r->display, XDefaultScreen(r->display)));
	r->copy_buffer = 1;

	r->width  = width;
	r->height = height;

	r->colormap  = DefaultColormap(r->display, XDefaultScreen(r->display));
	r->wm_delete = XInternAtom(r->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(r->display, r->window, &r->wm_delete, 1);

	r->gc = XCreateGC(r->display, r->window, 0, 0);

	create_pixmap(r);

	XSetGraphicsExposures(r->display, r->gc, False);

	XSelectInput(r->display, r->window, mask);
	XMapWindow(r->display, r->window);

	return r;
}

void MwLLDestroy(MwLL handle) {
	MwLLDestroyCommon(handle);

	destroy_pixmap(handle);
	XFreeGC(handle->display, handle->gc);
	XDestroyWindow(handle->display, handle->window);
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
	XFillPolygon(handle->display, handle->pixmap, handle->gc, p, points_count, Convex, CoordModeOrigin);

	free(p);
}

MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	XColor	  xc;

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
			if(ev.xbutton.button == Button1) {
				MwPoint p;
				p.x = ev.xbutton.x;
				p.y = ev.xbutton.y;

				MwLLDispatch(handle, down, &p);

				XSetInputFocus(handle->display, handle->window, RevertToNone, CurrentTime);
			}
		} else if(ev.type == ButtonRelease) {
			if(ev.xbutton.button == Button1) {
				MwPoint p;
				p.x = ev.xbutton.x;
				p.y = ev.xbutton.y;

				MwLLDispatch(handle, up, &p);
			}
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
		} else if(ev.type == MotionNotify) {
			MwPoint p;
			p.x = ev.xmotion.x;
			p.y = ev.xmotion.y;

			MwLLDispatch(handle, move, &p);
		} else if(ev.type == KeyPress) {
			int    n   = -1;
			KeySym sym = XLookupKeysym(&ev.xkey, 0);
			char*  str = XKeysymToString(sym);

			if(strcmp(str, "space") == 0) str = " ";

			/* HACK: this is bad, you can guess why */
			if(strlen(str) == 1) {
				char s = str == NULL ? 0 : str[0];

				if(ev.xkey.state & (ShiftMask | LockMask)) {
					n = toupper((int)s);
				} else {
					n = s;
				}
			} else if(strcmp(str, "BackSpace") == 0) {
				n = MwLLKeyBackSpace;
			}

			if(n != -1) MwLLDispatch(handle, key, &n);
		}
		if(render) {
			int	     x, y;
			unsigned int w, h;

			MwLLGetXYWH(handle, &x, &y, &w, &h);

			MwLLDispatch(handle, draw, NULL);
			if(handle->copy_buffer) XCopyArea(handle->display, handle->pixmap, handle->window, handle->gc, 0, 0, w, h, 0, 0);
		}
	}
}

void MwLLSleep(int ms) {
	usleep(ms * 1000);
}

void MwLLSetTitle(MwLL handle, const char* title) {
	XSetStandardProperties(handle->display, handle->window, title, title, None, (char**)NULL, 0, NULL);
}

MwLLPixmap MwLLCreatePixmap(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));
	char*	   d = malloc(4 * width * height);
	int	   y, x;
	int	   evbase, erbase;

	r->width   = width;
	r->height  = height;
	r->display = handle->display;
	r->use_shm = XShmQueryExtension(handle->display) ? 1 : 0;
	r->data	   = malloc(width * height * 4);

	if(!XRenderQueryExtension(handle->display, &evbase, &erbase)) {
		fprintf(stderr, "XRender missing - cannot proceed pixmap creation\n");
		r->image = NULL;
		return r;
	}

	if(r->use_shm) {
		r->image = XShmCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), 24, ZPixmap, NULL, &r->shm, width, height);
		free(d);

		r->shm.shmid   = shmget(IPC_PRIVATE, r->image->bytes_per_line * height, IPC_CREAT | 0777);
		r->shm.shmaddr = d = r->image->data = shmat(r->shm.shmid, 0, 0);
		r->shm.readOnly			    = False;
		XShmAttach(handle->display, &r->shm);
	} else {
		r->image = XCreateImage(handle->display, DefaultVisual(handle->display, DefaultScreen(handle->display)), 24, ZPixmap, 0, d, width, height, 32, width * 4);
	}

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			unsigned char* px = &data[(y * width + x) * 4];
			unsigned long  p  = 0;
			p <<= 8;
			p |= px[3];
			p <<= 8;
			p |= px[0];
			p <<= 8;
			p |= px[1];
			p <<= 8;
			p |= px[2];
			XPutPixel(r->image, x, y, p);
			*(unsigned long*)(&r->data[(x + y * width) * 4]) = p;
		}
	}

	return r;
}

void MwLLDestroyPixmap(MwLLPixmap pixmap) {
	if(pixmap->image != NULL) {
		if(pixmap->use_shm) {
			XShmDetach(pixmap->display, &pixmap->shm);
		}
		XDestroyImage(pixmap->image);
		if(pixmap->use_shm) {
			shmdt(pixmap->shm.shmaddr);
			shmctl(pixmap->shm.shmid, IPC_RMID, 0);
		}
		free(pixmap->data);
	}

	free(pixmap);
}

void MwLLDrawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	if(pixmap->image != NULL) {
		Pixmap			 px	= XCreatePixmap(handle->display, handle->window, pixmap->width, pixmap->height, 24);
		XRenderPictFormat*	 format = XRenderFindStandardFormat(handle->display, PictStandardRGB24);
		XRenderPictureAttributes attr;
		Picture			 src, dest;
		XTransform		 m;
		double			 xsc = (double)pixmap->width / rect->width;
		double			 ysc = (double)pixmap->height / rect->height;

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

		if(pixmap->use_shm) {
			XShmPutImage(handle->display, px, handle->gc, pixmap->image, 0, 0, 0, 0, pixmap->width, pixmap->height, False);
		} else {
			XPutImage(handle->display, px, handle->gc, pixmap->image, 0, 0, 0, 0, pixmap->width, pixmap->height);
		}

		src  = XRenderCreatePicture(handle->display, px, format, 0, &attr);
		dest = XRenderCreatePicture(handle->display, handle->pixmap, format, 0, &attr);

		XRenderSetPictureTransform(handle->display, src, &m);
		XRenderComposite(handle->display, PictOpSrc, src, 0, dest, 0, 0, 0, 0, rect->x, rect->y, rect->width, rect->height);

		XRenderFreePicture(handle->display, src);
		XRenderFreePicture(handle->display, dest);

		XFreePixmap(handle->display, px);
	}
}

void MwLLSetIcon(MwLL handle, MwLLPixmap pixmap) {
	unsigned long* icon = malloc((2 + pixmap->width * pixmap->height) * sizeof(*icon));
	int	       i;
	Atom	       atom = XInternAtom(handle->display, "_NET_WM_ICON", False);

	icon[0] = pixmap->width;
	icon[1] = pixmap->height;

	for(i = 0; i < pixmap->width * pixmap->height; i++) {
		icon[2 + i] = *(unsigned long*)(&pixmap->data[i * 4]);
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
}

void MwLLShow(MwLL handle, int show) {
	if(show) {
		XMapWindow(handle->display, handle->window);
		XSetInputFocus(handle->display, handle->window, RevertToNone, CurrentTime);
	} else {
		XUnmapWindow(handle->display, handle->window);
	}
}
