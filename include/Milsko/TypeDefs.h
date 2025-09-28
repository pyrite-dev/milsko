/* $Id$ */
#ifndef __MILSKO_TYPEDEFS_H__
#define __MILSKO_TYPEDEFS_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoClass *	      MilskoClass, MilskoClassRec;
typedef struct _MilskoPoint	      MilskoPoint;
typedef struct _MilskoRect	      MilskoRect;
typedef struct _MilskoTextKeyValue    MilskoTextKeyValue;
typedef struct _MilskoIntegerKeyValue MilskoIntegerKeyValue;
#ifdef _MILSKO
typedef struct _MilskoWidget *MilskoWidget, MilskoWidgetRec;
#else
typedef void* MilskoWidget;
#endif
typedef void (*MilskoHandler)(MilskoWidget handle);

#ifdef _MILSKO
#include <Milsko/LowLevel.h>
#endif

#define MilskoDispatch(x, y) \
	if(x->class != NULL && x->class->y != NULL) x->class->y(x)

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

#ifdef _MILSKO
struct _MilskoWidget {
	char* name;

	MilskoLL      lowlevel;
	MilskoWidget  parent;
	MilskoWidget* children;
	MilskoClass class;

	int pressed;

	MilskoTextKeyValue*    text;
	MilskoIntegerKeyValue* integer;
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
