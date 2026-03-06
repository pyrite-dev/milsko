#include "Mw/BaseTypes.h"
#include "Mw/LowLevel.h"
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
  p->image = NULL;
  return p;
}
- (void)updateWithData:(unsigned char *)_data {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
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
  [self->rep retain];
  self->image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
  assert(self->image);
  [self->image addRepresentation:self->rep];
  [self->image retain];
  [pool release];
}
- (void)destroy {
  if (self->image != NULL) {
    [self->image release];
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
  [c retain];

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
  [c->window retain];

  if (parent != NULL) {
    MilskoCocoa *p = parent->cocoa.real;
    [p->window addChildWindow:c->window ordered:NSWindowAbove];
    [c->window setHasShadow:MwFALSE];
  } else {
    [c->application setActivationPolicy:NSApplicationActivationPolicyRegular];
    [c->application activateIgnoringOtherApps:true];
    [c->window makeFirstResponder:c->view];
  }

  c->view = [[MilskoCocoaView alloc] initWithFrame:c->rect];
  c->handle = [[MilskoFakePointer alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)];
  [c->handle setPointer:r];
  [c->view addSubview:c->handle];

  [c->window setContentView:c->view];

  c->parent = parent;

  c->_forceRender = MwTRUE;
  c->strHash = 0;

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
  }
  [pool release];
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
  if (_forceRender) {
    _forceRender = MwFALSE;
    return 1;
  }
// Apple does not give you a reliable way to tell when events are coming. I
// found this out by accident by stumbling upon a comment wxWidget's code,
// thought I could figure out something better, and I ended up with the same
// solution they tried and can confirm it doesn't work.
// Thanks Tim Cook.
#if 0
  self->lastEvent = [self->window nextEventMatchingMask:NSAnyEventMask
                                              untilDate:[NSDate distantPast]
                                                 inMode:NSDefaultRunLoopMode
                                                dequeue:YES];


  return self->lastEvent != NULL;
#endif
  // And unlike wxWidgets we can't just return 1, so instead we alternate
  // between such.
  return (self->pendingTicker = !self->pendingTicker);
};

- (void)getNextEvent {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSEvent *ev;

  while ((ev = [self->window nextEventMatchingMask:NSAnyEventMask
                                         untilDate:[NSDate distantPast]
                                            inMode:NSDefaultRunLoopMode
                                           dequeue:YES])) {
    [self eventProcess:ev];
  }

  [self sendClipboardEvent];
  [pool release];
};

- (void)eventProcess:(NSEvent *)ev {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSWindow *win = [ev window];
  NSWindow *parentWindow = [self parentWindow];
  MwLL h;
  MwBool doSendEvent = MwTRUE;
  MwLL this = self->handle.pointer;

  if (!win) {
    [pool release];
    return;
  }

  if ([win contentView].subviews.count == 0) {
    printf("no subviews on %p\n", win);
    [pool release];
    return;
  } else {
    h = [((MilskoFakePointer *)[win contentView].subviews[0]) pointer];
  }

  switch (ev.type) {
  case NSEventTypeLeftMouseDown:
  case NSEventTypeRightMouseDown:
  case NSEventTypeOtherMouseDown:
  case NSEventTypeLeftMouseUp:
  case NSEventTypeRightMouseUp:
  case NSEventTypeOtherMouseUp: {
    MwLLMouse mouse = {};
    MwBool isDown = MwTRUE;
    CGPoint mousePoint = pointFlip([ev locationInWindow]);
    switch (ev.type) {
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
  case NSEventTypeLeftMouseDragged:
  case NSEventTypeRightMouseDragged:
  case NSEventTypeOtherMouseDragged:
  case NSEventTypeMouseMoved: {
    MwPoint pos;
    pos.x = [ev locationInWindow].x;
    pos.y = [win contentRectForFrameRect:win.frame].size.height -
            [ev locationInWindow].y;
    MwLLDispatch(h, move, &pos);
    break;
  }
  case NSEventTypeMouseEntered:
    MwLLDispatch(h, focus_in, NULL);
    break;
  case NSEventTypeMouseExited:
    MwLLDispatch(h, focus_out, NULL);
    break;
  case NSEventTypeKeyDown:
  case NSEventTypeKeyUp: {
    int ch;
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
      kVK_ANSI_KeypadDecimal = 0x41,
      kVK_ANSI_KeypadMultiply = 0x43,
      kVK_ANSI_KeypadPlus = 0x45,
      kVK_ANSI_KeypadClear = 0x47,
      kVK_ANSI_KeypadDivide = 0x4B,
      kVK_ANSI_KeypadEnter = 0x4C,
      kVK_ANSI_KeypadMinus = 0x4E,
      kVK_ANSI_KeypadEquals = 0x51,
      kVK_ANSI_Keypad0 = 0x52,
      kVK_ANSI_Keypad1 = 0x53,
      kVK_ANSI_Keypad2 = 0x54,
      kVK_ANSI_Keypad3 = 0x55,
      kVK_ANSI_Keypad4 = 0x56,
      kVK_ANSI_Keypad5 = 0x57,
      kVK_ANSI_Keypad6 = 0x58,
      kVK_ANSI_Keypad7 = 0x59,
      kVK_ANSI_Keypad8 = 0x5B,
      kVK_ANSI_Keypad9 = 0x5C,
      kVK_Return = 0x24,
      kVK_Tab = 0x30,
      kVK_Space = 0x31,
      kVK_Delete = 0x33,
      kVK_Escape = 0x35,
      kVK_Command = 0x37,
      kVK_Shift = 0x38,
      kVK_CapsLock = 0x39,
      kVK_Option = 0x3A,
      kVK_Control = 0x3B,
      kVK_RightCommand = 0x36,
      kVK_RightShift = 0x3C,
      kVK_RightOption = 0x3D,
      kVK_RightControl = 0x3E,
      kVK_Function = 0x3F,
      kVK_F17 = 0x40,
      kVK_VolumeUp = 0x48,
      kVK_VolumeDown = 0x49,
      kVK_Mute = 0x4A,
      kVK_F18 = 0x4F,
      kVK_F19 = 0x50,
      kVK_F20 = 0x5A,
      kVK_F5 = 0x60,
      kVK_F6 = 0x61,
      kVK_F7 = 0x62,
      kVK_F3 = 0x63,
      kVK_F8 = 0x64,
      kVK_F9 = 0x65,
      kVK_F11 = 0x67,
      kVK_F13 = 0x69,
      kVK_F16 = 0x6A,
      kVK_F14 = 0x6B,
      kVK_F10 = 0x6D,
      kVK_F12 = 0x6F,
      kVK_F15 = 0x71,
      kVK_Help = 0x72,
      kVK_Home = 0x73,
      kVK_PageUp = 0x74,
      kVK_ForwardDelete = 0x75,
      kVK_F4 = 0x76,
      kVK_End = 0x77,
      kVK_F2 = 0x78,
      kVK_PageDown = 0x79,
      kVK_F1 = 0x7A,
      kVK_LeftArrow = 0x7B,
      kVK_RightArrow = 0x7C,
      kVK_DownArrow = 0x7D,
      kVK_UpArrow = 0x7E
    };
    // [view.nextResponder
    //     interpretKeyEvents:[NSArray arrayWithObject:lastEvent]];
    switch (ev.keyCode) {
    case kVK_ANSI_A:
      ch = 'a';
      break;
    case kVK_ANSI_B:
      ch = 'b';
      break;
    case kVK_ANSI_C:
      ch = 'c';
      break;
    case kVK_ANSI_D:
      ch = 'd';
      break;
    case kVK_ANSI_E:
      ch = 'e';
      break;
    case kVK_ANSI_F:
      ch = 'f';
      break;
    case kVK_ANSI_G:
      ch = 'g';
      break;
    case kVK_ANSI_H:
      ch = 'h';
      break;
    case kVK_ANSI_I:
      ch = 'i';
      break;
    case kVK_ANSI_J:
      ch = 'j';
      break;
    case kVK_ANSI_K:
      ch = 'k';
      break;
    case kVK_ANSI_L:
      ch = 'l';
      break;
    case kVK_ANSI_M:
      ch = 'm';
      break;
    case kVK_ANSI_N:
      ch = 'n';
      break;
    case kVK_ANSI_O:
      ch = 'o';
      break;
    case kVK_ANSI_P:
      ch = 'p';
      break;
    case kVK_ANSI_Q:
      ch = 'q';
      break;
    case kVK_ANSI_R:
      ch = 'r';
      break;
    case kVK_ANSI_S:
      ch = 's';
      break;
    case kVK_ANSI_T:
      ch = 't';
      break;
    case kVK_ANSI_U:
      ch = 'u';
      break;
    case kVK_ANSI_V:
      ch = 'v';
      break;
    case kVK_ANSI_W:
      ch = 'w';
      break;
    case kVK_ANSI_X:
      ch = 'x';
      break;
    case kVK_ANSI_Y:
      ch = 'y';
      break;
    case kVK_ANSI_Z:
      ch = 'z';
      break;
    case kVK_ANSI_0:
      ch = '0';
      break;
    case kVK_ANSI_1:
      ch = '1';
      break;
    case kVK_ANSI_2:
      ch = '2';
      break;
    case kVK_ANSI_3:
      ch = '3';
      break;
    case kVK_ANSI_4:
      ch = '4';
      break;
    case kVK_ANSI_5:
      ch = '5';
      break;
    case kVK_ANSI_6:
      ch = '6';
      break;
    case kVK_ANSI_7:
      ch = '7';
      break;
    case kVK_ANSI_8:
      ch = '8';
      break;
    case kVK_ANSI_9:
      ch = '9';
      break;

      // case kVK_ANSI_Keypad0:
      //   ch = MwLLKey;
      //   break;
      // case kVK_ANSI_Keypad1:
      //   ch = keypad1;
      //   break;
      // case kVK_ANSI_Keypad2:
      //   ch = keypad2;
      //   break;
      // case kVK_ANSI_Keypad3:
      //   ch = keypad3;
      //   break;
      // case kVK_ANSI_Keypad4:
      //   ch = keypad4;
      //   break;
      // case kVK_ANSI_Keypad5:
      //   ch = keypad5;
      //   break;
      // case kVK_ANSI_Keypad6:
      //   ch = keypad6;
      //   break;
      // case kVK_ANSI_Keypad7:
      //   ch = keypad7;
      //   break;
      // case kVK_ANSI_Keypad8:
      //   ch = keypad8;
      //   break;
      // case kVK_ANSI_Keypad9:
      //   ch = keypad9;
      //   break;
      // case kVK_ANSI_KeypadClear:
      //   ch = keypadClear;
      //   break;
      // case kVK_ANSI_KeypadDivide:
      //   ch = keypadDivide;
      //   break;
      // case kVK_ANSI_KeypadEnter:
      //   ch = keypadEnter;
      //   break;
      // case kVK_ANSI_KeypadEquals:
      //   ch = keypadEquals;
      //   break;
      // case kVK_ANSI_KeypadMinus:
      //   ch = keypadMinus;
      //   break;
      // case kVK_ANSI_KeypadPlus:
      //   ch = keypadPlus;
      //   break;
      // case kVK_PageDown:
      //   ch = MwLLKey;
      //   break;
      // case kVK_PageUp:
      //   ch = pageUp;
      //   break;
      // case kVK_End:
      //   ch = end;
      //   break;
      // case kVK_Home:
      //   ch = home;
      //   break;

      // case kVK_F1:
      //   ch = f1;
      //   break;
      // case kVK_F2:
      //   ch = f2;
      //   break;
      // case kVK_F3:
      //   ch = f3;
      //   break;
      // case kVK_F4:
      //   ch = f4;
      //   break;
      // case kVK_F5:
      //   ch = f5;
      //   break;
      // case kVK_F6:
      //   ch = f6;
      //   break;
      // case kVK_F7:
      //   ch = f7;
      //   break;
      // case kVK_F8:
      //   ch = f8;
      //   break;
      // case kVK_F9:
      //   ch = f9;
      //   break;
      // case kVK_F10:
      //   ch = f10;
      //   break;
      // case kVK_F11:
      //   ch = f11;
      //   break;
      // case kVK_F12:
      //   ch = f12;
      //   break;
      // case kVK_F13:
      //   ch = f13;
      //   break;
      // case kVK_F14:
      //   ch = f14;
      //   break;
      // case kVK_F15:
      //   ch = f15;
      //   break;
      // case kVK_F16:
      //   ch = f16;
      //   break;
      // case kVK_F17:
      //   ch = f17;
      //   break;
      // case kVK_F18:
      //   ch = f18;
      //   break;
      // case kVK_F19:
      //   ch = f19;
      //   break;
      // case kVK_F20:
      //   ch = f20;
      //   break;
      // case kVK_ANSI_KeypadDecimal:
      //   ch = decimal;
      //   break;

    case kVK_ANSI_Quote:
      ch = '\"';
      break;
    case kVK_ANSI_Grave:
      ch = '`';
      break;
    case kVK_ANSI_Backslash:
      ch = '/';
      break;
    case kVK_ANSI_Comma:
      ch = ',';
      break;
    // case kVK_Delete:
    //   ch = delete;
    //   break;
    // case kVK_ANSI_Equal:
    //   ch = equals;
    //   break;
    case kVK_Escape:
      ch = MwLLKeyEscape;
      break;
    // case kVK_ANSI_LeftBracket:
    //   ch = leftBracket;
    //   break;
    // case kVK_ANSI_Minus:
    //   ch = minus;
    //   break;
    // case kVK_ANSI_KeypadMultiply:
    //   ch = multiply;
    //   break;
    // case kVK_ANSI_Period:
    //   ch = period;
    //   break;
    case kVK_Return:
      ch = MwLLKeyEnter;
      break;
    // case kVK_ANSI_RightBracket:
    //   ch = rightBracket;
    //   break;
    case kVK_ANSI_Semicolon:
      ch = ';';
      break;
    case kVK_ANSI_Slash:
      ch = '\\';
      break;
    case kVK_Space:
      ch = ' ';
      break;
    // case kVK_Tab:
    //   ch = tab;
    //   break;

    // case kVK_Command:
    //   ch = MwLLKey;
    //   break;
    // case kVK_RightCommand:
    //   ch = rightCommand;
    //   break;
    case kVK_Control:
      ch = MwLLKeyControl;
      break;
    case kVK_RightControl:
      ch = MwLLKeyControl;
      break;
    // case kVK_Function:
    //   ch = function;
    //   break;
    // case kVK_Option:
    //   ch = option;
    //   break;
    // case kVK_RightOption:
    //   ch = rightOption;
    //   break;
    case kVK_Shift:
      ch = MwLLKeyLeftShift;
      break;
    case kVK_RightShift:
      ch = MwLLKeyRightShift;
      break;

    case kVK_DownArrow:
      ch = MwLLKeyDown;
      break;
    case kVK_LeftArrow:
      ch = MwLLKeyLeft;
      break;
    case kVK_RightArrow:
      ch = MwLLKeyRight;
      break;
    case kVK_UpArrow:
      ch = MwLLKeyUp;
      break;
    }
    switch (ev.type) {
    case NSEventTypeKeyDown:
      MwLLDispatch(this, key, &ch);
      break;
    case NSEventTypeKeyUp:
      MwLLDispatch(this, key_released, &ch);
      break;
    default:
      break;
    }
    doSendEvent = MwFALSE;
    break;
  }
  case NSEventTypeCursorUpdate:
    break;
  case NSEventTypeScrollWheel:
    break;
  default:
    break;
  };
  if (doSendEvent) {
    [win sendEvent:ev];
  }

  [pool release];
}

- (void)sendClipboardEvent {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  NSArray *items = @[
    @"public.utf8-plain-text",
    @"public.utf16-external-plain-text",
    @"com.apple.traditional-mac-plain-text",
  ];
  MwLL this = self->handle.pointer;

  if ([pasteboard canReadItemWithDataConformingToTypes:items]) {
    char *data = NULL;
    size_t size = 0;
    for (NSPasteboardItem *item in [pasteboard pasteboardItems]) {
      for (NSString *it in items) {
        NSString *itemData = [item stringForType:(NSString *)it];
        if (itemData != NULL) {
          if (strHash != 0 && strHash != [itemData hash]) {
            char *text = malloc([itemData length]);
            strncpy(text, [itemData UTF8String], [itemData length]);
            MwLLDispatch(this, clipboard, text);
            printf("%s -> %p\n", text, this);
            free(text);
          }
          strHash = [itemData hash];
        }
      }
    }
  }

  [pool release];
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
  [pool release];
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
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard declareTypes:@[ NSPasteboardTypeString ] owner:nil];
  [pasteboard setString:@(text) forType:NSPasteboardTypeString];
  [pool release];
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
  [self->window release];
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
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSSize sz = self->rep.size;
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
}

- (void)setFrameSize:(NSSize)newSize {
  [super setFrameSize:newSize];
  [self->rep setSize:newSize];
}

- (void)displayRect:(NSRect)rect {
};

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

// This will close/terminate the application when the main window is closed.
- (void)windowWillClose:(NSNotification *)notification {
  // MilskoCocoa *window = notification.object;
  // MwLL handle = [window getHandle].pointer;
  // MwLLDispatch(handle, close, NULL);
  [NSApp terminate:nil];
}

- (instancetype)initWithWin:(NSWindow *)win {
  self->w = win;
  return self;
}

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
