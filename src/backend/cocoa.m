#include <Mw/Milsko.h>

#pragma clang push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

static CGRect rectFlip(CGRect originFrame) {
  NSScreen *zeroScreen = NSScreen.screens[0];
  double screenHeight = zeroScreen.frame.size.height;
  double originY = originFrame.origin.y;
  double frameHeight = originFrame.size.height;
  double destinationY = screenHeight - (originY + frameHeight);
  CGRect destinationFrame = originFrame;
  destinationFrame.origin.y = destinationY;
  if (destinationFrame.origin.x < 0)
    destinationFrame.origin.x = 0;
  if (destinationFrame.origin.y < 0)
    destinationFrame.origin.y = 0;
  if (destinationFrame.size.width < 0)
    destinationFrame.size.width = 0;
  if (destinationFrame.size.height < 0)
    destinationFrame.size.height = 0;
  return destinationFrame;
}

static CGPoint pointFlip(CGPoint point) {
  return rectFlip(NSMakeRect(point.x, point.y, 0, 0)).origin;
}

@implementation MilskoCocoaPixmap

+ (MilskoCocoaPixmap *)newWithWidth:(int)width height:(int)height {
  MilskoCocoaPixmap *p = [MilskoCocoaPixmap alloc];

  p->width = width;
  p->height = height;
  p->data = NULL;
  p->image = NULL;
  return p;
}
- (void)updateWithData:(unsigned char *)_data {
  [self destroy];

  self->rep =
      [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&_data
                                              pixelsWide:(int)width
                                              pixelsHigh:(int)height
                                           bitsPerSample:8
                                         samplesPerPixel:4
                                                hasAlpha:YES
                                                isPlanar:NO
                                          colorSpaceName:NSDeviceRGBColorSpace
                                             bytesPerRow:(int)width * 4
                                            bitsPerPixel:32];

  assert(self->rep);
  self->data = [NSData dataWithBytes:[self->rep bitmapData]
                              length:self->width * self->height * 4];
  assert(self->data);
  self->image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
  assert(self->image);
  [self->image addRepresentation:self->rep];
}
- (void)destroy {
  if (self->data != NULL) {
    [self->data dealloc];
  }
  if (self->image != NULL) {
    [self->image dealloc];
  }
}
- (NSImage *)image {
  return self->image;
}
@end

@implementation MilskoCocoa

+ (MilskoCocoa *)newWithParent:(MwLL)parent
                             x:(int)x
                             y:(int)y
                         width:(int)width
                        height:(int)height
                        handle:(MwLL)r {
  MilskoCocoa *c = [MilskoCocoa alloc];

  if (x == MwDEFAULT) {
    x = ([NSScreen mainScreen].frame.size.width / 2.) - (width / 2.);
  }
  if (y == MwDEFAULT) {
    y = ([NSScreen mainScreen].frame.size.height / 2.) - (height / 2.);
  }
  c->application = [NSApplication sharedApplication];
  c->rect = rectFlip(NSMakeRect(x, y, width, height));

  if (parent == NULL) {
    c->window = [[NSWindow alloc]
        initWithContentRect:c->rect
                  styleMask:(NSTitledWindowMask | NSClosableWindowMask |
                             NSMiniaturizableWindowMask | NSResizableWindowMask)
                    backing:NSBackingStoreBuffered
                      defer:NO];
  } else {
    double offset = 0;
    NSWindow *parentWindow = parent->cocoa.real->window;
    offset =
        [parentWindow frameRectForContentRect:parentWindow.frame].size.height -
        [parentWindow contentRectForFrameRect:parentWindow.frame].size.height;

    c->rect.origin.x += parentWindow.frame.origin.x;
    c->rect.origin.y -= parentWindow.frame.origin.y - offset;
    c->rect.origin.y -= offset;

    c->window = [[NSWindow alloc] initWithContentRect:c->rect
                                            styleMask:NSBorderlessWindowMask
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
  }
  c->window.delegate = (id<NSWindowDelegate>)[[MilskoCocoaWindowDelegate alloc]
      initWithWin:c->window];

  [c->window makeKeyAndOrderFront:c->application];

  if (parent != NULL) {
    MilskoCocoa *p = parent->cocoa.real;
    [p->window addChildWindow:c->window ordered:NSWindowAbove];
    [c->window setHasShadow:MwFALSE];
  } else {
    // [c->application activateIgnoringOtherApps:true];
  }

  c->view = [[MilskoCocoaView alloc] initWithFrame:c->rect];
  c->handle = [[MilskoFakePointer alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)];
  [c->handle setPointer:r];
  [c->view addSubview:c->handle];

  [c->window setContentView:c->view];

  c->parent = parent;

  c->_forceRender = MwTRUE;

  return c;
}
- (void)polygonWithPoints:(MwPoint *)points
             points_count:(int)points_count
                    color:(MwLLColor)color {
  NSGraphicsContext *ctx = [self->view context];
  if (ctx) {
    int i;
    NSBezierPath *path = [NSBezierPath bezierPath];
    NSColor *nscolor = [NSColor colorWithRed:color->common.red / 255.
                                       green:color->common.green / 255.
                                        blue:color->common.blue / 255.
                                       alpha:1.0];

    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];

    [nscolor setFill];
    for (i = 0; i < points_count; i++) {
      if (i == 0) {
        [path moveToPoint:NSMakePoint(points[i].x,
                                      [self->window frame].size.height -
                                          points[i].y)];
      } else {
        [path lineToPoint:NSMakePoint(points[i].x,
                                      [self->window frame].size.height -
                                          points[i].y)];
      }
    }

    [path closePath];
    [path fill];

    [NSGraphicsContext restoreGraphicsState];

    [self->view setNeedsDisplay:YES];

    [nscolor dealloc];
  }
};
- (void)lineWithPoints:(MwPoint *)points color:(MwLLColor)color {
};
- (void)getX:(int *)x Y:(int *)y W:(unsigned int *)w H:(unsigned int *)h {
  NSRect frame = rectFlip([self->window frame]);

  *x = frame.origin.x;
  *y = frame.origin.y;

  *w = frame.size.width;
  *h = frame.size.height;
};
- (void)setX:(int)x Y:(int)y {
  NSRect frame = [self->window frame];

  frame.origin.x = x;
  frame.origin.y = y;

  frame = rectFlip(frame);

  if (parent) {
    double offset = 0;
    NSWindow *parentWindow = parent->cocoa.real->window;
    offset =
        [parentWindow frameRectForContentRect:parentWindow.frame].size.height -
        [parentWindow contentRectForFrameRect:parentWindow.frame].size.height;

    if (x < parentWindow.frame.origin.x) {
      frame.origin.x += parentWindow.frame.origin.x;
    }

    if (y < parentWindow.frame.origin.y) {
      frame.origin.y -= parentWindow.frame.origin.y - offset;
      frame.origin.y -= offset;
    }
  }
  [self->view setFrameSize:frame.size];

  [self->window setFrame:frame display:YES animate:false];
};
- (void)setW:(int)w H:(int)h {
  NSRect frame = [self->window frame];
  frame.size.width = w;
  frame.size.height = h;

  self->rect = frame;

  [self->view setFrameSize:frame.size];
  [self->window setFrame:frame display:YES animate:false];
};
- (int)pending {
  self->lastEvent =
      [self->application nextEventMatchingMask:NSAnyEventMask
                                     untilDate:nil
                                        inMode:NSDefaultRunLoopMode
                                       dequeue:YES];
  if (_forceRender) {
    _forceRender = MwFALSE;
    return 1;
  }
  return self->lastEvent != NULL;
};
- (void)getNextEvent {
  if (self->lastEvent != nil) {
    NSWindow *win = [self->lastEvent window];
    NSWindow *parentWindow = [self parentWindow];
    MwLL h;

    if ([win contentView].subviews.count == 0) {
      NSLog(@"[WARNING] no subviews on this window, cannot process events.\n");
      return;
    }

    h = [((MilskoFakePointer *)[win contentView].subviews[0]) pointer];
    switch (self->lastEvent.type) {
    case NSEventTypeLeftMouseDown:
    case NSEventTypeRightMouseDown:
    case NSEventTypeOtherMouseDown:
    case NSEventTypeLeftMouseUp:
    case NSEventTypeRightMouseUp:
    case NSEventTypeOtherMouseUp: {
      MwLLMouse mouse = {};
      MwBool isDown = MwTRUE;
      CGPoint mousePoint = pointFlip([lastEvent locationInWindow]);
      switch (self->lastEvent.type) {
      case NSEventTypeLeftMouseUp:
        isDown = MwFALSE;
      case NSEventTypeLeftMouseDown:
        mouse.button = MwLLMouseLeft;
        break;
      case NSEventTypeRightMouseUp:
        isDown = MwFALSE;
      case NSEventTypeRightMouseDown:
        mouse.button = MwLLMouseRight;
        break;
      case NSEventTypeOtherMouseUp:
        isDown = MwFALSE;
      case NSEventTypeOtherMouseDown:
        mouse.button = MwLLMouseMiddle;
        break;
      default:
        break;
      }
      mouse.point.x = mousePoint.x;
      mouse.point.y = mousePoint.y;

      if (isDown) {
        MwLLDispatch(h, down, &mouse);
      } else {
        MwLLDispatch(h, up, &mouse);
      }
      break;
    }

    break;
    case NSEventTypeMouseMoved: {
      MwPoint pos;
      pos.x = [lastEvent locationInWindow].x;
      pos.y = [win contentRectForFrameRect:win.frame].size.height -
              [lastEvent locationInWindow].y;
      MwLLDispatch(h, move, &pos);
      break;
    }
    case NSEventTypeLeftMouseDragged:
      MwLLDispatch(h, focus_in, NULL);
      break;
    case NSEventTypeRightMouseDragged:
      MwLLDispatch(h, focus_out, NULL);
      break;
    case NSEventTypeMouseEntered:
      break;
    case NSEventTypeMouseExited:
      break;
    case NSEventTypeKeyDown:
      [win interpretKeyEvents:[NSArray arrayWithObject:lastEvent]];
      break;
    case NSEventTypeKeyUp:
      [win interpretKeyEvents:[NSArray arrayWithObject:lastEvent]];
      break;
    case NSEventTypeFlagsChanged:
      break;
    case NSEventTypeAppKitDefined:
      break;
    case NSEventTypeSystemDefined:
      break;
    case NSEventTypeApplicationDefined:
      break;
    case NSEventTypePeriodic:
      break;
    case NSEventTypeCursorUpdate:
      break;
    case NSEventTypeScrollWheel:
      break;
    case NSEventTypeTabletPoint:
      break;
    case NSEventTypeTabletProximity:
      break;
      break;
    case NSEventTypeOtherMouseDragged:
      break;
    case NSEventTypeGesture:
      break;
    case NSEventTypeMagnify:
      break;
    case NSEventTypeSwipe:
      break;
    case NSEventTypeRotate:
      break;
    case NSEventTypeBeginGesture:
      break;
    case NSEventTypeEndGesture:
      break;
    case NSEventTypeSmartMagnify:
      break;
    case NSEventTypeQuickLook:
      break;
    case NSEventTypePressure:
      break;
    case NSEventTypeDirectTouch:
      break;
    case NSEventTypeChangeMode:
      break;
    };
    // printf("got event: %ld\n", self->lastEvent.type);
    [self->application sendEvent:self->lastEvent];
  }

  [self->view setNeedsDisplay:true];
};

- (void)setTitle:(const char *)title {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  [self->window
      setTitleWithRepresentedFilename:[NSString stringWithUTF8String:title]];
  [pool release];
};
- (void)drawPixmap:(MwLLPixmap)pixmap rect:(MwRect *)_rect {
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  NSGraphicsContext *ctx = [self->view context];
  if (ctx) {
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];

    [[p image] drawInRect:NSMakeRect(_rect->x, _rect->y, _rect->width,
                                     _rect->height)
                 fromRect:NSZeroRect
                operation:NSCompositeSourceOver
                 fraction:1.0
           respectFlipped:NO
                    hints:nil];

    [NSGraphicsContext restoreGraphicsState];

    [self->view setNeedsDisplay:YES];
  }
};
- (void)setIcon:(MwLLPixmap)pixmap {
};
- (void)forceRender {
  _forceRender = MwTRUE;
};
- (void)setCursor:(MwCursor *)image mask:(MwCursor *)mask {
};
- (void)detachWithPoint:(MwPoint *)point {
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
  MwU32 mask = [self->window styleMask];
  if (mask & NSBorderlessWindowMask) {
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
  /* MacOS didn't have a "pointer grab" function
   * until 10.13.2 so I need to do this manually */
};
- (void)setClipboard:(const char *)text {
};
- (void)getClipboard {
};
- (void)makeToolWindow {
};
- (void)getCursorCoord:(MwPoint *)point {
  NSPoint p = [NSEvent mouseLocation];
  point->x = p.x;
  point->y = p.y;
};
- (void)getScreenSize:(MwRect *)_rect {
  NSScreen *screen = [self->window screen];
  _rect->x = [screen frame].origin.x;
  _rect->y = [screen frame].origin.y;
  _rect->width = [screen frame].size.width;
  _rect->height = [screen frame].size.height;
};
- (void)destroy {
  [self->window dealloc];
}

- (NSWindow *)parentWindow {
  NSWindow *topmostWindow = self->window;
  while (topmostWindow.parentWindow)
    topmostWindow = topmostWindow.parentWindow;
  return topmostWindow;
}

- (NSView *)getView {
  return view;
}
@end

@implementation MilskoCocoaView
- (id)initWithFrame:(NSRect)frame {
  width = frame.size.width;
  height = frame.size.height;
  self = [super initWithFrame:frame];
  self->space = CGColorSpaceCreateDeviceRGB();

  if (width == 0 || height == 0) {
    self->rep = NULL;
    self->context = NULL;
  } else {
    self->rep =
        [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL
                                                pixelsWide:width
                                                pixelsHigh:height
                                             bitsPerSample:8
                                           samplesPerPixel:4
                                                  hasAlpha:YES
                                                  isPlanar:NO
                                            colorSpaceName:NSDeviceRGBColorSpace
                                               bytesPerRow:width * 4
                                              bitsPerPixel:32];
    assert(self->rep);

    self->context =
        [NSGraphicsContext graphicsContextWithBitmapImageRep:self->rep];
    assert(self->context);
  }

  return self;
}

- (NSGraphicsContext *)context {
  return self->context;
}

- (NSBitmapImageRep *)getRep {
  return self->rep;
}

- (void)drawRect:(NSRect)dirtyRect {
  NSSize sz = self->rep.size;
  [super drawRect:dirtyRect];
  if (!self->rep) {
    return;
  }

  [self->rep drawInRect:NSMakeRect(0, 0, sz.width, sz.height)];

  [self->rep bitmapData];
}

- (void)destroy {
  CGColorSpaceRelease(self->space);
}

- (void)setFrameSize:(NSSize)newSize {
  [super setFrameSize:newSize];
  [self->rep setSize:newSize];
}

- (void)displayRect:(NSRect)rect {
};
@end

@implementation MilskoFakePointer
- (void)setPointer:(void *)pointer {
  self.frame = *(NSRect *)&pointer;
  self->ptr = pointer;
};
- (void *)pointer {
  return self->ptr;
};

- (void)drawRect:(NSRect)dirtyRect {
  /* explicitly do nothing */
}

- (void)destroy {
}

@end

@implementation MilskoCocoaWindowDelegate

- (NSSize)windowWillResize:(NSWindow *)win toSize:(NSSize)frameSize;
{
  if (win.contentView.subviews.count >= 1) {
    MilskoFakePointer *ptr = win.contentView.subviews[0];
    MwLL h = [ptr pointer];

    // MwLLDispatch(h, resize, NULL);
    MwLLDispatch(h, draw, NULL);
  }
  return frameSize;
}

- (void)windowDidResize:(NSNotification *)notification {
}

- (instancetype)initWithWin:(NSWindow *)win {
  self->w = win;
  return self;
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

  MilskoCocoa *o = [MilskoCocoa newWithParent:parent
                                            x:x
                                            y:y
                                        width:width
                                       height:height
                                       handle:r];
  r->cocoa.real = o;

  return r;
}

static void MwLLDestroyImpl(MwLL handle) {
  MilskoCocoa *h = handle->cocoa.real;

  [h destroy];

  MwLLDestroyCommon(handle);

  free(handle);
}

static void MwLLBeginDrawImpl(MwLL handle) { (void)handle; }

static void MwLLEndDrawImpl(MwLL handle) { (void)handle; }

static void MwLLPolygonImpl(MwLL handle, MwPoint *points, int points_count,
                            MwLLColor color) {
  MilskoCocoa *h = handle->cocoa.real;
  [h polygonWithPoints:points points_count:points_count color:color];
}

static void MwLLLineImpl(MwLL handle, MwPoint *points, MwLLColor color) {
  MilskoCocoa *h = handle->cocoa.real;
  [h lineWithPoints:points color:color];
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
  MwLLColor c = malloc(sizeof(*c));
  MwLLColorUpdate(handle, c, r, g, b);
  return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
  (void)handle;

  c->common.red = r;
  c->common.green = g;
  c->common.blue = b;
}

static void MwLLGetXYWHImpl(MwLL handle, int *x, int *y, unsigned int *w,
                            unsigned int *height) {
  MilskoCocoa *h = handle->cocoa.real;
  [h getX:x Y:y W:w H:height];
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setX:x Y:y];
}

static void MwLLSetWHImpl(MwLL handle, int w, int height) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setW:w H:height];
}

static void MwLLFreeColorImpl(MwLLColor color) { free(color); }

static int MwLLPendingImpl(MwLL handle) {
  MilskoCocoa *h = handle->cocoa.real;
  if ([h pending]) {
    MwLLDispatch(handle, draw, NULL);
    return 1;
  };
  return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
  MilskoCocoa *h = handle->cocoa.real;
  [h getNextEvent];
}

static void MwLLSetTitleImpl(MwLL handle, const char *title) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setTitle:title];
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char *data,
                                       int width, int height) {
  (void)handle;

  MwLLPixmap r = malloc(sizeof(*r));

  r->common.raw = malloc(4 * width * height);
  memcpy(r->common.raw, data, 4 * width * height);

  r->common.width = width;
  r->common.height = height;

  r->cocoa.real = [MilskoCocoaPixmap newWithWidth:width height:height];

  MwLLPixmapUpdate(r);
  return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  [p updateWithData:pixmap->common.raw];
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  [p destroy];
  free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect *rect, MwLLPixmap pixmap) {
  MilskoCocoa *h = handle->cocoa.real;
  [h drawPixmap:pixmap rect:rect];
  MwLLForceRender(handle);
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setIcon:pixmap];
}

static void MwLLForceRenderImpl(MwLL handle) {
  MilskoCocoa *h = handle->cocoa.real;
  [h forceRender];
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor *image, MwCursor *mask) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setCursor:image mask:mask];
}

static void MwLLDetachImpl(MwLL handle, MwPoint *point) {
  MilskoCocoa *h = handle->cocoa.real;
  [h detachWithPoint:point];
}

static void MwLLShowImpl(MwLL handle, int show) {
  MilskoCocoa *h = handle->cocoa.real;
  [h show:show];
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
  MilskoCocoa *h = handle->cocoa.real;
  [h makePopupWithParent:parent];
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx,
                                 int maxy) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setSizeHintsWithMinX:minx MinY:miny MaxX:maxx MaxY:maxy];
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
  MilskoCocoa *h = handle->cocoa.real;
  [h makeBorderless:toggle];
}

static void MwLLFocusImpl(MwLL handle) {
  MilskoCocoa *h = handle->cocoa.real;
  [h focus];
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
  MilskoCocoa *h = handle->cocoa.real;
  [h grabPointer:toggle];
}

static void MwLLSetClipboardImpl(MwLL handle, const char *text) {
  MilskoCocoa *h = handle->cocoa.real;
  [h setClipboard:text];
}

static void MwLLGetClipboardImpl(MwLL handle) { (void)handle; }

static void MwLLMakeToolWindowImpl(MwLL handle) {

  MilskoCocoa *h = handle->cocoa.real;
  [h makeToolWindow];
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint *point) {
  MilskoCocoa *h = handle->cocoa.real;
  [h getCursorCoord:point];
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect *rect) {
  MilskoCocoa *h = handle->cocoa.real;
  [h getScreenSize:rect];
}

static void MwLLBeginStateChangeImpl(MwLL handle) { MwLLShow(handle, 0); }

static void MwLLEndStateChangeImpl(MwLL handle) { MwLLShow(handle, 1); }

static int MwLLCocoaCallInitImpl(void) { return 0; }

#include "call.c"
CALL(Cocoa);
