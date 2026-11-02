/*!
 * @file Mw/LowLevel/Cocoa.h
 * @brief Cocoa Backend
 * @warning This is used internally.
 */
#ifndef __MW_LOWLEVEL_COCOA_H__
#define __MW_LOWLEVEL_COCOA_H__

#include <Mw/LowLevel.h>
#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <ApplicationServices/ApplicationServices.h>

@class MilskoCocoaWindow;
@class MilskoCocoaView;

#define MW_COCOA_OBJC(x) x
#else
#define MW_COCOA_OBJC(x) void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MwLLCocoaEvent {
	int	type;
	MwMouse mouse;
	int	key;
	char*	data;
} MwLLCocoaEvent;

struct _MwLLCocoa {
	struct _MwLLCommon common;

	MwLL  parent;
	MwLL* children;

	MW_COCOA_OBJC(MilskoCocoaWindow*) window;
	MW_COCOA_OBJC(MilskoCocoaView*) view;
	MW_COCOA_OBJC(NSString*) title;
	MW_COCOA_OBJC(NSCursor*) cursor;
	MW_COCOA_OBJC(NSWindow*) popup_owner;

	MW_COCOA_OBJC(CGContextRef) context;
	unsigned char* buffer;

	MwLLCocoaEvent* events;
	long		last_pump;

	int x;
	int y;
	int width;
	int height;

	int shown;
	int pending_show;
	int dirty;
	int in_state_change;

	int borderless;
	int tool;
	int min_w, min_h, max_w, max_h;

	int    has_clip;
	MwRect clip;

	int accepts_dnd;

	MwLL	     focus;
	MwLL	     capture;
	int	     capture_button;
	MwLL	     hover;
	MwLL	     grab;
	unsigned int last_flags;
	double	     scroll_acc;
};

struct _MwLLCocoaColor {
	struct _MwLLCommonColor common;
};

struct _MwLLCocoaPixmap {
	struct _MwLLCommonPixmap common;

	unsigned char* premultiplied;
	MW_COCOA_OBJC(CGImageRef) image;
};

MWDECL int MwLLCocoaCallInit(void);

/*!
 * @brief Gets the NSView of the handle
 * @param handle Handle
 * @return NSView
 */
MWDECL MW_COCOA_OBJC(NSView*) MwLLCocoaGetView(MwLL handle);

#ifdef __cplusplus
}
#endif

#endif
