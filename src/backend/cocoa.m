#include <AppKit/NSGraphicsContext.h>
#include <Mw/Milsko.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "../../external/stb_ds.h"
#include "Mw/BaseTypes.h"

@implementation MilskoCocoaPixmap

+ (MilskoCocoaPixmap*)newWithWidth:(int)width height:(int)height {
	MilskoCocoaPixmap* p = [MilskoCocoaPixmap alloc];

	p->width  = width;
	p->height = height;
	p->data	  = NULL;
	p->image  = NULL;

	return p;
}
- (void)updateWithData:(void*)_data {
	[self destroy];

	self->data  = [NSData dataWithBytes:_data length:self->width * self->height * 4];
	self->image = [[NSImage alloc] initWithData:self->data];
}
- (void)destroy {
	if(self->data != NULL) {
		[self->data dealloc];
	}
	if(self->image != NULL) {
		[self->image dealloc];
	}
}
- (NSImage*)image {
	return self->image;
}
@end

@implementation MilskoCocoa

+ (MilskoCocoa*)newWithParent:(MwLL)parent
			    x:(int)x
			    y:(int)y
			width:(int)width
		       height:(int)height {
	MilskoCocoa* c	     = [MilskoCocoa alloc];
	bool	     centerX = false, centerY = false;

	if(x == MwDEFAULT) {
		x	= 0;
		centerX = true;
	}
	if(y == MwDEFAULT) {
		y	= 0;
		centerY = true;
	}
	c->application = [NSApplication sharedApplication];

	c->rect = NSMakeRect(x, y, width, height);

	c->window = [[NSWindow alloc]
	    initWithContentRect:c->rect
		      styleMask:parent == NULL
				    ? (NSTitledWindowMask | NSClosableWindowMask |
				       NSMiniaturizableWindowMask |
				       NSResizableWindowMask)
				    : NSBorderlessWindowMask
			backing:NSBackingStoreBuffered
			  defer:NO];
	[c->window makeKeyAndOrderFront:c->application];

	if(parent != NULL) {
		MilskoCocoa* p = parent->cocoa.real;
		[p->window addChildWindow:c->window ordered:NSWindowAbove];
	} else {
		[c->application activateIgnoringOtherApps:true];
	}

	c->view = [[MilskoCocoaView alloc] initWithFrame:c->rect];
	[c->window setContentView:c->view];

	return c;
}
- (void)polygonWithPoints:(MwPoint*)points
	     points_count:(int)points_count
		    color:(MwLLColor)color {
	int	     i;
	CGContextRef cg = [self->view context];

	CGContextSetRGBFillColor(cg, color->common.red / 255., color->common.blue / 255., color->common.green / 255., 1);
	CGContextBeginPath(cg);
	for(i = 0; i < points_count; i++) {
		CGContextMoveToPoint(cg, points[i].x, points[i].y);
		if(i < points_count - 1) {
			CGContextAddLineToPoint(cg, points[i + 1].x, points[i + 1].y);
		}
	}
	CGContextFillPath(cg);

	// [self->view setNeedsDisplay:true];
};
- (void)lineWithPoints:(MwPoint*)points color:(MwLLColor)color {
};
- (void)getX:(int*)x Y:(int*)y W:(unsigned int*)w H:(unsigned int*)h {
	NSRect frame = [self->window frame];

	*x = frame.origin.x;
	*y = frame.origin.y;
	*w = frame.size.width;
	*h = frame.size.height;
};
- (void)setX:(int)x Y:(int)y {
	NSPoint p;
	p.x	       = x;
	p.y	       = y;
	NSRect frame   = [self->window frame];
	frame.origin.x = x;
	frame.origin.y = y;
	[self->window setFrame:frame display:YES animate:true];
};
- (void)setW:(int)w H:(int)h {
	NSRect frame	  = [self->window frame];
	frame.size.width  = w;
	frame.size.height = h;
	[self->window setFrame:frame display:YES animate:true];
};
- (int)pending {
	return 1;
};
- (void)getNextEvent {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	NSEvent* event = [self->application nextEventMatchingMask:NSAnyEventMask
							untilDate:nil
							   inMode:NSDefaultRunLoopMode
							  dequeue:YES];

	if(event != nil) {
		printf("got event: %p\n", event);
	}

	[self->application sendEvent:event];

	/* this should be in the draw functions but it's here for now for testing */
	[self->view setNeedsDisplay:true];

	[pool release];
};
- (void)setTitle:(const char*)title {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	[self->window
	    setTitleWithRepresentedFilename:[NSString stringWithUTF8String:title]];
	[pool release];
};
- (void)drawPixmap:(MwLLPixmap)pixmap rect:(MwRect*)_rect {
	MilskoCocoaPixmap* p = pixmap->cocoa.real;
	[[p image] drawAtPoint:NSMakePoint(_rect->x, _rect->y) fromRect:NSMakeRect(_rect->x, _rect->y, _rect->width, _rect->height) operation:NSCompositeClear fraction:1.0];
};
- (void)setIcon:(MwLLPixmap)pixmap {
};
- (void)forceRender {
};
- (void)setCursor:(MwCursor*)image mask:(MwCursor*)mask {
};
- (void)detachWithPoint:(MwPoint*)point {
};
- (void)show:(int)show {
};
- (void)makePopupWithParent:(MwLL)parent {
};
- (void)setSizeHintsWithMinX:(int)minx
			MinY:(int)miny
			MaxX:(int)maxx
			MaxY:(int)maxy {
};
- (void)makeBorderless:(int)toggle {
	uint32_t mask = [self->window styleMask];
	if(mask & NSBorderlessWindowMask) {
		mask ^= NSBorderlessWindowMask;
		mask |= NSTitledWindowMask;
	} else {
		mask |= NSBorderlessWindowMask;
		mask ^= NSTitledWindowMask;
	}
	[self->window initWithContentRect:self->rect
				styleMask:mask
				  backing:NSBackingStoreBuffered
				    defer:NO];
};
- (void)focus {
	[self->window makeMainWindow];
};
- (void)grabPointer:(int)toggle {
	/* MacOS didn't have a "pointer grab" function until 10.13.2 so I need to do
	 * this manually */
};
- (void)setClipboard:(const char*)text {
};
- (void)getClipboard {
};
- (void)makeToolWindow {
};
- (void)getCursorCoord:(MwPoint*)point {
	NSPoint p = [NSEvent mouseLocation];
	point->x  = p.x;
	point->y  = p.y;
};
- (void)getScreenSize:(MwRect*)_rect {
	NSScreen* screen = [self->window screen];
	_rect->x	 = [screen frame].origin.x;
	_rect->y	 = [screen frame].origin.y;
	_rect->width	 = [screen frame].size.width;
	_rect->height	 = [screen frame].size.height;
};
- (void)destroy {
	[self->window dealloc];
}

@end

@implementation MilskoCocoaView
- (id)initWithFrame:(NSRect)frame {
	self	     = [super initWithFrame:frame];
	self->width  = frame.size.width;
	self->height = frame.size.height;
	self->buf    = malloc(self->width * self->height * 4);
	self->space  = CGColorSpaceCreateDeviceRGB();
	self->cg     = CGBitmapContextCreate(self->buf,
					     self->width,
					     self->height,
					     CHAR_BIT,
					     self->width * sizeof(uint32_t),
					     self->space, kCGBitmapByteOrder32Host | kCGImageAlphaPremultipliedLast);
	assert(self->cg);
	printf("%p\n", self->cg);
	return self;
}

- (CGContextRef)context {
	return self->cg;
}

- (void)drawRect:(NSRect)dirtyRect {
	[super drawRect:dirtyRect];
	if(self) {

		CGImageRef img = CGBitmapContextCreateImage(self->cg);
		if(!img) {
			return;
		}
		printf("printed image\n");

		CGContextDrawImage([[NSGraphicsContext currentContext] graphicsPort], CGRectMake(0, 0, self->width, self->height), img);
		CGImageRelease(img);
	}
	// printf("%0.2f %0.2f %0.2f %0.2f\n", dirtyRect.origin.x, dirtyRect.origin.y, dirtyRect.size.width, dirtyRect.size.height);
}

- (void)destroy {
	free(self->buf);
	CGContextRelease(self->cg);
	CGColorSpaceRelease(self->space);
}

@end

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	(void)x;
	(void)y;
	(void)width;
	(void)height;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

	MilskoCocoa* o =
	    [MilskoCocoa newWithParent:parent
				     x:x
				     y:y
				 width:width
				height:height];
	r->cocoa.real = o;

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MilskoCocoa* h = handle->cocoa.real;

	[h destroy];

	MwLLDestroyCommon(handle);

	free(handle);
}

static void MwLLBeginDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count,
			    MwLLColor color) {
	MilskoCocoa* h = handle->cocoa.real;
	[h polygonWithPoints:points points_count:points_count color:color];
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	MilskoCocoa* h = handle->cocoa.real;
	[h lineWithPoints:points color:color];
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	(void)handle;

	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w,
			    unsigned int* height) {
	MilskoCocoa* h = handle->cocoa.real;
	[h getX:x Y:y W:w H:height];
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setX:x Y:y];
}

static void MwLLSetWHImpl(MwLL handle, int w, int height) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setW:w H:height];
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static int MwLLPendingImpl(MwLL handle) {
	MilskoCocoa* h = handle->cocoa.real;
	return [h pending];
}

static void MwLLNextEventImpl(MwLL handle) {

	MilskoCocoa* h = handle->cocoa.real;
	[h getNextEvent];
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setTitle:title];
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data,
				       int width, int height) {
	(void)handle;

	MwLLPixmap r = malloc(sizeof(*r));

	r->common.raw = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	r->common.width	 = width;
	r->common.height = height;

	r->cocoa.real = [MilskoCocoaPixmap newWithWidth:width height:height];

	MwLLPixmapUpdate(r);
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {
	MilskoCocoaPixmap* p = pixmap->cocoa.real;
	[p updateWithData:pixmap->common.raw];
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	MilskoCocoaPixmap* p = pixmap->cocoa.real;
	[p destroy];
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	MilskoCocoa* h = handle->cocoa.real;
	[h drawPixmap:pixmap rect:rect];
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setIcon:pixmap];
}

static void MwLLForceRenderImpl(MwLL handle) {
	MilskoCocoa* h = handle->cocoa.real;
	[h forceRender];
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setCursor:image mask:mask];
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	MilskoCocoa* h = handle->cocoa.real;
	[h detachWithPoint:point];
}

static void MwLLShowImpl(MwLL handle, int show) {
	MilskoCocoa* h = handle->cocoa.real;
	[h show:show];
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	MilskoCocoa* h = handle->cocoa.real;
	[h makePopupWithParent:parent];
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx,
				 int maxy) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setSizeHintsWithMinX:minx MinY:miny MaxX:maxx MaxY:maxy];
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	MilskoCocoa* h = handle->cocoa.real;
	[h makeBorderless:toggle];
}

static void MwLLFocusImpl(MwLL handle) {
	MilskoCocoa* h = handle->cocoa.real;
	[h focus];
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	MilskoCocoa* h = handle->cocoa.real;
	[h grabPointer:toggle];
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	MilskoCocoa* h = handle->cocoa.real;
	[h setClipboard:text];
}

static void MwLLGetClipboardImpl(MwLL handle) {
	(void)handle;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {

	MilskoCocoa* h = handle->cocoa.real;
	[h makeToolWindow];
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	MilskoCocoa* h = handle->cocoa.real;
	[h getCursorCoord:point];
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	MilskoCocoa* h = handle->cocoa.real;
	[h getScreenSize:rect];
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static int MwLLCocoaCallInitImpl(void) {
	return 0;
}

#include "call.c"
CALL(Cocoa);
