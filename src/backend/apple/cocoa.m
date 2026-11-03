#include <Mw/Milsko.h>
#include "../../external/stb_ds.h"

#include <AvailabilityMacros.h>

#ifdef MW_VULKAN
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_metal.h>
#endif

#ifdef __clang__
#pragma clang push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __BIG_ENDIAN__
#define BITMAP_INFO ((CGBitmapInfo)kCGImageAlphaPremultipliedFirst)
#else
#define BITMAP_INFO ((CGBitmapInfo)kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little)
#endif

enum cevent_type {
	CE_DRAW = 0,
	CE_RESIZE,
	CE_DOWN,
	CE_UP,
	CE_MOVE,
	CE_KEY,
	CE_KEYUP,
	CE_FOCUSIN,
	CE_FOCUSOUT,
	CE_CLOSE,
	CE_CLIPBOARD,
	CE_DROP,
	CE_DARK
};

@interface MilskoCocoaView : NSView {
    @public
	MwLL mw;
}
@end

@interface MilskoCocoaWindow : NSWindow {
    @public
	MwLL mw;
}
@end

@interface MilskoCocoaObserver : NSObject {
}
- (void)quit:(id)sender;
- (void)themeChanged:(NSNotification*)notification;
@end

static void after_resize(MwLL top);

static MwLL get_top(MwLL handle) {
	while(handle->cocoa.parent != NULL) {
		handle = handle->cocoa.parent;
	}

	if(handle->cocoa.window == nil) {
		return NULL;
	}

	return handle;
}

static void get_offset(MwLL handle, int* x, int* y) {
	*x = 0;
	*y = 0;
	while(handle->cocoa.parent != NULL) {
		*x += handle->cocoa.x;
		*y += handle->cocoa.y;

		handle = handle->cocoa.parent;
	}
}

static NSRect primary_frame(void) {
	NSArray* screens = [NSScreen screens];

	if(screens != nil && [screens count] > 0) {
		return [[screens objectAtIndex:0] frame];
	}
	if([NSScreen mainScreen] != nil) {
		return [[NSScreen mainScreen] frame];
	}

	return NSMakeRect(0, 0, 1024, 768);
}

static void content_origin(MwLL top, int* x, int* y) {
	NSRect r = [top->cocoa.window contentRectForFrameRect:[top->cocoa.window frame]];

	*x = (int)r.origin.x;
	*y = (int)(primary_frame().size.height - (r.origin.y + r.size.height));
}

static NSString* utf8_string(const char* s) {
	NSString* r = [NSString stringWithUTF8String:s];

	if(r == nil) {
		return @"";
	}

	return r;
}

static char* string_dup(NSString* s) {
	const char* u = [s UTF8String];
	char*	    r;

	if(u == NULL) {
		u = "";
	}
	r = malloc(strlen(u) + 1);
	strcpy(r, u);

	return r;
}

static MwLLCocoaEvent* new_event(int type, MwLL handle) {
	MwLLCocoaEvent e;

	memset(&e, 0, sizeof(e));
	e.type = type;
	arrput(handle->cocoa.events, e);

	return &handle->cocoa.events[arrlen(handle->cocoa.events) - 1];
}

static void push_unique(int type, MwLL handle) {
	int i;

	for(i = 0; i < arrlen(handle->cocoa.events); i++) {
		if(handle->cocoa.events[i].type == type) {
			return;
		}
	}

	new_event(type, handle);
}

static void push_mouse(int type, MwLL handle, int x, int y, int button) {
	MwLLCocoaEvent* e = new_event(type, handle);

	e->mouse.point.x = x;
	e->mouse.point.y = y;
	e->mouse.button	 = button;
}

static void push_key(int down, MwLL handle, int key) {
	if(down) {
		new_event(CE_KEY, handle)->key = key;
	} else {
		new_event(CE_KEYUP, handle)->key = key;
	}
}

static void push_move(MwLL handle, int x, int y, int relative) {
	int n = arrlen(handle->cocoa.events);

	if(n > 0 && handle->cocoa.events[n - 1].type == CE_MOVE) {
		if(relative) {
			handle->cocoa.events[n - 1].mouse.point.x += x;
			handle->cocoa.events[n - 1].mouse.point.y += y;
		} else {
			handle->cocoa.events[n - 1].mouse.point.x = x;
			handle->cocoa.events[n - 1].mouse.point.y = y;
		}
		return;
	}

	push_mouse(CE_MOVE, handle, x, y, 0);
}

static void set_focus(MwLL handle) {
	MwLL top = get_top(handle);

	if(top == NULL || top->cocoa.focus == handle) {
		return;
	}

	if(top->cocoa.focus != NULL) {
		push_unique(CE_FOCUSOUT, top->cocoa.focus);
	}
	top->cocoa.focus = handle;
	push_unique(CE_FOCUSIN, handle);
}

static int is_inside(MwLL handle, MwLL ancestor) {
	MwLL h;

	for(h = handle; h != NULL; h = h->cocoa.parent) {
		if(h == ancestor) {
			return 1;
		}
	}

	return 0;
}

static void forget(MwLL top, MwLL handle) {
	if(is_inside(top->cocoa.focus, handle)) {
		top->cocoa.focus = NULL;
	}
	if(is_inside(top->cocoa.capture, handle)) {
		top->cocoa.capture = NULL;
	}
	if(is_inside(top->cocoa.hover, handle)) {
		top->cocoa.hover = NULL;
	}
	if(is_inside(top->cocoa.grab, handle)) {
		top->cocoa.grab = NULL;
		CGAssociateMouseAndMouseCursorPosition(true);
	}
}

static MwLL from_window(NSWindow* window) {
	if(window == nil || ![window isKindOfClass:[MilskoCocoaWindow class]]) {
		return NULL;
	}

	return ((MilskoCocoaWindow*)window)->mw;
}

static void quit_all(void) {
	NSArray*     windows = [NSApp orderedWindows];
	unsigned int i;

	for(i = 0; i < [windows count]; i++) {
		MwLL h = from_window([windows objectAtIndex:i]);

		if(h != NULL && !h->cocoa.tool) {
			push_unique(CE_CLOSE, h);
			return;
		}
	}
}

static void apply_clip(MwLL handle) {
	CGContextRef c = handle->cocoa.context;

	CGContextRestoreGState(c);
	CGContextSaveGState(c);

	if(handle->cocoa.has_clip) {
		MwRect* r = &handle->cocoa.clip;

		CGContextClipToRect(c, CGRectMake(r->x, r->y, r->width, r->height));
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

		CGContextFlush(handle->cocoa.context);
		for(y = 0; y < ch; y++) {
			memcpy(buffer + (size_t)y * w * 4, handle->cocoa.buffer + (size_t)y * handle->cocoa.width * 4, cw * 4);
		}
	}

	rgb = CGColorSpaceCreateDeviceRGB();
	c   = CGBitmapContextCreate(buffer, w, h, 8, w * 4, rgb, BITMAP_INFO);
	CGColorSpaceRelease(rgb);
	if(c == NULL) {
		free(buffer);
		return;
	}

	if(handle->cocoa.context != NULL) {
		CGContextRelease(handle->cocoa.context);
	}
	free(handle->cocoa.buffer);

	CGContextTranslateCTM(c, 0, h);
	CGContextScaleCTM(c, 1, -1);
	CGContextSetShouldAntialias(c, false);
	CGContextSetInterpolationQuality(c, kCGInterpolationNone);
	CGContextSetLineWidth(c, 1);
	CGContextSetLineCap(c, kCGLineCapSquare);
	CGContextSaveGState(c);

	handle->cocoa.context = c;
	handle->cocoa.buffer  = buffer;
	handle->cocoa.width   = w;
	handle->cocoa.height  = h;

	apply_clip(handle);
}

static void mark_dirty(MwLL handle) {
	MwLL top;

	[handle->cocoa.view setNeedsDisplay:YES];
	if((top = get_top(handle)) != NULL) {
		top->cocoa.dirty = 1;
	}
}

static void push_draw_tree(MwLL handle) {
	int i;

	push_unique(CE_DRAW, handle);
	for(i = 0; i < arrlen(handle->cocoa.children); i++) {
		push_draw_tree(handle->cocoa.children[i]);
	}
}

static void place_view(MwLL handle) {
	MwLL p = handle->cocoa.parent;

	if(p == NULL) {
		return;
	}

	[handle->cocoa.view setFrame:NSMakeRect(handle->cocoa.x, p->cocoa.height - handle->cocoa.y - handle->cocoa.height, handle->cocoa.width, handle->cocoa.height)];
}

static void place_children(MwLL handle) {
	int i;

	for(i = 0; i < arrlen(handle->cocoa.children); i++) {
		place_view(handle->cocoa.children[i]);
	}
}

static void restack(MwLL parent) {
	int i;

	for(i = 0; i < arrlen(parent->cocoa.children); i++) {
		[parent->cocoa.children[i]->cocoa.view removeFromSuperview];
	}
	for(i = 0; i < arrlen(parent->cocoa.children); i++) {
		MwLL c = parent->cocoa.children[i];

		if(c->cocoa.shown) {
			[parent->cocoa.view addSubview:c->cocoa.view];
		}
	}
}

static unsigned int style_for(MwLL handle) {
	unsigned int s;

	if(handle->cocoa.tool || handle->cocoa.borderless) {
		return NSBorderlessWindowMask;
	}

	s = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
	if(!(handle->cocoa.min_w > 0 && handle->cocoa.min_w == handle->cocoa.max_w && handle->cocoa.min_h == handle->cocoa.max_h)) {
		s |= NSResizableWindowMask;
	}

	return s;
}

static void apply_hints(MwLL handle) {
	NSWindow* w	= handle->cocoa.window;
	int	  min_w = 1, min_h = 1, max_w = 30000, max_h = 30000;

	if(handle->cocoa.min_w > 0) {
		min_w = handle->cocoa.min_w;
	}
	if(handle->cocoa.min_h > 0) {
		min_h = handle->cocoa.min_h;
	}
	if(handle->cocoa.max_w > 0) {
		max_w = handle->cocoa.max_w;
	}
	if(handle->cocoa.max_h > 0) {
		max_h = handle->cocoa.max_h;
	}

	[w setMinSize:[w frameRectForContentRect:NSMakeRect(0, 0, min_w, min_h)].size];
	[w setMaxSize:[w frameRectForContentRect:NSMakeRect(0, 0, max_w, max_h)].size];
}

static NSWindow* parent_window(NSWindow* w) {
	if(![w respondsToSelector:@selector(parentWindow)]) {
		return nil;
	}

	return [w parentWindow];
}

static void unparent_window(NSWindow* w) {
	NSWindow* p = parent_window(w);

	if(p != nil) {
		[p removeChildWindow:w];
	}
}

static void create_window(MwLL handle, int x, int y) {
	MilskoCocoaWindow* old = handle->cocoa.window;
	MilskoCocoaWindow* win;
	NSRect		   r = NSMakeRect(x, primary_frame().size.height - y - handle->cocoa.height, handle->cocoa.width, handle->cocoa.height);

	win	= [[MilskoCocoaWindow alloc] initWithContentRect:r styleMask:style_for(handle) backing:NSBackingStoreBuffered defer:NO];
	win->mw = handle;

	[win setReleasedWhenClosed:NO];
	[win setDelegate:(id)win];
	[win setAcceptsMouseMovedEvents:YES];
	if(handle->cocoa.title != nil) {
		[win setTitle:handle->cocoa.title];
	}

	if(old != nil) {
		old->mw = NULL;
		[old setDelegate:nil];
		unparent_window(old);
		[old orderOut:nil];
		[old setContentView:nil];
	}

	[win setContentView:handle->cocoa.view];
	[handle->cocoa.view registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];

	if(handle->cocoa.tool) {
		[win setLevel:NSPopUpMenuWindowLevel];
		[win setHidesOnDeactivate:YES];
	}

	handle->cocoa.window = win;
	apply_hints(handle);

	if(old != nil) {
		[old release];
	}

	if(handle->cocoa.shown) {
		handle->cocoa.pending_show = 1;
	}
}

static void restyle(MwLL handle) {
	int x, y;

	if(handle->cocoa.window == nil || handle->cocoa.in_state_change) {
		return;
	}

	if([handle->cocoa.window styleMask] != style_for(handle) || (handle->cocoa.tool && [handle->cocoa.window level] != NSPopUpMenuWindowLevel)) {
		content_origin(handle, &x, &y);
		create_window(handle, x, y);
	} else {
		apply_hints(handle);
	}
}

static MwLL hit_test(MwLL handle, int* x, int* y) {
	int i;

	for(i = arrlen(handle->cocoa.children) - 1; i >= 0; i--) {
		MwLL c = handle->cocoa.children[i];

		if(!c->cocoa.shown) {
			continue;
		}

		if(*x >= c->cocoa.x && *y >= c->cocoa.y && *x < c->cocoa.x + c->cocoa.width && *y < c->cocoa.y + c->cocoa.height) {
			*x -= c->cocoa.x;
			*y -= c->cocoa.y;
			return hit_test(c, x, y);
		}
	}

	return handle;
}

static MwLL hit_top(MwLL top, int gx, int gy) {
	int ox, oy, x, y;

	if(top == NULL || ![top->cocoa.window isVisible]) {
		return NULL;
	}

	content_origin(top, &ox, &oy);
	x = gx - ox;
	y = gy - oy;
	if(x < 0 || y < 0 || x >= top->cocoa.width || y >= top->cocoa.height) {
		return NULL;
	}

	return hit_test(top, &x, &y);
}

static MwLL handle_at(int gx, int gy) {
	NSArray*     windows = [NSApp windows];
	unsigned int i;
	MwLL	     h;

	for(i = 0; i < [windows count]; i++) {
		NSWindow* w = [windows objectAtIndex:i];

		if([w level] > NSNormalWindowLevel && (h = hit_top(from_window(w), gx, gy)) != NULL) {
			return h;
		}
	}

	windows = [NSApp orderedWindows];
	for(i = 0; i < [windows count]; i++) {
		if((h = hit_top(from_window([windows objectAtIndex:i]), gx, gy)) != NULL) {
			return h;
		}
	}

	return NULL;
}

static void global_to_handle(MwLL handle, int gx, int gy, int* x, int* y) {
	MwLL top = get_top(handle);
	int  ox, oy, hx, hy;

	if(top == NULL) {
		*x = 0;
		*y = 0;
		return;
	}

	content_origin(top, &ox, &oy);
	get_offset(handle, &hx, &hy);

	*x = gx - ox - hx;
	*y = gy - oy - hy;
}

static void event_global(NSEvent* ev, int* x, int* y) {
	NSPoint	  p = [ev locationInWindow];
	NSWindow* w = [ev window];

	if(w != nil) {
		NSRect f = [w frame];

		p.x += f.origin.x;
		p.y += f.origin.y;
	}

	*x = (int)floor(p.x);
	*y = (int)floor(primary_frame().size.height - p.y);
}

static int event_button(NSEvent* ev) {
	switch([ev type]) {
	case NSRightMouseDown:
	case NSRightMouseUp:
		return MwMOUSE_RIGHT;
	case NSOtherMouseDown:
	case NSOtherMouseUp:
		return MwMOUSE_MIDDLE;
	default:
		if([ev modifierFlags] & NSControlKeyMask) {
			return MwMOUSE_RIGHT;
		}
		return MwMOUSE_LEFT;
	}
}

static MwLL grabbing_top(void) {
	MwLL top = from_window([NSApp keyWindow]);

	if(top == NULL || top->cocoa.grab == NULL) {
		return NULL;
	}

	return top;
}

static MwLL pointer_target(int gx, int gy) {
	MwLL top = grabbing_top();

	if(top != NULL) {
		return top->cocoa.grab;
	}

	return handle_at(gx, gy);
}

static MwLL key_target(void) {
	MwLL top = from_window([NSApp keyWindow]);

	if(top == NULL || top->cocoa.focus == NULL) {
		return top;
	}

	return top->cocoa.focus;
}

@implementation MilskoCocoaView
- (BOOL)isOpaque {
	return YES;
}

- (BOOL)isFlipped {
	return NO;
}

- (BOOL)acceptsFirstMouse:(NSEvent*)ev {
	(void)ev;
	return YES;
}

- (BOOL)mouseDownCanMoveWindow {
	return NO;
}

- (void)drawRect:(NSRect)rect {
	NSRect		  b = [self bounds];
	size_t		  size;
	CGColorSpaceRef	  rgb;
	CGContextRef	  port;
	CGDataProviderRef provider;
	CGImageRef	  image;

	(void)rect;

	if(mw == NULL || !mw->common.copy_buffer || mw->cocoa.context == NULL) {
		return;
	}

	CGContextFlush(mw->cocoa.context);
	size = (size_t)mw->cocoa.width * mw->cocoa.height * 4;

	port	 = [[NSGraphicsContext currentContext] graphicsPort];
	rgb	 = CGColorSpaceCreateDeviceRGB();
	provider = CGDataProviderCreateWithData(NULL, mw->cocoa.buffer, size, NULL);
	image	 = CGImageCreate(mw->cocoa.width, mw->cocoa.height, 8, 32, mw->cocoa.width * 4, rgb, BITMAP_INFO, provider, NULL, false, kCGRenderingIntentDefault);

	CGContextSetInterpolationQuality(port, kCGInterpolationNone);
	CGContextDrawImage(port, CGRectMake(b.origin.x, b.origin.y, b.size.width, b.size.height), image);

	CGImageRelease(image);
	CGDataProviderRelease(provider);
	CGColorSpaceRelease(rgb);
}

- (void)resetCursorRects {
	if(mw != NULL && mw->cocoa.cursor != nil) {
		[self addCursorRect:[self visibleRect] cursor:mw->cocoa.cursor];
	}
}

- (void)mouseDown:(NSEvent*)ev {
	(void)ev;
}
- (void)rightMouseDown:(NSEvent*)ev {
	(void)ev;
}
- (void)otherMouseDown:(NSEvent*)ev {
	(void)ev;
}
- (void)keyDown:(NSEvent*)ev {
	(void)ev;
}
- (void)keyUp:(NSEvent*)ev {
	(void)ev;
}

- (MwLL)dropTargetFor:(id)info {
	NSPoint p = [info draggingLocation];
	NSRect	f = [[self window] frame];
	MwLL	h = handle_at((int)floor(f.origin.x + p.x), (int)floor(primary_frame().size.height - (f.origin.y + p.y)));

	while(h != NULL && !h->cocoa.accepts_dnd) {
		h = h->cocoa.parent;
	}

	return h;
}

- (NSDragOperation)draggingEntered:(id)info {
	if([self dropTargetFor:info] == NULL) {
		return NSDragOperationNone;
	}

	return NSDragOperationCopy;
}

- (NSDragOperation)draggingUpdated:(id)info {
	return [self draggingEntered:info];
}

- (BOOL)performDragOperation:(id)info {
	MwLL	     t	   = [self dropTargetFor:info];
	NSArray*     files = [[info draggingPasteboard] propertyListForType:NSFilenamesPboardType];
	unsigned int i;

	if(t == NULL || files == nil || ![files isKindOfClass:[NSArray class]]) {
		return NO;
	}

	for(i = 0; i < [files count]; i++) {
		new_event(CE_DROP, t)->data = string_dup([files objectAtIndex:i]);
	}

	return YES;
}
@end

@implementation MilskoCocoaWindow
- (BOOL)canBecomeKeyWindow {
	return mw == NULL || !mw->cocoa.tool;
}

- (BOOL)canBecomeMainWindow {
	return mw == NULL || (!mw->cocoa.tool && !mw->cocoa.borderless);
}

- (void)keyDown:(NSEvent*)ev {
	(void)ev;
}

- (void)keyUp:(NSEvent*)ev {
	(void)ev;
}

- (BOOL)windowShouldClose:(id)sender {
	(void)sender;

	if(mw != NULL) {
		push_unique(CE_CLOSE, mw);
	}

	return NO;
}

- (void)windowDidResize:(NSNotification*)notification {
	(void)notification;

	if(mw != NULL) {
		after_resize(mw);
	}
}

- (void)windowDidResignKey:(NSNotification*)notification {
	(void)notification;

	if(mw != NULL && mw->cocoa.focus != NULL) {
		push_unique(CE_FOCUSOUT, mw->cocoa.focus);
		mw->cocoa.focus = NULL;
	}
}
@end

@implementation MilskoCocoaObserver
- (void)quit:(id)sender {
	(void)sender;

	quit_all();
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)app {
	(void)app;

	quit_all();

	return NSTerminateCancel;
}

- (void)themeChanged:(NSNotification*)notification {
	NSArray*     windows = [NSApp windows];
	unsigned int i;

	(void)notification;

	[[NSUserDefaults standardUserDefaults] synchronize];
	for(i = 0; i < [windows count]; i++) {
		MwLL h = from_window([windows objectAtIndex:i]);

		if(h != NULL) {
			push_unique(CE_DARK, h);
		}
	}
}
@end

static void after_resize(MwLL top) {
	NSRect r = [[top->cocoa.window contentView] frame];
	int    w = (int)r.size.width;
	int    h = (int)r.size.height;

	if(w < 1) {
		w = 1;
	}
	if(h < 1) {
		h = 1;
	}
	if(w == top->cocoa.width && h == top->cocoa.height) {
		return;
	}

	create_context(top, w, h);
	place_children(top);

	push_unique(CE_RESIZE, top);
	push_unique(CE_DRAW, top);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL		   r = malloc(sizeof(*r));
	NSAutoreleasePool* pool;

	if(r == NULL) {
		return NULL;
	}

	pool = [[NSAutoreleasePool alloc] init];

	memset(r, 0, sizeof(*r));
	MwLLCreateCommon(r);

	r->common.copy_buffer	  = 1;
	r->common.type		  = MwLLBackendCocoa;
	r->common.coordinate_type = MwCOORDINATE_GLOBAL;

	if(width < 1) {
		width = 1;
	}
	if(height < 1) {
		height = 1;
	}

	r->cocoa.parent	   = parent;
	r->cocoa.shown	   = 1;
	r->cocoa.last_pump = -1;
	create_context(r, width, height);

	r->cocoa.view	  = [[MilskoCocoaView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
	r->cocoa.view->mw = r;

	if(parent == NULL) {
		NSRect s = [[NSScreen mainScreen] visibleFrame];

		if(x == MwDEFAULT) {
			x = (int)(s.origin.x + (s.size.width - width) / 2);
		}
		if(y == MwDEFAULT) {
			y = (int)(primary_frame().size.height - (s.origin.y + s.size.height) + (s.size.height - height) / 2);
		}

		create_window(r, x, y);
		push_unique(CE_DARK, r);
	} else {
		r->cocoa.x = 0;
		r->cocoa.y = 0;
		if(x != MwDEFAULT) {
			r->cocoa.x = x;
		}
		if(y != MwDEFAULT) {
			r->cocoa.y = y;
		}

		arrput(parent->cocoa.children, r);
		place_view(r);
		[parent->cocoa.view addSubview:r->cocoa.view];
	}

	push_unique(CE_DRAW, r);

	[pool release];

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLL		   parent = handle->cocoa.parent;
	MwLL		   top	  = get_top(handle);
	NSAutoreleasePool* pool	  = [[NSAutoreleasePool alloc] init];
	int		   i;

	MwLLDestroyCommon(handle);

	if(top != NULL) {
		forget(top, handle);
	}

	if(parent != NULL) {
		for(i = 0; i < arrlen(parent->cocoa.children); i++) {
			if(parent->cocoa.children[i] == handle) {
				arrdel(parent->cocoa.children, i);
				break;
			}
		}
	}

	for(i = 0; i < arrlen(handle->cocoa.children); i++) {
		handle->cocoa.children[i]->cocoa.parent = NULL;
	}
	arrfree(handle->cocoa.children);

	for(i = 0; i < arrlen(handle->cocoa.events); i++) {
		free(handle->cocoa.events[i].data);
	}
	arrfree(handle->cocoa.events);

	handle->cocoa.view->mw = NULL;
	[handle->cocoa.view removeFromSuperview];

	if(handle->cocoa.window != nil) {
		MilskoCocoaWindow* w = handle->cocoa.window;

		w->mw = NULL;
		[w setDelegate:nil];
		unparent_window(w);
		[w orderOut:nil];
		[w setContentView:nil];
		[w release];
	}

	[handle->cocoa.view release];
	[handle->cocoa.title release];
	[handle->cocoa.cursor release];
	[handle->cocoa.popup_owner release];
	if(handle->cocoa.context != NULL) {
		CGContextRelease(handle->cocoa.context);
	}
	free(handle->cocoa.buffer);

	free(handle);

	[pool release];
}

static void MwLLBeginDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndDrawImpl(MwLL handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	mark_dirty(handle);

	[pool release];
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	CGContextRef c = handle->cocoa.context;
	int	     i;

	if(points_count < 3 || c == NULL) {
		return;
	}

	CGContextBeginPath(c);
	CGContextMoveToPoint(c, points[0].x, points[0].y);
	for(i = 1; i < points_count; i++) {
		CGContextAddLineToPoint(c, points[i].x, points[i].y);
	}
	CGContextClosePath(c);

	CGContextSetRGBFillColor(c, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0, 1.0);
	CGContextEOFillPath(c);

	mark_dirty(handle);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	CGContextRef c = handle->cocoa.context;

	if(c == NULL) {
		return;
	}

	CGContextBeginPath(c);
	CGContextMoveToPoint(c, points[0].x + 0.5, points[0].y + 0.5);
	CGContextAddLineToPoint(c, points[1].x + 0.5, points[1].y + 0.5);

	CGContextSetRGBStrokeColor(c, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0, 1.0);
	CGContextStrokePath(c);

	mark_dirty(handle);
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	(void)handle;

	if(c == NULL) {
		return NULL;
	}

	MwLLColorUpdate(c, r, g, b);

	return c;
}

static void MwLLColorUpdateImpl(MwLLColor c, int r, int g, int b) {
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	if(handle->cocoa.window != nil) {
		content_origin(handle, x, y);
	} else {
		*x = handle->cocoa.x;
		*y = handle->cocoa.y;
	}

	*w = handle->cocoa.width;
	*h = handle->cocoa.height;

	[pool release];
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSWindow*	   w	= handle->cocoa.window;

	if(w != nil) {
		[w setFrame:[w frameRectForContentRect:NSMakeRect(x, primary_frame().size.height - y - handle->cocoa.height, handle->cocoa.width, handle->cocoa.height)] display:NO];
	} else {
		handle->cocoa.x = x;
		handle->cocoa.y = y;
		place_view(handle);
		if(handle->cocoa.parent != NULL) {
			mark_dirty(handle->cocoa.parent);
		}
		push_draw_tree(handle);
	}

	[pool release];
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSWindow*	   win	= handle->cocoa.window;
	int		   x, y;

	if(w < 1) {
		w = 1;
	}
	if(h < 1) {
		h = 1;
	}

	if(win != nil) {
		content_origin(handle, &x, &y);
		[win setFrame:[win frameRectForContentRect:NSMakeRect(x, primary_frame().size.height - y - h, w, h)] display:NO];
		after_resize(handle);
	} else if(handle->cocoa.width != w || handle->cocoa.height != h) {
		create_context(handle, w, h);
		place_view(handle);
		place_children(handle);
		if(handle->cocoa.parent != NULL) {
			mark_dirty(handle->cocoa.parent);
		}

		push_unique(CE_RESIZE, handle);
		push_unique(CE_DRAW, handle);
	}

	[pool release];
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[handle->cocoa.title release];
	handle->cocoa.title = [utf8_string(title) retain];

	if(handle->cocoa.window != nil) {
		[handle->cocoa.window setTitle:handle->cocoa.title];
	}

	[pool release];
}

static int MwLLPendingImpl(MwLL handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	long		   now	= MwTimeGetTick();
	NSEvent*	   ev;
	MwLL		   top;

	if(now != handle->cocoa.last_pump) {
		handle->cocoa.last_pump = now;

		while((ev = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES]) != nil) {
			int  forward = 1;
			int  gx, gy, x, y;
			MwLL t;
			MwLL et;

			switch([ev type]) {
			case NSLeftMouseDown:
			case NSRightMouseDown:
			case NSOtherMouseDown:
				event_global(ev, &gx, &gy);
				if((t = pointer_target(gx, gy)) == NULL) {
					break;
				}

				get_top(t)->cocoa.capture	 = t;
				get_top(t)->cocoa.capture_button = event_button(ev);
				set_focus(t);

				global_to_handle(t, gx, gy, &x, &y);
				push_mouse(CE_DOWN, t, x, y, event_button(ev));
				break;
			case NSLeftMouseUp:
			case NSRightMouseUp:
			case NSOtherMouseUp:
			{
				int button = event_button(ev);

				event_global(ev, &gx, &gy);
				et = from_window([ev window]);
				if(et == NULL || et->cocoa.capture == NULL) {
					break;
				}

				t		  = et->cocoa.capture;
				button		  = et->cocoa.capture_button;
				et->cocoa.capture = NULL;

				global_to_handle(t, gx, gy, &x, &y);
				push_mouse(CE_UP, t, x, y, button);
				break;
			}
			case NSMouseMoved:
			case NSLeftMouseDragged:
			case NSRightMouseDragged:
			case NSOtherMouseDragged:
				if((et = grabbing_top()) != NULL) {
					x = (int)[ev deltaX];
					y = (int)[ev deltaY];
					if(x != 0 || y != 0) {
						push_move(et->cocoa.grab, x, y, 1);
					}
					break;
				}

				event_global(ev, &gx, &gy);
				et = from_window([ev window]);
				if(et != NULL && et->cocoa.capture != NULL) {
					t = et->cocoa.capture;
				} else {
					t = handle_at(gx, gy);
				}

				if(t != NULL) {
					MwLL tt = get_top(t);

					if(t->cocoa.cursor != nil) {
						[t->cocoa.cursor set];
					} else if(tt->cocoa.hover != NULL && tt->cocoa.hover != t) {
						[[NSCursor arrowCursor] set];
					}
					tt->cocoa.hover = t;

					global_to_handle(t, gx, gy, &x, &y);
					push_move(t, x, y, 0);
				}
				break;
			case NSScrollWheel:
			{
				MwLL tt;
				int  n;

				event_global(ev, &gx, &gy);
				if((t = pointer_target(gx, gy)) == NULL) {
					break;
				}

				tt = get_top(t);
				tt->cocoa.scroll_acc += [ev deltaY];
				n = (int)tt->cocoa.scroll_acc;
				tt->cocoa.scroll_acc -= n;
				if(n > 5) {
					n = 5;
				}
				if(n < -5) {
					n = -5;
				}

				global_to_handle(t, gx, gy, &x, &y);
				for(; n > 0; n--) {
					push_mouse(CE_DOWN, t, x, y, MwMOUSE_WHEELUP);
					push_mouse(CE_UP, t, x, y, MwMOUSE_WHEELUP);
				}
				for(; n < 0; n++) {
					push_mouse(CE_DOWN, t, x, y, MwMOUSE_WHEELDOWN);
					push_mouse(CE_UP, t, x, y, MwMOUSE_WHEELDOWN);
				}
				break;
			}
			case NSKeyDown:
			case NSKeyUp:
			{
				int	     down  = [ev type] == NSKeyDown;
				unsigned int flags = [ev modifierFlags];
				NSString*    s	   = [ev charactersIgnoringModifiers];
				unsigned int i;
				unichar	     c;
				int	     n = 0;

				forward = 0;

				if(down && (flags & NSCommandKeyMask) && [[NSApp mainMenu] performKeyEquivalent:ev]) {
					break;
				}

				if((t = key_target()) == NULL || s == nil || [s length] == 0) {
					break;
				}

				c = [s characterAtIndex:0];
				switch(c) {
				case NSUpArrowFunctionKey:
					n = MwKEY_UP;
					break;
				case NSDownArrowFunctionKey:
					n = MwKEY_DOWN;
					break;
				case NSLeftArrowFunctionKey:
					n = MwKEY_LEFT;
					break;
				case NSRightArrowFunctionKey:
					n = MwKEY_RIGHT;
					break;
				case NSCarriageReturnCharacter:
				case NSNewlineCharacter:
				case NSEnterCharacter:
					n = MwKEY_ENTER;
					break;
				case NSDeleteCharacter:
				case NSBackspaceCharacter:
					n = MwKEY_BACKSPACE;
					break;
				case 0x1b:
					n = MwKEY_ESCAPE;
					break;
				}

				if(n != 0) {
					push_key(down, t, n);
					break;
				}

				if(c >= NSUpArrowFunctionKey && c <= 0xf8ff) {
					break;
				}

				if(flags & (NSControlKeyMask | NSCommandKeyMask)) {
					if(c >= 'A' && c <= 'Z') {
						c += 'a' - 'A';
					}
					push_key(down, t, c | MwKEY_CONTROL_FLAG);
					break;
				}

				if([ev characters] != nil && [[ev characters] length] > 0) {
					s = [ev characters];
				}

				for(i = 0; i < [s length]; i++) {
					int cp = [s characterAtIndex:i];

					if(cp >= 0xd800 && cp < 0xdc00 && i + 1 < [s length]) {
						int lo = [s characterAtIndex:i + 1];

						if(lo >= 0xdc00 && lo < 0xe000) {
							cp = 0x10000 + ((cp - 0xd800) << 10) + (lo - 0xdc00);
							i++;
						}
					}
					if(cp < 0x20 && cp != '\t') {
						continue;
					}

					push_key(down, t, cp);

					if(!down) {
						break;
					}
				}
				break;
			}
			case NSFlagsChanged:
			{
				enum {
					device_left_shift  = 0x2,
					device_right_shift = 0x4
				};
				const struct {
					unsigned int mask;
					int	     key;
				} mods[] = {
				    {device_left_shift, MwKEY_LEFTSHIFT},
				    {device_right_shift, MwKEY_RIGHTSHIFT},
				    {NSControlKeyMask, MwKEY_CONTROL},
				    {NSAlternateKeyMask, MwKEY_ALT}};
				unsigned int flags = [ev modifierFlags];
				int	     i;

				forward = 0;

				if((t = key_target()) == NULL) {
					break;
				}
				et = get_top(t);

				if((flags & NSShiftKeyMask) && !(flags & (device_left_shift | device_right_shift))) {
					flags |= device_left_shift;
				}

				for(i = 0; i < (int)(sizeof(mods) / sizeof(mods[0])); i++) {
					if((flags ^ et->cocoa.last_flags) & mods[i].mask) {
						push_key(flags & mods[i].mask, t, mods[i].key);
					}
				}

				et->cocoa.last_flags = flags;
				break;
			}
			default:
				break;
			}

			if(forward) {
				[NSApp sendEvent:ev];
			}
		}
	}

	if((top = get_top(handle)) != NULL) {
		if(top->cocoa.pending_show) {
			MilskoCocoaWindow* w = top->cocoa.window;

			top->cocoa.pending_show = 0;

			if(top->cocoa.popup_owner != nil && [top->cocoa.popup_owner isVisible] && parent_window(w) == nil && [w respondsToSelector:@selector(addChildWindow:ordered:)]) {
				[top->cocoa.popup_owner addChildWindow:w ordered:NSWindowAbove];
			}

			if(top->cocoa.tool) {
				[w orderFront:nil];
			} else {
				[w makeKeyAndOrderFront:nil];
			}
		}

		if(top->cocoa.dirty) {
			top->cocoa.dirty = 0;
			[top->cocoa.window displayIfNeeded];
		}
	}

	[pool release];

	return arrlen(handle->cocoa.events) > 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	NSAutoreleasePool* pool;
	MwLLCocoaEvent	   e;
	MwU32		   dark;

	if(arrlen(handle->cocoa.events) == 0) {
		return;
	}

	e = handle->cocoa.events[0];
	arrdel(handle->cocoa.events, 0);

	pool = [[NSAutoreleasePool alloc] init];

	switch(e.type) {
	case CE_DRAW:
		MwLLDispatch(handle, draw, NULL);
		break;
	case CE_RESIZE:
		MwLLDispatch(handle, resize, NULL);
		break;
	case CE_DOWN:
		MwLLDispatch(handle, down, &e.mouse);
		break;
	case CE_UP:
		MwLLDispatch(handle, up, &e.mouse);
		break;
	case CE_MOVE:
		MwLLDispatch(handle, move, &e.mouse.point);
		break;
	case CE_KEY:
		MwLLDispatch(handle, key, &e.key);
		break;
	case CE_KEYUP:
		MwLLDispatch(handle, key_released, &e.key);
		break;
	case CE_FOCUSIN:
		MwLLDispatch(handle, focus_in, NULL);
		break;
	case CE_FOCUSOUT:
		MwLLDispatch(handle, focus_out, NULL);
		break;
	case CE_CLOSE:
		MwLLDispatch(handle, close, NULL);
		break;
	case CE_CLIPBOARD:
	{
		NSPasteboard* pb   = [NSPasteboard generalPasteboard];
		NSString*     type = [pb availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
		NSString*     s;
		char*	      p;

		if(type == nil || (s = [pb stringForType:type]) == nil) {
			break;
		}

		e.data = string_dup(s);
		for(p = e.data; *p != 0; p++) {
			if(*p == '\r') {
				*p = '\n';
			}
		}

		MwLLDispatch(handle, clipboard, e.data);
		break;
	}
	case CE_DROP:
		MwLLDispatch(handle, drag_and_drop, e.data);
		break;
	case CE_DARK:
	{
		NSString* s = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];

		dark = 0;
		if(handle->common.theme_override != 0) {
			dark = handle->common.theme_override == 2;
		} else if(s != nil && [s caseInsensitiveCompare:@"Dark"] == NSOrderedSame) {
			dark = 1;
		}

		MwLLDispatch(handle, dark_theme, &dark);
		break;
	}
	}

	free(e.data);

	[pool release];
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));
	size_t	   size;

	(void)handle;

	if(r == NULL) {
		return NULL;
	}

	if(width < 1) {
		width = 1;
	}
	if(height < 1) {
		height = 1;
	}
	size = (size_t)4 * width * height;

	memset(r, 0, sizeof(*r));
	r->common.width	       = width;
	r->common.height       = height;
	r->common.raw	       = malloc(size);
	r->cocoa.premultiplied = malloc(size);
	if(r->common.raw == NULL || r->cocoa.premultiplied == NULL) {
		free(r->common.raw);
		free(r->cocoa.premultiplied);
		free(r);
		return NULL;
	}
	memcpy(r->common.raw, data, size);

	MwLLPixmapUpdate(r);

	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {
	size_t		  n = (size_t)pixmap->common.width * pixmap->common.height;
	MwU32*		  d = (MwU32*)pixmap->cocoa.premultiplied;
	CGDataProviderRef provider;
	CGColorSpaceRef	  rgb;
	size_t		  i;

	for(i = 0; i < n; i++) {
		unsigned char* s = &pixmap->common.raw[i * 4];
		MwU32	       a = s[3];

		d[i] = (a << 24) | ((s[0] * a / 255) << 16) | ((s[1] * a / 255) << 8) | (s[2] * a / 255);
	}

	if(pixmap->cocoa.image != NULL) {
		CGImageRelease(pixmap->cocoa.image);
	}

	rgb		    = CGColorSpaceCreateDeviceRGB();
	provider	    = CGDataProviderCreateWithData(NULL, pixmap->cocoa.premultiplied, n * 4, NULL);
	pixmap->cocoa.image = CGImageCreate(pixmap->common.width, pixmap->common.height, 8, 32, pixmap->common.width * 4, rgb, BITMAP_INFO, provider, NULL, false, kCGRenderingIntentDefault);
	CGDataProviderRelease(provider);
	CGColorSpaceRelease(rgb);
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	if(pixmap->cocoa.image != NULL) {
		CGImageRelease(pixmap->cocoa.image);
	}
	free(pixmap->cocoa.premultiplied);
	free(pixmap->common.raw);
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	CGContextRef c = handle->cocoa.context;

	if(rect->width <= 0 || rect->height <= 0 || c == NULL || pixmap->cocoa.image == NULL) {
		return;
	}

	CGContextSaveGState(c);
	CGContextTranslateCTM(c, rect->x, rect->y + rect->height);
	CGContextScaleCTM(c, 1, -1);
	CGContextDrawImage(c, CGRectMake(0, 0, rect->width, rect->height), pixmap->cocoa.image);
	CGContextRestoreGState(c);

	mark_dirty(handle);
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSBitmapImageRep*  rep;
	NSImage*	   image;
	unsigned char*	   d;
	int		   i;

	(void)handle;

	rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL pixelsWide:pixmap->common.width pixelsHigh:pixmap->common.height bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES isPlanar:NO colorSpaceName:NSDeviceRGBColorSpace bytesPerRow:pixmap->common.width * 4 bitsPerPixel:32];
	if(rep == nil) {
		[pool release];
		return;
	}

	d = [rep bitmapData];
	for(i = 0; i < pixmap->common.width * pixmap->common.height; i++) {
		unsigned char* s = &pixmap->common.raw[i * 4];

		d[i * 4 + 0] = s[0] * s[3] / 255;
		d[i * 4 + 1] = s[1] * s[3] / 255;
		d[i * 4 + 2] = s[2] * s[3] / 255;
		d[i * 4 + 3] = s[3];
	}

	image = [[NSImage alloc] initWithSize:NSMakeSize(pixmap->common.width, pixmap->common.height)];
	[image addRepresentation:rep];
	[NSApp setApplicationIconImage:image];

	[image release];
	[rep release];

	[pool release];
}

static void MwLLForceRenderImpl(MwLL handle) {
	push_unique(CE_DRAW, handle);
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	MwLL		   top	= get_top(handle);
	NSBitmapImageRep*  rep;
	NSImage*	   img;
	unsigned char*	   d;
	int		   x, y, xs, ys, hx, hy;

	rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL pixelsWide:MwCursorDataHeight pixelsHigh:MwCursorDataHeight bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES isPlanar:NO colorSpaceName:NSDeviceRGBColorSpace bytesPerRow:MwCursorDataHeight * 4 bitsPerPixel:32];
	if(rep == nil) {
		[pool release];
		return;
	}

	d = [rep bitmapData];
	memset(d, 0, MwCursorDataHeight * MwCursorDataHeight * 4);

	xs = -mask->x + image->x;
	ys = mask->height + mask->y;
	ys = image->height + image->y - ys;
	if(ys < 0) {
		ys = -ys;
	}

	for(y = 0; y < mask->height && y < MwCursorDataHeight; y++) {
		unsigned int l = mask->data[y];

		for(x = mask->width - 1; x >= 0; x--) {
			if((l & 1) && x < MwCursorDataHeight) {
				d[(y * MwCursorDataHeight + x) * 4 + 3] = 255;
			}
			l = l >> 1;
		}
	}
	for(y = 0; y < image->height; y++) {
		unsigned int l = image->data[y];

		for(x = image->width - 1; x >= 0; x--) {
			int px = xs + x;
			int py = ys + y;

			if((l & 1) && px >= 0 && px < MwCursorDataHeight && py >= 0 && py < MwCursorDataHeight) {
				memset(&d[(py * MwCursorDataHeight + px) * 4], 255, 4);
			}
			l = l >> 1;
		}
	}

	img = [[NSImage alloc] initWithSize:NSMakeSize(MwCursorDataHeight, MwCursorDataHeight)];
	[img addRepresentation:rep];

	hx = xs - image->x;
	if(hx < 0) {
		hx = 0;
	}
	if(hx >= MwCursorDataHeight) {
		hx = MwCursorDataHeight - 1;
	}
	hy = ys + (MwCursorDataHeight + image->y);
	if(hy < 0) {
		hy = 0;
	}
	if(hy >= MwCursorDataHeight) {
		hy = MwCursorDataHeight - 1;
	}

	[handle->cocoa.cursor release];
	handle->cocoa.cursor = [[NSCursor alloc] initWithImage:img hotSpot:NSMakePoint(hx, hy)];

	[img release];
	[rep release];

	if([handle->cocoa.view window] != nil) {
		[[handle->cocoa.view window] invalidateCursorRectsForView:handle->cocoa.view];
	}

	if(top != NULL && (top->cocoa.hover == handle || top->cocoa.grab == handle)) {
		[handle->cocoa.cursor set];
	}

	[pool release];
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	MwLL		   parent = handle->cocoa.parent;
	MwLL		   top;
	NSAutoreleasePool* pool;
	int		   ox, oy, x, y, i;

	if(parent == NULL || (top = get_top(parent)) == NULL) {
		return;
	}

	pool = [[NSAutoreleasePool alloc] init];

	forget(top, handle);
	content_origin(top, &ox, &oy);
	get_offset(parent, &x, &y);

	for(i = 0; i < arrlen(parent->cocoa.children); i++) {
		if(parent->cocoa.children[i] == handle) {
			arrdel(parent->cocoa.children, i);
			break;
		}
	}

	[handle->cocoa.view removeFromSuperview];
	handle->cocoa.parent = NULL;

	create_window(handle, ox + x + point->x, oy + y + point->y);

	push_unique(CE_DRAW, handle);

	[pool release];
}

static void MwLLShowImpl(MwLL handle, int show) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	if(handle->cocoa.window != nil) {
		handle->cocoa.shown = show;
		if(show) {
			handle->cocoa.pending_show = 1;
		} else {
			handle->cocoa.pending_show = 0;
			unparent_window(handle->cocoa.window);
			[handle->cocoa.window orderOut:nil];
		}
	} else if(handle->cocoa.shown != show) {
		handle->cocoa.shown = show;
		if(handle->cocoa.parent != NULL) {
			restack(handle->cocoa.parent);
		}
		if(show) {
			push_unique(CE_DRAW, handle);
		}
	}

	[pool release];
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	MwLL top = NULL;

	if(parent != NULL) {
		top = get_top(parent);
	}

	[handle->cocoa.popup_owner release];
	handle->cocoa.popup_owner = nil;
	if(top != NULL) {
		handle->cocoa.popup_owner = [top->cocoa.window retain];
	}
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	handle->cocoa.min_w = minx;
	handle->cocoa.min_h = miny;
	handle->cocoa.max_w = maxx;
	handle->cocoa.max_h = maxy;

	restyle(handle);

	[pool release];
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	handle->cocoa.borderless = toggle;

	restyle(handle);

	[pool release];
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	handle->cocoa.tool = 1;

	restyle(handle);

	[pool release];
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	handle->cocoa.in_state_change = 1;
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	handle->cocoa.in_state_change = 0;

	restyle(handle);

	[pool release];
}

static void MwLLFocusImpl(MwLL handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	MwLL		   top	= get_top(handle);

	if(top != NULL && !top->cocoa.tool && top->cocoa.shown && !top->cocoa.pending_show && ![top->cocoa.window isKeyWindow]) {
		[top->cocoa.window makeKeyAndOrderFront:nil];
	}

	set_focus(handle);

	[pool release];
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	MwLL		   top	= get_top(handle);
	int		   ox, oy, hx, hy;
	CGPoint		   p;

	if(top != NULL && toggle) {
		top->cocoa.grab = handle;
		set_focus(handle);

		content_origin(top, &ox, &oy);
		get_offset(handle, &hx, &hy);
		p.x = ox + hx + handle->cocoa.width / 2;
		p.y = oy + hy + handle->cocoa.height / 2;
		CGWarpMouseCursorPosition(p);
		CGAssociateMouseAndMouseCursorPosition(false);

		if(handle->cocoa.cursor != nil) {
			[handle->cocoa.cursor set];
		}
	} else if(top != NULL && top->cocoa.grab == handle) {
		top->cocoa.grab = NULL;
		CGAssociateMouseAndMouseCursorPosition(true);
	}

	[pool release];
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text, int clipboard_type) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSPasteboard*	   pb	= [NSPasteboard generalPasteboard];

	(void)handle;
	(void)clipboard_type;

	[pb declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
	[pb setString:utf8_string(text) forType:NSStringPboardType];

	[pool release];
}

static void MwLLGetClipboardImpl(MwLL handle, int clipboard_type) {
	(void)clipboard_type;

	push_unique(CE_CLIPBOARD, handle);
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSPoint		   p	= [NSEvent mouseLocation];

	(void)handle;

	point->x = (int)floor(p.x);
	point->y = (int)floor(primary_frame().size.height - p.y);

	[pool release];
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSRect		   f	= primary_frame();

	(void)handle;

	rect->x	     = 0;
	rect->y	     = 0;
	rect->width  = (int)f.size.width;
	rect->height = (int)f.size.height;

	[pool release];
}

static void MwLLSetupDragAndDropImpl(MwLL handle) {
	handle->cocoa.accepts_dnd = 1;
}

static void MwLLSetDarkThemeImpl(MwLL handle, int toggle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	MwLL		   top	= get_top(handle);
	Class		   appearance;
	NSString*	   name = @"NSAppearanceNameAqua";

	if(toggle) {
		name = @"NSAppearanceNameDarkAqua";
	}

	if(top != NULL && (appearance = NSClassFromString(@"NSAppearance")) != nil && [top->cocoa.window respondsToSelector:@selector(setAppearance:)]) {
		[top->cocoa.window performSelector:@selector(setAppearance:) withObject:[appearance performSelector:@selector(appearanceNamed:) withObject:name]];
	}

	[pool release];
}

static MwBool MwLLDoModernImpl(MwLL handle) {
	(void)handle;

	return MwTRUE;
}

static void MwLLRaiseImpl(MwLL handle) {
	NSAutoreleasePool* pool	  = [[NSAutoreleasePool alloc] init];
	MwLL		   parent = handle->cocoa.parent;
	int		   i;

	if(handle->cocoa.window != nil) {
		if(handle->cocoa.shown && !handle->cocoa.pending_show) {
			[handle->cocoa.window orderFront:nil];
		}
	} else if(parent != NULL) {
		for(i = 0; i < arrlen(parent->cocoa.children); i++) {
			if(parent->cocoa.children[i] == handle && i != arrlen(parent->cocoa.children) - 1) {
				arrdel(parent->cocoa.children, i);
				arrput(parent->cocoa.children, handle);
				restack(parent);
				break;
			}
		}
	}

	[pool release];
}

static void MwLLClipImpl(MwLL handle, MwRect* rect) {
	if(rect == NULL) {
		handle->cocoa.has_clip = 0;
	} else {
		handle->cocoa.has_clip = 1;
		handle->cocoa.clip     = *rect;
	}

	if(handle->cocoa.context != NULL) {
		apply_clip(handle);
	}
}

NSView* MwLLCocoaGetView(MwLL handle) {
	return handle->cocoa.view;
}

static int MwLLCocoaCallInitImpl(void) {
	NSAutoreleasePool*   pool = [[NSAutoreleasePool alloc] init];
	MilskoCocoaObserver* observer;
	NSString*	     name;
	NSMenu*		     bar;
	NSMenuItem*	     item;
	NSMenu*		     app;
	enum {
		current_process		= 2,
		transform_to_foreground = 1
	};
	int (*transform)(void*, unsigned int) = NULL;
	struct {
		unsigned int high;
		unsigned int low;
	} psn = {0, current_process};

	[NSApplication sharedApplication];

	if([[NSApp delegate] isKindOfClass:[MilskoCocoaObserver class]]) {
		[pool release];
		return 0;
	}

#if defined(MAC_OS_X_VERSION_MAX_ALLOWED) && MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
	if([NSApp respondsToSelector:@selector(setActivationPolicy:)]) {
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	} else
#endif
	{
		transform = (int (*)(void*, unsigned int))MwDynamicSymbol(NULL, "TransformProcessType");
		if(transform != NULL) {
			transform(&psn, transform_to_foreground);
		}
	}

	observer = [[MilskoCocoaObserver alloc] init];
	[NSApp setDelegate:observer];

	name = [[NSProcessInfo processInfo] processName];
	bar  = [[NSMenu alloc] initWithTitle:@""];
	app  = [[NSMenu alloc] initWithTitle:name];
	item = [[NSMenuItem alloc] initWithTitle:name action:NULL keyEquivalent:@""];
	[item setSubmenu:app];
	[bar addItem:item];
	[item release];

	item = [app addItemWithTitle:[@"Hide " stringByAppendingString:name] action:@selector(hide:) keyEquivalent:@"h"];
	[item setTarget:NSApp];
	[app addItem:[NSMenuItem separatorItem]];
	item = [app addItemWithTitle:[@"Quit " stringByAppendingString:name] action:@selector(quit:) keyEquivalent:@"q"];
	[item setTarget:observer];

	[NSApp setMainMenu:bar];

	if([NSApp respondsToSelector:@selector(setAppleMenu:)]) {
		[NSApp performSelector:@selector(setAppleMenu:) withObject:app];
	}

	[app release];
	[bar release];

	[NSApp finishLaunching];
	[NSApp activateIgnoringOtherApps:YES];

	[[NSDistributedNotificationCenter defaultCenter] addObserver:observer selector:@selector(themeChanged:) name:@"AppleInterfaceThemeChangedNotification" object:nil];

	[pool release];

	return 0;
}

#ifdef MW_VULKAN
VkMetalSurfaceCreateInfoEXT MwCocoaGetMetalSurfaceCreateInfo(MwWidget handle) {
	NSView* view  = handle->lowlevel->cocoa.view;
	Class	cls   = NSClassFromString(@"CAMetalLayer");
	id	layer = nil;

	VkMetalSurfaceCreateInfoEXT createInfo = {
	    .sType  = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,
	    .pNext  = NULL,
	    .flags  = 0,
	    .pLayer = NULL,
	};

	if(cls != nil) {
		layer = [cls performSelector:@selector(layer)];
	}

	if(layer != nil) {
		handle->lowlevel->common.copy_buffer = 0;
		[view setLayer:layer];
		[view setWantsLayer:YES];
		createInfo.pLayer = (void*)layer;
	}

	return createInfo;
};
#endif

#include "../call.c"
CALL(Cocoa);
