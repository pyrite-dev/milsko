/* $Id$ */
#ifndef __MILSKO_TYPEDEFS_H__
#define __MILSKO_TYPEDEFS_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoPoint {
	int x;
	int y;
} MilskoPoint;

typedef struct _MilskoRect {
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
} MilskoRect;

typedef struct _MilskoTextKeyValue {
	char* key;
	char* value;
} MilskoTextKeyValue;

typedef struct _MilskoIntegerKeyValue {
	char* key;
	int   value;
} MilskoIntegerKeyValue;

typedef struct _MilskoClass* MilskoClass;

#ifdef _MILSKO
#include <Milsko/LowLevel.h>

typedef struct _Milsko* MilskoWidget;

typedef struct _Milsko {
	char* name;

	MilskoLL      lowlevel;
	MilskoWidget  parent;
	MilskoWidget* children;
	MilskoClass class;

	MilskoTextKeyValue*    text;
	MilskoIntegerKeyValue* integer;
}* MilskoWidget;
#else
typedef void* MilskoWidget;
#endif

typedef void (*MilskoHandler)(MilskoWidget handle);

typedef struct _MilskoClass {
	void*	      opaque;
	MilskoHandler create;
	MilskoHandler destroy;
	MilskoHandler draw;
	MilskoHandler click;
} *MilskoClass, MilskoClassRec;

#endif
