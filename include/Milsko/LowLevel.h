/* $Id$ */
#ifndef __MILSKO_LL_H__
#define __MILSKO_LL_H__

#ifdef _MILSKO
#ifdef USE_X11
#include <Milsko/X11.h>
#endif
#ifdef USE_GDI
#include <Milsko/GDI.h>
#endif
#else
typedef void* HMILSKOLL;
typedef void* HMILSKOCOLOR;
#endif

#include <Milsko/MachDep.h>
#include <Milsko/TypeDef.h>

MILSKODECL HMILSKOLL	MilskoLLCreate(HMILSKOLL parent, int x, int y, int width, int height);
MILSKODECL void		MilskoLLDestroy(HMILSKOLL handle);
MILSKODECL void		MilskoLLPolygon(HMILSKOLL handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color);
MILSKODECL HMILSKOCOLOR MilskoLLAllocColor(HMILSKOLL handle, int r, int g, int b);
MILSKODECL void		MilskoLLGetXYWH(HMILSKOLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
MILSKODECL void		MilskoLLSetXYWH(HMILSKOLL handle, int x, int y, unsigned int w, unsigned int h);
MILSKODECL int		MilskoLLPending(HMILSKOLL handle);
MILSKODECL void		MilskoLLNextEvent(HMILSKOLL handle);
MILSKODECL void		MilskoLLSleep(int ms);

#endif
