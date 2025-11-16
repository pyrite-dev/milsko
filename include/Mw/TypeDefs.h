/* $Id$ */
/*!
 * @file Mw/TypeDefs.h
 * @brief Type definitions
 */
#ifndef __MW_TYPEDEFS_H__
#define __MW_TYPEDEFS_H__

#include <Mw/MachDep.h>
#include <Mw/BaseTypes.h>

typedef struct _MwClass *	      MwClass, MwClassRec;
typedef struct _MwIntegerKeyValue     MwIntegerKeyValue;
typedef struct _MwTextKeyValue	      MwTextKeyValue;
typedef struct _MwUserHandlerKeyValue MwUserHandlerKeyValue;
typedef struct _MwVoidKeyValue	      MwVoidKeyValue;
typedef struct _MwMenu*		      MwMenu;
typedef struct _MwEntry*	      MwEntry;
typedef struct _MwViewport*	      MwViewport;
typedef struct _MwListBox*	      MwListBox;
typedef struct _MwComboBox*	      MwComboBox;
typedef struct _MwListBoxEntry	      MwListBoxEntry;
typedef struct _MwDirectoryEntry      MwDirectoryEntry;
typedef struct _MwListBoxPacket	      MwListBoxPacket;
#ifdef _MILSKO
typedef struct _MwWidget* MwWidget;
#else
typedef void* MwWidget;
#endif
typedef void (*MwHandler)(MwWidget handle);
typedef int (*MwHandlerWithStatus)(MwWidget handle);
typedef void (*MwHandlerProp)(MwWidget handle, const char* key);
typedef void (*MwHandlerKey)(MwWidget handle, int key);
typedef void (*MwHandlerMouse)(MwWidget handle, void* ptr);
typedef void (*MwUserHandler)(MwWidget handle, void* user_data, void* call_data);
typedef void (*MwErrorHandler)(int code, const char* message, void* user_data);
typedef void (*MwHandlerExecute)(MwWidget handle, const char* name, void* out, va_list args);

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

#include <Mw/LowLevel.h>

#ifdef _MILSKO
struct _MwWidget {
	char*	  name;
	MwLLColor bgcolor;
	int	  dark_theme;

	MwLL	  lowlevel;
	MwWidget  parent;
	MwWidget* children;
	MwClass	  widget_class;

	int	pressed;
	MwPoint mouse_point;
	int	close;
	jmp_buf before_step;
	int	prop_event;

	void* internal;
	void* opaque;
	void (*draw_inject)(MwWidget handle);

	MwIntegerKeyValue*     integer;
	MwTextKeyValue*	       text;
	MwUserHandlerKeyValue* handler;
	MwVoidKeyValue*	       data;

	MwWidget* destroy_queue;
	MwWidget* tick_list;

	int destroyed;
};
#endif

struct _MwMenu {
	char*	 name;
	int	 keep;
	MwWidget wsub;
	MwMenu*	 sub;
};

struct _MwEntry {
	int	cursor;
	int	right;
	int	length;
	MwPoint mouse;
};

struct _MwViewport {
	MwWidget vscroll;
	MwWidget hscroll;
	MwWidget frame;
	MwWidget inframe;
	int	 vchanged;
	int	 hchanged;
};

struct _MwListBoxEntry {
	char**	   name;
	MwLLPixmap pixmap;
};

struct _MwListBox {
	MwWidget	vscroll;
	MwWidget	frame;
	MwListBoxEntry* list;
	int selected;
	unsigned long	click_time;
	int		pressed;
	int*		width;
	int		changed;
};

struct _MwComboBox {
	char**	 list;
	int	 opened;
	MwWidget listbox;
};

struct _MwDirectoryEntry {
	char*	 name;
	int	 type;
	MwOffset size;
	time_t	 mtime;
};

struct _MwListBoxPacket {
	MwLLPixmap* pixmaps;
	char***	    names;
};

struct _MwClass {
	MwHandlerWithStatus create;
	MwHandler	    destroy;
	MwHandler	    draw;
	MwHandler	    click;
	MwHandler	    parent_resize;
	MwHandlerProp	    prop_change;
	MwHandler	    mouse_move;
	MwHandlerMouse	    mouse_up;
	MwHandlerMouse	    mouse_down;
	MwHandlerKey	    key;
	MwHandlerExecute    execute;
	MwHandler	    tick;
	void*		    reserved3;
	void*		    reserved4;
	void*		    reserved5;
};

#endif
