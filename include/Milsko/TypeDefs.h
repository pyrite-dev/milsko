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

typedef struct _Milsko* HMILSKO;

typedef struct _Milsko {
	HMILSKOLL lowlevel;
	HMILSKO	  parent;
	HMILSKO*  children;
	MilskoClass class;

	MilskoTextKeyValue*    text;
	MilskoIntegerKeyValue* integer;
}* HMILSKO;
#else
typedef void* HMILSKO;
#endif

typedef struct _MilskoClass {
	void* opaque;
	void (*create)(HMILSKO handle);
	void (*destroy)(HMILSKO handle);
	void (*draw)(HMILSKO handle);
	void (*click)(HMILSKO handle);
} *MilskoClass, MilskoClassRec;

#endif
