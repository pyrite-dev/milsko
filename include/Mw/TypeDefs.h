/* $Id$ */
/*!
 * %file Mw/TypeDefs.h
 * %brief Type definitions
 */
#ifndef __MW_TYPEDEFS_H__
#define __MW_TYPEDEFS_H__

#include <Mw/MachDep.h>

typedef struct _MwClass *	      MwClass, MwClassRec;
typedef struct _MwPoint		      MwPoint;
typedef struct _MwRect		      MwRect;
typedef struct _MwIntegerKeyValue     MwIntegerKeyValue;
typedef struct _MwTextKeyValue	      MwTextKeyValue;
typedef struct _MwUserHandlerKeyValue MwUserHandlerKeyValue;
typedef struct _MwVoidKeyValue	      MwVoidKeyValue;
typedef struct _MwFont		      MwFont;
typedef struct _MwMenu*		      MwMenu;
typedef struct _MwCursor	      MwCursor;
#ifdef _MILSKO
typedef struct _MwWidget* MwWidget;
#else
typedef void* MwWidget;
#endif
typedef void (*MwHandler)(MwWidget handle);
typedef int (*MwHandler2)(MwWidget handle);
typedef int (*MwHandler3)(MwWidget handle, const char* key);
typedef void (*MwUserHandler)(MwWidget handle, void* user_data, void* call_data);
typedef void (*MwErrorHandler)(int code, const char* message, void* user_data);

#ifdef _MILSKO
#include <Mw/LowLevel.h>
#endif

struct _MwPoint {
	int x;
	int y;
};

struct _MwRect {
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
};

struct _MwTextKeyValue {
	char* key;
	char* value;
};

struct _MwIntegerKeyValue {
	char* key;
	int   value;
};

struct _MwUserHandlerKeyValue {
	char*	      key;
	void*	      user_data;
	MwUserHandler value;
};

struct _MwVoidKeyValue {
	char* key;
	void* value;
};

#ifdef _MILSKO
struct _MwWidget {
	char* name;

	MwLL	  lowlevel;
	MwWidget  parent;
	MwWidget* children;
	MwClass	  widget_class;

	int	pressed;
	MwPoint mouse_point;
	int	close;
	jmp_buf before_step;

	void* internal;

	MwIntegerKeyValue*     integer;
	MwTextKeyValue*	       text;
	MwUserHandlerKeyValue* handler;
	MwVoidKeyValue*	       data;

	MwWidget* destroy_queue;
};
#endif

struct _MwMenu {
	char*	 name;
	int	 keep;
	MwWidget wsub;
	MwMenu*	 sub;
};

#define MwCursorDataHeight 16
struct _MwCursor {
	int	     width;
	int	     height;
	int	     x;
	int	     y;
	unsigned int data[MwCursorDataHeight];
};

struct _MwClass {
	MwHandler2 create;
	MwHandler  destroy;
	MwHandler  draw;
	MwHandler  click;
	MwHandler  parent_resize;
	MwHandler3 prop_change;
	MwHandler  mouse_move;
	MwHandler  mouse_up;
	MwHandler  mouse_down;
};

struct _MwFont {
	int	      left;
	int	      top;
	unsigned char data[16];
};

#endif
