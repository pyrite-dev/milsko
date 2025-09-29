/* $Id$ */
#ifndef __MILSKO_TYPEDEFS_H__
#define __MILSKO_TYPEDEFS_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoClass *		  MilskoClass, MilskoClassRec;
typedef struct _MilskoPoint		  MilskoPoint;
typedef struct _MilskoRect		  MilskoRect;
typedef struct _MilskoIntegerKeyValue	  MilskoIntegerKeyValue;
typedef struct _MilskoTextKeyValue	  MilskoTextKeyValue;
typedef struct _MilskoUserHandlerKeyValue MilskoUserHandlerKeyValue;
#ifdef _MILSKO
typedef struct _MilskoWidget *MilskoWidget, MilskoWidgetRec;
#else
typedef void* MilskoWidget;
#endif
typedef void (*MilskoHandler)(MilskoWidget handle);
typedef void (*MilskoUserHandler)(MilskoWidget handle, void* user_data, void* call_data);

#ifdef _MILSKO
#include <Milsko/LowLevel.h>
#endif

struct _MilskoPoint {
	int x;
	int y;
};

struct _MilskoRect {
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
};

struct _MilskoTextKeyValue {
	char* key;
	char* value;
};

struct _MilskoIntegerKeyValue {
	char* key;
	int   value;
};

struct _MilskoUserHandlerKeyValue {
	char*		  key;
	void*		  user_data;
	MilskoUserHandler value;
};

#ifdef _MILSKO
struct _MilskoWidget {
	char* name;

	MilskoLL      lowlevel;
	MilskoWidget  parent;
	MilskoWidget* children;
	MilskoClass class;

	int pressed;

	MilskoIntegerKeyValue*	   integer;
	MilskoTextKeyValue*	   text;
	MilskoUserHandlerKeyValue* handler;
};
#endif

struct _MilskoClass {
	void*	      opaque;
	MilskoHandler create;
	MilskoHandler destroy;
	MilskoHandler draw;
	MilskoHandler click;
};

#endif
