/* $Id$ */
#include <Mw/Milsko.h>

static unsigned long generate_color(MwLL handle, unsigned long r, unsigned long g, unsigned long b) {
	unsigned long c = 0;

	c |= (r * handle->x11.red_max / 255) << handle->x11.red_shift;

	c |= (g * handle->x11.green_max / 255) << handle->x11.green_shift;

	c |= (b * handle->x11.blue_max / 255) << handle->x11.blue_shift;

	return c;
}

static void MwLLPolygonImplNative(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
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

static void MwLLLineImplNative(MwLL handle, MwPoint* points, MwLLColor color) {
	XSetForeground(handle->x11.display, handle->x11.gc, color->x11.pixel);

	XDrawLine(handle->x11.display, handle->x11.pixmap, handle->x11.gc, points[0].x, points[0].y, points[1].x, points[1].y);
}

static MwLLColor MwLLAllocColorImplNative(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImplNative(MwLL handle, MwLLColor c, int r, int g, int b) {
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

static MwLLPixmap MwLLCreatePixmapImplNative(MwLL handle, unsigned char* data, int width, int height) {
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

static void MwLLPixmapUpdateImplNative(MwLLPixmap r) {
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

static void MwLLDestroyPixmapImplNative(MwLLPixmap pixmap) {
	free(pixmap->common.raw);
	XDestroyImage(pixmap->x11.image);
	XDestroyImage(pixmap->x11.mask);
	free(pixmap->x11.data);

	free(pixmap);
}

static void MwLLDrawPixmapImplNative(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
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
static void MwLLFreeColorImplNative(MwLLColor color) {
	free(color);
}
