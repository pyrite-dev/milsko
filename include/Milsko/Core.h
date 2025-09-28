/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoClass* MilskoClass;

#ifdef _MILSKO
#include <Milsko/LowLevel.h>

typedef struct _Milsko* HMILSKO;

typedef struct _Milsko {
	HMILSKOLL lowlevel;
	HMILSKO	  parent;
	HMILSKO*  children;
	MilskoClass class;
}* HMILSKO;
#else
typedef void* HMILSKO;
#endif

typedef struct _MilskoClass {
}* MilskoClass;

MILSKODECL HMILSKO MilskoCreateWidget(MilskoClass class, HMILSKO parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL void	   MilskoDestroyWidget(HMILSKO handle);

#endif
