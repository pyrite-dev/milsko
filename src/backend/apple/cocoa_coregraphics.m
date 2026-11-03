#include <Mw/Milsko.h>
#include "applegraphics.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __BIG_ENDIAN__
#define BITMAP_INFO ((CGBitmapInfo)kCGImageAlphaPremultipliedFirst)
#else
#define BITMAP_INFO ((CGBitmapInfo)kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little)
#endif

/* SDKs older than 10.5 have no CGFloat and use float everywhere */
#ifndef CGFLOAT_DEFINED
typedef float CGFloat;
#endif

static struct {
	void* lib;

	CGColorSpaceRef (*CGColorSpaceCreateDeviceRGB)(void);
	void (*CGColorSpaceRelease)(CGColorSpaceRef space);

	CGContextRef (*CGBitmapContextCreate)(void* data, size_t width, size_t height, size_t bits_per_component, size_t bytes_per_row, CGColorSpaceRef space, CGBitmapInfo info);
	void (*CGContextRelease)(CGContextRef c);
	void (*CGContextFlush)(CGContextRef c);
	void (*CGContextSaveGState)(CGContextRef c);
	void (*CGContextRestoreGState)(CGContextRef c);
	void (*CGContextClipToRect)(CGContextRef c, CGRect rect);
	void (*CGContextTranslateCTM)(CGContextRef c, CGFloat tx, CGFloat ty);
	void (*CGContextScaleCTM)(CGContextRef c, CGFloat sx, CGFloat sy);
	void (*CGContextSetShouldAntialias)(CGContextRef c, bool should_antialias);
	void (*CGContextSetInterpolationQuality)(CGContextRef c, CGInterpolationQuality quality);
	void (*CGContextSetLineWidth)(CGContextRef c, CGFloat width);
	void (*CGContextSetLineCap)(CGContextRef c, CGLineCap cap);
	void (*CGContextBeginPath)(CGContextRef c);
	void (*CGContextMoveToPoint)(CGContextRef c, CGFloat x, CGFloat y);
	void (*CGContextAddLineToPoint)(CGContextRef c, CGFloat x, CGFloat y);
	void (*CGContextClosePath)(CGContextRef c);
	void (*CGContextSetRGBFillColor)(CGContextRef c, CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha);
	void (*CGContextSetRGBStrokeColor)(CGContextRef c, CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha);
	void (*CGContextEOFillPath)(CGContextRef c);
	void (*CGContextStrokePath)(CGContextRef c);
	void (*CGContextDrawImage)(CGContextRef c, CGRect rect, CGImageRef image);

	CGDataProviderRef (*CGDataProviderCreateWithData)(void* info, const void* data, size_t size, void (*release_data)(void* info, const void* data, size_t size));
	void (*CGDataProviderRelease)(CGDataProviderRef provider);

	CGImageRef (*CGImageCreate)(size_t width, size_t height, size_t bits_per_component, size_t bits_per_pixel, size_t bytes_per_row, CGColorSpaceRef space, CGBitmapInfo info, CGDataProviderRef provider, const CGFloat* decode, bool should_interpolate, CGColorRenderingIntent intent);
	void (*CGImageRelease)(CGImageRef image);
} cg;

static int load_funcs(void) {
	cg.lib = MwDynamicOpen("/System/Library/Frameworks/ApplicationServices.framework/ApplicationServices");
	if(cg.lib == NULL) {
		printf("(ApplicationServices not found, cannot use CoreGraphics.)\n");
		return 1;
	}

#define COREGRAPHICS_FUNC(x) \
	cg.x = MwDynamicSymbol(NULL, #x); \
	if(cg.x == NULL) { \
		printf("WARNING: Could use CoreGraphics if " #x " was found.\n"); \
		return 1; \
	}

	COREGRAPHICS_FUNC(CGColorSpaceCreateDeviceRGB);
	COREGRAPHICS_FUNC(CGColorSpaceRelease);
	COREGRAPHICS_FUNC(CGBitmapContextCreate);
	COREGRAPHICS_FUNC(CGContextRelease);
	COREGRAPHICS_FUNC(CGContextFlush);
	COREGRAPHICS_FUNC(CGContextSaveGState);
	COREGRAPHICS_FUNC(CGContextRestoreGState);
	COREGRAPHICS_FUNC(CGContextClipToRect);
	COREGRAPHICS_FUNC(CGContextTranslateCTM);
	COREGRAPHICS_FUNC(CGContextScaleCTM);
	COREGRAPHICS_FUNC(CGContextSetShouldAntialias);
	COREGRAPHICS_FUNC(CGContextSetInterpolationQuality);
	COREGRAPHICS_FUNC(CGContextSetLineWidth);
	COREGRAPHICS_FUNC(CGContextSetLineCap);
	COREGRAPHICS_FUNC(CGContextBeginPath);
	COREGRAPHICS_FUNC(CGContextMoveToPoint);
	COREGRAPHICS_FUNC(CGContextAddLineToPoint);
	COREGRAPHICS_FUNC(CGContextClosePath);
	COREGRAPHICS_FUNC(CGContextSetRGBFillColor);
	COREGRAPHICS_FUNC(CGContextSetRGBStrokeColor);
	COREGRAPHICS_FUNC(CGContextEOFillPath);
	COREGRAPHICS_FUNC(CGContextStrokePath);
	COREGRAPHICS_FUNC(CGContextDrawImage);
	COREGRAPHICS_FUNC(CGDataProviderCreateWithData);
	COREGRAPHICS_FUNC(CGDataProviderRelease);
	COREGRAPHICS_FUNC(CGImageCreate);
	COREGRAPHICS_FUNC(CGImageRelease);

#undef COREGRAPHICS_FUNC

	return 0;
}

static void apply_clip(MwLL handle) {
	CGContextRef c = handle->cocoa.context;

	cg.CGContextRestoreGState(c);
	cg.CGContextSaveGState(c);

	if(handle->cocoa.has_clip) {
		MwRect* r = &handle->cocoa.clip;

		cg.CGContextClipToRect(c, CGRectMake(r->x, r->y, r->width, r->height));
	}
}

static void create_context(MwLL handle, int w, int h) {
	unsigned char*	buffer = malloc((size_t)w * h * 4);
	CGColorSpaceRef rgb;
	CGContextRef	c;
	int		y;

	if(buffer == NULL) {
		return;
	}
	memset(buffer, 0xff, (size_t)w * h * 4);

	if(handle->cocoa.buffer != NULL) {
		int cw = handle->cocoa.width;
		int ch = handle->cocoa.height;

		if(w < cw) {
			cw = w;
		}
		if(h < ch) {
			ch = h;
		}

		cg.CGContextFlush(handle->cocoa.context);
		for(y = 0; y < ch; y++) {
			memcpy(buffer + (size_t)y * w * 4, handle->cocoa.buffer + (size_t)y * handle->cocoa.width * 4, cw * 4);
		}
	}

	rgb = cg.CGColorSpaceCreateDeviceRGB();
	c   = cg.CGBitmapContextCreate(buffer, w, h, 8, w * 4, rgb, BITMAP_INFO);
	cg.CGColorSpaceRelease(rgb);
	if(c == NULL) {
		free(buffer);
		return;
	}

	if(handle->cocoa.context != NULL) {
		cg.CGContextRelease(handle->cocoa.context);
	}
	free(handle->cocoa.buffer);

	cg.CGContextTranslateCTM(c, 0, h);
	cg.CGContextScaleCTM(c, 1, -1);
	cg.CGContextSetShouldAntialias(c, false);
	cg.CGContextSetInterpolationQuality(c, kCGInterpolationNone);
	cg.CGContextSetLineWidth(c, 1);
	cg.CGContextSetLineCap(c, kCGLineCapSquare);
	cg.CGContextSaveGState(c);

	handle->cocoa.context = c;
	handle->cocoa.buffer  = buffer;
	handle->cocoa.width   = w;
	handle->cocoa.height  = h;

	apply_clip(handle);
}

static void destroy_context(MwLL handle) {
	if(handle->cocoa.context != NULL) {
		cg.CGContextRelease(handle->cocoa.context);
	}
	free(handle->cocoa.buffer);
}

static void present(MwLL handle, NSRect bounds) {
	size_t		  size;
	CGColorSpaceRef	  rgb;
	CGContextRef	  port;
	CGDataProviderRef provider;
	CGImageRef	  image;

	cg.CGContextFlush(handle->cocoa.context);
	size = (size_t)handle->cocoa.width * handle->cocoa.height * 4;

	port	 = [[NSGraphicsContext currentContext] graphicsPort];
	rgb	 = cg.CGColorSpaceCreateDeviceRGB();
	provider = cg.CGDataProviderCreateWithData(NULL, handle->cocoa.buffer, size, NULL);
	image	 = cg.CGImageCreate(handle->cocoa.width, handle->cocoa.height, 8, 32, handle->cocoa.width * 4, rgb, BITMAP_INFO, provider, NULL, false, kCGRenderingIntentDefault);

	cg.CGContextSetInterpolationQuality(port, kCGInterpolationNone);
	cg.CGContextDrawImage(port, CGRectMake(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height), image);

	cg.CGImageRelease(image);
	cg.CGDataProviderRelease(provider);
	cg.CGColorSpaceRelease(rgb);
}

static void polygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	CGContextRef c = handle->cocoa.context;
	int	     i;

	cg.CGContextBeginPath(c);
	cg.CGContextMoveToPoint(c, points[0].x, points[0].y);
	for(i = 1; i < points_count; i++) {
		cg.CGContextAddLineToPoint(c, points[i].x, points[i].y);
	}
	cg.CGContextClosePath(c);

	cg.CGContextSetRGBFillColor(c, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0, 1.0);
	cg.CGContextEOFillPath(c);
}

static void line(MwLL handle, MwPoint* points, MwLLColor color) {
	CGContextRef c = handle->cocoa.context;

	cg.CGContextBeginPath(c);
	cg.CGContextMoveToPoint(c, points[0].x + 0.5, points[0].y + 0.5);
	cg.CGContextAddLineToPoint(c, points[1].x + 0.5, points[1].y + 0.5);

	cg.CGContextSetRGBStrokeColor(c, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0, 1.0);
	cg.CGContextStrokePath(c);
}

static void update_pixmap(MwLLPixmap pixmap) {
	size_t		  n = (size_t)pixmap->common.width * pixmap->common.height;
	CGDataProviderRef provider;
	CGColorSpaceRef	  rgb;

	if(pixmap->cocoa.image != NULL) {
		cg.CGImageRelease(pixmap->cocoa.image);
	}

	rgb		    = cg.CGColorSpaceCreateDeviceRGB();
	provider	    = cg.CGDataProviderCreateWithData(NULL, pixmap->cocoa.premultiplied, n * 4, NULL);
	pixmap->cocoa.image = cg.CGImageCreate(pixmap->common.width, pixmap->common.height, 8, 32, pixmap->common.width * 4, rgb, BITMAP_INFO, provider, NULL, false, kCGRenderingIntentDefault);
	cg.CGDataProviderRelease(provider);
	cg.CGColorSpaceRelease(rgb);
}

static void destroy_pixmap(MwLLPixmap pixmap) {
	if(pixmap->cocoa.image != NULL) {
		cg.CGImageRelease(pixmap->cocoa.image);
	}
}

static void draw_pixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	CGContextRef c = handle->cocoa.context;

	cg.CGContextSaveGState(c);
	cg.CGContextTranslateCTM(c, rect->x, rect->y + rect->height);
	cg.CGContextScaleCTM(c, 1, -1);
	cg.CGContextDrawImage(c, CGRectMake(0, 0, rect->width, rect->height), pixmap->cocoa.image);
	cg.CGContextRestoreGState(c);
}

int MwLLCocoaCoreGraphicsSetup(MwLLCocoaGraphics* graphics) {
	if(load_funcs() != 0) {
		return 1;
	}

	graphics->create_context  = create_context;
	graphics->destroy_context = destroy_context;
	graphics->apply_clip	  = apply_clip;
	graphics->present	  = present;
	graphics->polygon	  = polygon;
	graphics->line		  = line;
	graphics->update_pixmap	  = update_pixmap;
	graphics->destroy_pixmap  = destroy_pixmap;
	graphics->draw_pixmap	  = draw_pixmap;

	return 0;
}
