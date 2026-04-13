/*!
 * @file Mw/LowLevel/Cocoa.h
 * @brief Work in progress Cocoa Backend
 * @warning This is used internally.
 */
#ifndef __MW_LOWLEVEL_COCOA_H__
#define __MW_LOWLEVEL_COCOA_H__

#include <Mw/LowLevel.h>
#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSGeometry.h>

#ifdef __APPLE__
#import <CoreServices/CoreServices.h>
#else
#import <CoreGraphics/CoreGraphics.h>
#endif

@interface MilskoCocoaApplication : NSApplication {
	MwBool doPending;
}
- (MwBool)pending;
@end

// Note: implements NSApplicationDelegate
@interface MilskoCocoaApplicationDelegate : NSObject {
	MilskoCocoaApplication* appl;
}
- (MilskoCocoaApplicationDelegate*)initWithAppl:(MilskoCocoaApplication*)appl;
@end

@interface MilskoCocoaWindow : NSWindow {
}
@end

// Note: implements NSWindowDelegate
@interface MilskoCocoaWindowDelegate : NSObject {
	MilskoCocoaWindow* w;
}
- (MilskoCocoaWindowDelegate*)initWithWin:(MilskoCocoaWindow*)win;
@end

/*
 So we want to associate each NSWindow with its corresponding MwLL handle. The
 conventional way of doing this is through "associative pointers", however
 this is a feature that Apple added in 10.6 (marketted back then as "Objective
 C 2" iirc). For 10.4 compatibility, we have to do a bit of an ugly hack that
 might seem like horrifically undefined behavior, but I've tested this on
 both 10.4 and modern Mac OS and as long as we're careful there's no
 problems: we override NSView, use NSView's "frame" property to store the
 pointer, and then override functions appropriately so that this frame is never
 actually used. This can then be attached to an NSWindow with addSubview and
 retrieved appropriately.
 */
@interface MilskoFakePointer : NSView {
	void* ptr;
}

- (void)setPointer:(void*)ptr;
- (void*)pointer;

@end

@interface MilskoCocoaPixmap : NSObject {
	MwBool		  valid;
	int		  width;
	int		  height;
	unsigned char*	  buf;
	NSImage*	  image;
	NSBitmapImageRep* rep;
}

+ (MilskoCocoaPixmap*)newWithWidth:(int)width height:(int)height;
- (void)updateWithData:(unsigned char*)data;
- (void)destroy;

/* using @property to create instance variables fucks up 10.4 gcc for some
 * reason? */
- (NSImage*)image;

@end

@interface MilskoCocoaView : NSView {
	NSBitmapImageRep*  rep;
	NSGraphicsContext* context;
	MwBool		   valid;
	CGColorSpaceRef	   space;
	CGDataProviderRef  provider;
	NSRect		   givenRect;
	float		   x;
	float		   y;
	float		   width;
	float		   height;
	MwBool		   _child;
}

- (void)initRepAndContextWithWidth:(float)w Height:(float)h;
- (NSGraphicsContext*)context;
- (void)destroy;
- (NSBitmapImageRep*)getRep;
- (void)setChild;
@end

@interface MilskoCocoaSubView : MilskoCocoaView {
}
@end

@interface MilskoCocoa : NSObject {
	MilskoCocoaApplication* application;
	MwBool			_forceRender;
	MwBool			_eventsPending;
	MilskoCocoaWindow*	window;
	NSRect			rect;
	MilskoCocoaView*	view;
	MwLL			parent;
	MilskoFakePointer*	handle;
	unsigned int		strHash;
	NSEvent*		lastEvent;

	MilskoCocoaPixmap* cursorPixmap;
	NSCursor*	   cursor;
	NSModalSession	   modalSession;

	MwBool pointerLocked;
	MwBool mouseMoved;

	MwBool rejectFirstTest;
}

+ (MilskoCocoa*)newWithParent:(MwLL)parent
			    x:(int)x
			    y:(int)y
			width:(int)width
		       height:(int)height
		       handle:(MwLL)handle;
- (void)polygonWithPoints:(MwPoint*)points
	     points_count:(int)points_count
		    color:(MwLLColor)color;
- (void)lineWithPoints:(MwPoint*)points color:(MwLLColor)color;
- (void)getX:(int*)x Y:(int*)y W:(unsigned int*)w H:(unsigned int*)h;
- (void)setX:(int)x Y:(int)y;
- (void)setW:(int)w H:(int)h;
- (int)pending;
- (void)getNextEvent;
- (void)setTitle:(const char*)title;
- (void)drawPixmap:(MwLLPixmap)pixmap rect:(MwRect*)rect;
- (void)setIcon:(MwLLPixmap)pixmap;
- (void)forceRender;
- (void)setCursor:(MwCursor*)image mask:(MwCursor*)mask;
- (void)detachWithPoint:(MwPoint*)point;
- (void)show:(int)show;
- (void)makePopupWithParent:(MwLL)parent;
- (void)setSizeHintsWithMinX:(int)minx
			MinY:(int)miny
			MaxX:(int)maxx
			MaxY:(int)maxy;
- (void)makeBorderless:(int)toggle;
- (void)focus;
- (void)grabPointer:(int)toggle;
- (void)setClipboard:(const char*)text;
- (void)makeToolWindow;
- (void)getCursorCoord:(MwPoint*)point;
- (void)getScreenSize:(MwRect*)rect;
- (void)destroy;
- (void)sendClipboardEvent;

- (MwLL)getParent;
- (NSView*)getView;
- (MilskoCocoaWindow*)getWindow;
- (void)nudge;

- (void)pushCursor;
- (void)popCursor;

- (MilskoFakePointer*)getHandle;
+ (void)eventCanceller:(MilskoCocoa*)this;

@end
#define OBJC(x) x
#else
#define OBJC(x) void*
#endif

MWDECL int MwLLCocoaCallInit(void);

struct _MwLLCocoa {
	struct _MwLLCommon common;
	OBJC(MilskoCocoa*)
	real;
};

struct _MwLLCocoaColor {
	struct _MwLLCommonColor common;
};

struct _MwLLCocoaPixmap {
	struct _MwLLCommonPixmap common;
	OBJC(MilskoCocoaPixmap*)
	real;
};

#endif
