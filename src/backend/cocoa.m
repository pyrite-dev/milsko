#include <Mw/Milsko.h>

#ifdef __clang__
#pragma clang push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static NSRect rectFlip(NSRect originFrame) {
  NSScreen *zeroScreen = [[NSScreen screens] objectAtIndex:0];
  double screenHeight = [zeroScreen frame].size.height;
  double originY = originFrame.origin.y;
  double frameHeight = originFrame.size.height;
  double destinationY = screenHeight - (originY + frameHeight);
  NSRect destinationFrame = originFrame;
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

static NSPoint pointFlip(NSPoint point) {
  return rectFlip(NSMakeRect(point.x, point.y, 0, 0)).origin;
}

@implementation MilskoCocoaPixmap

+ (MilskoCocoaPixmap *)newWithWidth:(int)width height:(int)height {
  MilskoCocoaPixmap *p = [MilskoCocoaPixmap alloc];

  p->width = width;
  p->height = height;
  p->image = NULL;

  p->buf = malloc(width * height * 4);

  p->rep =
      [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&p->buf
                                              pixelsWide:(int)width
                                              pixelsHigh:(int)height
                                           bitsPerSample:8
                                         samplesPerPixel:4
                                                hasAlpha:YES
                                                isPlanar:NO
                                          colorSpaceName:NSDeviceRGBColorSpace
                                             bytesPerRow:(int)width * 4
                                            bitsPerPixel:32];
  assert(p->rep);

  p->image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
  assert(p->image);
  [p->image addRepresentation:p->rep];

  return p;
}
- (void)updateWithData:(unsigned char *)_data {
  memcpy(self->buf, _data, width * height * 4);
}
- (void)destroy {
  free(self->buf);
  [self->image removeRepresentation:self->rep];
  [self->image dealloc];
  [self->rep dealloc];
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
  [c retain];

  if (x == MwDEFAULT) {
    x = ([[NSScreen mainScreen] frame].size.width / 2.) - (width / 2.);
  }
  if (y == MwDEFAULT) {
    y = ([[NSScreen mainScreen] frame].size.height / 2.) - (height / 2.);
  }
  c->application = [NSApplication sharedApplication];
  c->rect = rectFlip(NSMakeRect(x, y, width, height));

  if (parent == NULL) {
    c->window = [[MilskoCocoaWindow alloc]
        initWithContentRect:c->rect
                  styleMask:(NSTitledWindowMask |
                             NSTexturedBackgroundWindowMask |
                             NSClosableWindowMask | NSMiniaturizableWindowMask |
                             NSResizableWindowMask)
                    backing:NSBackingStoreBuffered
                      defer:NO];
  } else {
    double offset = 0;
    MilskoCocoaWindow *parentWindow = parent->cocoa.real->window;
    offset =
        [parentWindow frameRectForContentRect:[parentWindow frame]]
            .size.height -
        [parentWindow contentRectForFrameRect:[parentWindow frame]].size.height;

    c->rect.origin.x += [parentWindow frame].origin.x;
    c->rect.origin.y -= [parentWindow frame].origin.y - offset;
    c->rect.origin.y -= offset;

    c->window =
        [[MilskoCocoaWindow alloc] initWithContentRect:c->rect
                                             styleMask:NSBorderlessWindowMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];
  }
  [c->window
      setDelegate:[[MilskoCocoaWindowDelegate alloc] initWithWin:c->window]];

  [c->window makeKeyAndOrderFront:c->application];
  [c->window retain];

  if (parent != NULL) {
    MilskoCocoa *p = parent->cocoa.real;
    double offset = 0;
    offset = [p->window frameRectForContentRect:[p->window frame]].size.height -
             [p->window contentRectForFrameRect:[p->window frame]].size.height;
    [p->window addChildWindow:c->window ordered:NSWindowAbove];
    [c->window setHasShadow:MwFALSE];
    [c->window setParentWindow:p->window];

    c->rect.origin.x += [p->window frame].origin.x;
    c->rect.origin.y -= [p->window frame].origin.y - offset;
    c->rect.origin.y -= offset;
  } else {
    if ([c->application respondsToSelector:@selector(setActivationPolicy:)]) {
      [c->application
          setActivationPolicy:0 /* NSApplicationActivationPolicyRegular */];
    }
    [c->application activateIgnoringOtherApps:true];
    [c->window makeFirstResponder:c->view];
    [c->window makeKeyAndOrderFront:nil];
  }
  [c->application setDelegate:[[MilskoCocoaApplicationDelegate alloc]
                                  initWithAppl:c->application]];
  [c->application retain];

  c->view = [[MilskoCocoaView alloc] initWithFrame:c->rect];
  [c->view retain];
  c->handle = [[MilskoFakePointer alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)];
  [c->handle retain];
  [c->handle setPointer:r];
  [c->view addSubview:c->handle];

  [c->window setContentView:c->view];

  c->parent = parent;

  c->_forceRender = MwTRUE;
  c->strHash = 0;

  c->cursorPixmap = NULL;

  [c->application finishLaunching];

  c->pointerLocked = MwFALSE;

  return c;
}
- (void)polygonWithPoints:(MwPoint *)points
             points_count:(int)points_count
                    color:(MwLLColor)color {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSGraphicsContext *ctx = [self->view context];
  if (ctx) {
    int i;
    NSBezierPath *path = [NSBezierPath bezierPath];
    NSColor *nscolor =
        [NSColor colorWithCalibratedRed:color->common.red / 255.
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
  }
  [pool release];
};
- (void)lineWithPoints:(MwPoint *)points color:(MwLLColor)color {
  (void)points;
  (void)color;
};
- (void)getX:(int *)x Y:(int *)y W:(unsigned int *)w H:(unsigned int *)h {
  NSRect frame = rectFlip([self->window frame]);
  frame = rectFlip(frame);

  if (parent) {
    frame.origin.x -= [parent->cocoa.real->window frame].origin.x;
    frame.origin.y += [parent->cocoa.real->window frame].origin.y;
  }

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
    MilskoCocoaWindow *parentWindow = parent->cocoa.real->window;
    offset =
        [parentWindow frameRectForContentRect:[parentWindow frame]]
            .size.height -
        [parentWindow contentRectForFrameRect:[parentWindow frame]].size.height;

    frame.origin.x += [parentWindow frame].origin.x;
    frame.origin.y -= [parentWindow frame].origin.y - offset;
    frame.origin.y -= offset;
  }

  [self->view setFrameSize:frame.size];

  [self->window setFrameOrigin:frame.origin];

  [self forceRender];
};
- (void)setW:(int)w H:(int)h {
  NSRect frame = [self->window frame];
  frame.size.width = w;
  frame.size.height = h;

  self->rect = frame;

  [self->view setFrameSize:frame.size];
  [self->window setFrame:frame display:YES animate:false];
  [self forceRender];
};
- (int)pending {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  MwBool isPending = MwFALSE;
  if (_forceRender) {
    _forceRender = MwFALSE;
    [pool release];
    return 1;
  }
  self->lastEvent = [self->window nextEventMatchingMask:NSAnyEventMask
                                              untilDate:[NSDate distantPast]
                                                 inMode:NSDefaultRunLoopMode
                                                dequeue:YES];

  isPending = self->lastEvent != NULL;
  [pool release];
  return isPending;
};

- (void)getNextEvent {
  [self eventProcess:self->lastEvent];
  while (true) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSEvent *ev = [self->window nextEventMatchingMask:NSAnyEventMask
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
    if (!ev) {
      [pool release];
      break;
    }
    /* run through the switch case on ev.type, before calling sendEvent on any
     * events we handle */
    [self eventProcess:ev];
    [pool release];
    [self->application updateWindows];
  }

  [self sendClipboardEvent];

  if (self->pointerLocked && [self->window isMainWindow]) {
    struct CGPoint pos;
    pos.x = [window frame].origin.x + [window frame].size.width / 2;
    pos.y = [window frame].origin.y + [window frame].size.height / 2;

    CGWarpMouseCursorPosition(pos);
  }

  [self->application updateWindows];
};

- (void)eventProcess:(NSEvent *)ev {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSWindow *win = [ev window];
  MwLL h;
  MwBool doSendEvent = MwTRUE;

  if (!win) {
    [pool release];
    return;
  }

  if ([[[win contentView] subviews] count] == 0) {
    printf("no subviews on %p\n", win);
    [pool release];
    return;
  } else {
    h = [((MilskoFakePointer *)[[[win contentView] subviews]
        objectAtIndex:0]) pointer];
  }

  switch ([ev type]) {
  case NSLeftMouseDown:
  case NSRightMouseDown:
  case NSOtherMouseDown:
  case NSLeftMouseUp:
  case NSRightMouseUp:
  case NSOtherMouseUp: {
    [self handleMouseEvent:ev ll:h];
  }
  case NSLeftMouseDragged:
  case NSRightMouseDragged:
  case NSOtherMouseDragged:
  case NSMouseMoved: {
    MwPoint pos;
    NSPoint pos_translated = pointFlip([ev locationInWindow]);
    pos.x = pos_translated.x;
    pos.y = pos_translated.y;
    MwLLDispatch(h, move, &pos);
    break;
  }
  case NSMouseEntered:
    MwLLDispatch(h, focus_in, NULL);
    [self->cursor push];
    break;
  case NSMouseExited:
    MwLLDispatch(h, focus_out, NULL);
    [self->cursor pop];
    break;
  case NSKeyDown:
  case NSKeyUp: {
    [self handleKeyEvent:ev ll:h];
  }
  case NSCursorUpdate:
    [self->cursor set];
    break;
  case NSScrollWheel:
    break;
  default:
    break;
  };
  [win sendEvent:ev];

  [pool release];
}

- (void)handleMouseEvent:(NSEvent *)ev ll:(MwLL)ll {
  MwLLMouse mouse;
  MwBool isDown = MwTRUE;
  NSPoint mousePoint = pointFlip([ev locationInWindow]);
  MwLL this = [self->handle pointer];
  switch ([ev type]) {
  case NSLeftMouseUp:
    isDown = MwFALSE;
  case NSLeftMouseDown:
    mouse.button = MwLLMouseLeft;
    break;
  case NSRightMouseUp:
    isDown = MwFALSE;
  case NSRightMouseDown:
    mouse.button = MwLLMouseRight;
    break;
  case NSOtherMouseUp:
    isDown = MwFALSE;
  case NSOtherMouseDown:
    mouse.button = MwLLMouseMiddle;
    break;
  default:
    break;
  }
  mouse.point.x = mousePoint.x;
  mouse.point.y = mousePoint.y;

  if (isDown) {
    MwLLDispatch(ll, down, &mouse);
  } else {
    MwLLDispatch(ll, up, &mouse);
  }
}

- (void)handleKeyEvent:(NSEvent *)ev ll:(MwLL)ll {
  int ch;
  MwLL this = [self->handle pointer];
  enum {
    kVK_ANSI_A = 0x00,
    kVK_ANSI_S = 0x01,
    kVK_ANSI_D = 0x02,
    kVK_ANSI_F = 0x03,
    kVK_ANSI_H = 0x04,
    kVK_ANSI_G = 0x05,
    kVK_ANSI_Z = 0x06,
    kVK_ANSI_X = 0x07,
    kVK_ANSI_C = 0x08,
    kVK_ANSI_V = 0x09,
    kVK_ANSI_B = 0x0B,
    kVK_ANSI_Q = 0x0C,
    kVK_ANSI_W = 0x0D,
    kVK_ANSI_E = 0x0E,
    kVK_ANSI_R = 0x0F,
    kVK_ANSI_Y = 0x10,
    kVK_ANSI_T = 0x11,
    kVK_ANSI_1 = 0x12,
    kVK_ANSI_2 = 0x13,
    kVK_ANSI_3 = 0x14,
    kVK_ANSI_4 = 0x15,
    kVK_ANSI_6 = 0x16,
    kVK_ANSI_5 = 0x17,
    kVK_ANSI_Equal = 0x18,
    kVK_ANSI_9 = 0x19,
    kVK_ANSI_7 = 0x1A,
    kVK_ANSI_Minus = 0x1B,
    kVK_ANSI_8 = 0x1C,
    kVK_ANSI_0 = 0x1D,
    kVK_ANSI_RightBracket = 0x1E,
    kVK_ANSI_O = 0x1F,
    kVK_ANSI_U = 0x20,
    kVK_ANSI_LeftBracket = 0x21,
    kVK_ANSI_I = 0x22,
    kVK_ANSI_P = 0x23,
    kVK_ANSI_L = 0x25,
    kVK_ANSI_J = 0x26,
    kVK_ANSI_Quote = 0x27,
    kVK_ANSI_K = 0x28,
    kVK_ANSI_Semicolon = 0x29,
    kVK_ANSI_Backslash = 0x2A,
    kVK_ANSI_Comma = 0x2B,
    kVK_ANSI_Slash = 0x2C,
    kVK_ANSI_N = 0x2D,
    kVK_ANSI_M = 0x2E,
    kVK_ANSI_Period = 0x2F,
    kVK_ANSI_Grave = 0x32,
    kVK_Return = 0x24,
    kVK_Space = 0x31,
    kVK_Escape = 0x35,
    kVK_Shift = 0x38,
    kVK_Control = 0x3B,
    kVK_RightShift = 0x3C,
    kVK_RightControl = 0x3E,
    kVK_LeftArrow = 0x7B,
    kVK_RightArrow = 0x7C,
    kVK_DownArrow = 0x7D,
    kVK_UpArrow = 0x7E
  };
#define KEY_CASE(x, y)                                                         \
  case x:                                                                      \
    ch = y;                                                                    \
    break;
  switch ([ev keyCode]) {
    KEY_CASE(kVK_ANSI_A, 'a')
    KEY_CASE(kVK_ANSI_B, 'b')
    KEY_CASE(kVK_ANSI_C, 'c')
    KEY_CASE(kVK_ANSI_D, 'd')
    KEY_CASE(kVK_ANSI_E, 'e')
    KEY_CASE(kVK_ANSI_F, 'f')
    KEY_CASE(kVK_ANSI_G, 'g')
    KEY_CASE(kVK_ANSI_H, 'h')
    KEY_CASE(kVK_ANSI_I, 'i')
    KEY_CASE(kVK_ANSI_J, 'j')
    KEY_CASE(kVK_ANSI_K, 'k')
    KEY_CASE(kVK_ANSI_L, 'l')
    KEY_CASE(kVK_ANSI_M, 'm')
    KEY_CASE(kVK_ANSI_N, 'n')
    KEY_CASE(kVK_ANSI_O, 'o')
    KEY_CASE(kVK_ANSI_P, 'p')
    KEY_CASE(kVK_ANSI_Q, 'q')
    KEY_CASE(kVK_ANSI_R, 'r')
    KEY_CASE(kVK_ANSI_S, 's')
    KEY_CASE(kVK_ANSI_T, 't')
    KEY_CASE(kVK_ANSI_U, 'u')
    KEY_CASE(kVK_ANSI_V, 'v')
    KEY_CASE(kVK_ANSI_W, 'w')
    KEY_CASE(kVK_ANSI_X, 'x')
    KEY_CASE(kVK_ANSI_Y, 'y')
    KEY_CASE(kVK_ANSI_Z, 'z')
    KEY_CASE(kVK_ANSI_0, '0')
    KEY_CASE(kVK_ANSI_1, '1')
    KEY_CASE(kVK_ANSI_2, '2')
    KEY_CASE(kVK_ANSI_3, '3')
    KEY_CASE(kVK_ANSI_4, '4')
    KEY_CASE(kVK_ANSI_5, '5')
    KEY_CASE(kVK_ANSI_6, '6')
    KEY_CASE(kVK_ANSI_7, '7')
    KEY_CASE(kVK_ANSI_8, '8')
    KEY_CASE(kVK_ANSI_9, '9')
    KEY_CASE(kVK_ANSI_Quote, '\"')
    KEY_CASE(kVK_ANSI_Grave, '`')
    KEY_CASE(kVK_ANSI_Backslash, '/')
    KEY_CASE(kVK_ANSI_Comma, ',')
    KEY_CASE(kVK_ANSI_Equal, '=')
    KEY_CASE(kVK_Escape, MwLLKeyEscape)
    KEY_CASE(kVK_ANSI_LeftBracket, '[')
    KEY_CASE(kVK_ANSI_Minus, '-')
    KEY_CASE(kVK_ANSI_Period, '.')
    KEY_CASE(kVK_Return, MwLLKeyEnter)
    KEY_CASE(kVK_ANSI_RightBracket, ']')
    KEY_CASE(kVK_ANSI_Semicolon, ';')
    KEY_CASE(kVK_ANSI_Slash, '\\')
    KEY_CASE(kVK_Space, ' ')
    KEY_CASE(kVK_Control, MwLLKeyControl)
    KEY_CASE(kVK_RightControl, MwLLKeyControl)
    KEY_CASE(kVK_Shift, MwLLKeyLeftShift)
    KEY_CASE(kVK_RightShift, MwLLKeyRightShift)
    KEY_CASE(kVK_DownArrow, MwLLKeyDown)
    KEY_CASE(kVK_LeftArrow, MwLLKeyLeft)
    KEY_CASE(kVK_RightArrow, MwLLKeyRight)
    KEY_CASE(kVK_UpArrow, MwLLKeyUp)
  }
  switch ([ev type]) {
  case NSKeyDown:
    MwLLDispatch(this, key, &ch);
    MwLLDispatch(ll, key, &ch);
    break;
  case NSKeyUp:
    MwLLDispatch(this, key_released, &ch);
    MwLLDispatch(ll, key_released, &ch);
    break;
  default:
    break;
  }
}

- (void)sendClipboardEvent {
  /*NSAutoreleasePool* pool	      = [[NSAutoreleasePool alloc] init];
  NSPasteboard*	   pasteboard = [NSPasteboard generalPasteboard];
  NSArray*	   items      = @[
                 @"public.utf8-plain-text",
                 @"public.utf16-external-plain-text",
                 @"com.apple.traditional-mac-plain-text",
  ];
  MwLL this = self->handle.pointer;

  if([pasteboard canReadItemWithDataConformingToTypes:items]) {
          char*  data = NULL;
          size_t size = 0;
          for(NSPasteboardItem* item in [pasteboard pasteboardItems]) {
                  for(NSString* it in items) {
                          NSString* itemData = [item
  stringForType:(NSString*)it]; if(itemData != NULL) { if(strHash != 0 &&
  strHash != [itemData hash]) { char* text = malloc([itemData length]);
                                          strncpy(text, [itemData UTF8String],
  [itemData length]); MwLLDispatch(this, clipboard, text); printf("%s -> %p\n",
  text, this); free(text);
                                  }
                                  strHash = [itemData hash];
                          }
                  }
          }
  }

  [pool release];*/
}

- (void)setTitle:(const char *)title {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  [self->window
      setTitleWithRepresentedFilename:[NSString stringWithUTF8String:title]];
  [pool release];
};
- (void)drawPixmap:(MwLLPixmap)pixmap rect:(MwRect *)_rect {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  NSGraphicsContext *ctx = [self->view context];
  if (ctx) {
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];

    [[p image]
        drawInRect:NSMakeRect(_rect->x, _rect->y, _rect->width, _rect->height)
          fromRect:NSZeroRect
         operation:NSCompositeSourceOver
          fraction:1.0];

    [NSGraphicsContext restoreGraphicsState];

    [self->view setNeedsDisplay:YES];
  }
  [pool release];
};
- (void)setIcon:(MwLLPixmap)pixmap {
  [self->application setApplicationIconImage:[pixmap->cocoa.real image]];
};
- (void)forceRender {
  self->_forceRender = MwTRUE;
  // NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  // NSEvent *event = [NSEvent otherEventWithType:NSApplicationDefined
  //                                     location:NSMakePoint(0, 0)
  //                                modifierFlags:0
  //                                    timestamp:0
  //                                 windowNumber:0
  //                                      context:nil
  //                                      subtype:0
  //                                        data1:0
  //                                        data2:0];
  // [NSApp postEvent:event atStart:YES];
  // [pool release];
};
- (void)setCursor:(MwCursor *)image mask:(MwCursor *)mask {
  int y, x, ys, xs;
  unsigned char *di = malloc(image->width * image->height * 4);
  memset(di, 0, image->width * image->height * 4);

  if (self->cursorPixmap) {
    [self->cursorPixmap destroy];
  }
  self->cursorPixmap =
      [MilskoCocoaPixmap newWithWidth:image->width height:image->height];

  xs = -mask->x + image->x;
  ys = MwCursorDataHeight + mask->y;
  ys = MwCursorDataHeight + image->y - ys;

  for (y = 0; y < mask->height; y++) {
    unsigned int d = mask->data[y];
    for (x = mask->width - 1; x >= 0; x--) {
      int px = 0;
      int idx = ((y * mask->width) + x) * 4;

      if (d & 1) {
        di[idx + 3] = 255;
      };
      d = d >> 1;
    }
  }
  for (y = 0; y < image->height; y++) {
    unsigned int d = image->data[y];
    for (x = image->width - 1; x >= 0; x--) {
      int px = 0;
      int idx = ((y * image->width) + x) * 4;

      if (d & 1) {
        px = 255;
      };

      di[idx] = px;
      di[idx + 1] = px;
      di[idx + 2] = px;
      d = d >> 1;
    }
  }

  [self->cursorPixmap updateWithData:di];

  self->cursor = [[NSCursor alloc]
      initWithImage:[self->cursorPixmap image]
            hotSpot:NSMakePoint(image->x, image->y + image->height)];
  [self->cursor retain];

  [self->cursor pop];
  [self->cursor push];

  free(di);
};
- (void)detachWithPoint:(MwPoint *)point {
  [self->window setParentWindow:NULL];
};
- (void)show:(int)show {
  (void)show;
};
- (void)makePopupWithParent:(MwLL)_parent {
  (void)_parent;
};
- (void)setSizeHintsWithMinX:(int)minx
                        MinY:(int)miny
                        MaxX:(int)maxx
                        MaxY:(int)maxy {
  [self->window setMinSize:NSMakeSize(minx, miny)];
  [self->window setMaxSize:NSMakeSize(maxx, maxy)];
};
- (void)makeBorderless:(int)toggle {
  MwU32 mask = [self->window styleMask];
  if (toggle) {
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
  self->pointerLocked = toggle;
};
- (void)setClipboard:(const char *)text {
  (void)text;
  // TODO: find out how to do this while supporting 10.4
  // NSAutoreleasePool* pool	      = [[NSAutoreleasePool alloc] init];
  // NSPasteboard*	   pasteboard = [NSPasteboard generalPasteboard];
  // [pasteboard declareTypes:[NSArray arrayWithObjects:NSPasteboardTypeString]
  // owner:nil]; [pasteboard setString:[NSString stringWithUTF8String:text]
  // forType:NSPasteboardTypeString]; [pool release];
};
- (void)makeToolWindow {
  /* If my understand of what a "tool window" usually is is correct then I
   * highly doubt the Mac OS has this and if they did they probably outright
   * removed it along time ago is this kind of conflicts with modern UX. So
   * we'll just make it borderless idgaf */
  [self makeBorderless:MwTRUE];
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
  if (self->lastEvent) {
    [self->lastEvent release];
    [self->lastEvent dealloc];
  }
  [self->handle release];
  [self->handle dealloc];
  [self->window release];
  [self->window dealloc];
}

- (NSWindow *)parentWindow {
  NSWindow *topmostWindow = self->window;
  while ([topmostWindow parentWindow])
    topmostWindow = [topmostWindow parentWindow];
  return topmostWindow;
}

- (NSView *)getView {
  return view;
}
- (NSWindow *)getWindow {
  return window;
}

- (MilskoFakePointer *)getHandle {
  return handle;
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
    [self->rep retain];

    self->context =
        [NSGraphicsContext graphicsContextWithBitmapImageRep:self->rep];
    assert(self->context);
    [self->context retain];
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
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSSize sz = [self->rep size];
  [super drawRect:dirtyRect];
  if (!self->rep) {
    [pool release];
    return;
  }

  [self->rep drawInRect:NSMakeRect(0, 0, sz.width, sz.height)];

  [self->rep bitmapData];
  [pool release];
}

- (void)destroy {
  CGColorSpaceRelease(self->space);
  [self->rep release];
  [self->context release];
}

- (void)setFrameSize:(NSSize)newSize {
  [super setFrameSize:newSize];
  [self->rep setSize:newSize];

  self->width = newSize.width;
  self->height = newSize.height;
}

- (void)displayRect:(NSRect)rect {
  (void)rect;
};

- (BOOL)acceptsFirstResponder {
  return MwTRUE;
}
- (BOOL)performKeyEquivalent:(NSEvent *)event {
  (void)event;
  return MwTRUE;
}

@end

@implementation MilskoCocoaApplicationDelegate
- (MilskoCocoaApplicationDelegate *)initWithAppl:(NSApplication *)_appl {
  self->appl = _appl;
  return self;
}
- (void)applicationDidBecomeActive:(NSNotification *)notification {
  printf("test\n");
}
- (void)applicationWillFinishLaunching:(NSNotification *)notification {
  // printf("test\n");
  // [self->appl activateIgnoringOtherApps:MwTRUE];
}
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  // [self->appl activateIgnoringOtherApps:MwTRUE];
}
@end

@implementation MilskoCocoaWindowDelegate

- (NSSize)windowWillResize:(NSWindow *)win toSize:(NSSize)frameSize;
{
  if ([[[win contentView] subviews] count] >= 1) {
    MilskoFakePointer *ptr = [[[win contentView] subviews] objectAtIndex:0];
    MwLL h = [ptr pointer];

    // MwLLDispatch(h, resize, NULL);
    MwLLDispatch(h, draw, NULL);
  }
  return frameSize;
}

- (void)windowDidBecomeMain:(NSNotification *)notification {
}

- (void)windowDidResize:(NSNotification *)notification {
  (void)notification;
}

// This will close/terminate the application when the main window is closed.
- (void)windowWillClose:(NSNotification *)notification {
  (void)notification;
  // MilskoCocoa *window = notification.object;
  // MwLL handle = [window getHandle].pointer;
  // MwLLDispatch(handle, close, NULL);
  [NSApp terminate:nil];
}

- (MilskoCocoaWindowDelegate *)initWithWin:(NSWindow *)win {
  self->w = win;
  return self;
}

@end

@implementation MilskoFakePointer
- (void)setPointer:(void *)pointer {
  [self setFrame:*(NSRect *)&pointer];
  self->ptr = pointer;
};
- (void *)pointer {
  return self->ptr;
};

- (void)drawRect:(NSRect)dirtyRect {
  /* explicitly do nothing */
  (void)dirtyRect;
}

- (void)destroy {
}
@end

@implementation MilskoCocoaWindow
- (BOOL)canBecomeKeyWindow {
  return true;
}
- (BOOL)canBecomeMainWindow {
  return true;
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
  free(r->common.raw);
  return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap pixmap) {
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  [p updateWithData:pixmap->common.raw];
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
  MilskoCocoaPixmap *p = pixmap->cocoa.real;
  [p destroy];
  [p dealloc];
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
