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
@interface MilskoCocoaPixmap : NSObject {
	NSImage* image;
}
+ (MilskoCocoaPixmap*)newWithWidth:(int)width height:(int)height;
- (void)updateWithData:(void*)data;
- (void)destroy;
@end
@interface MilskoCocoa : NSObject {
	NSApplication* application;
	NSWindow*      window;
	NSRect	       rect;
}

+ (MilskoCocoa*)newWithParent:(MwLL)parent
			    x:(int)x
			    y:(int)y
			width:(int)width
		       height:(int)height;
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
