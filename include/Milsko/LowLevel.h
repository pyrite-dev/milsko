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
typedef void* MilskoLL;
typedef void* MilskoLLColor;
#endif

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

MILSKODECL MilskoLL MilskoLLCreate(MilskoLL parent, int x, int y, int width, int height);
MILSKODECL void	    MilskoLLDestroy(MilskoLL handle);

MILSKODECL void MilskoLLPolygon(MilskoLL handle, MilskoPoint* points, int points_count, MilskoLLColor color);

MILSKODECL MilskoLLColor MilskoLLAllocColor(MilskoLL handle, int r, int g, int b);
MILSKODECL void		 MilskoLLFreeColor(MilskoLLColor color);

MILSKODECL void MilskoLLGetXYWH(MilskoLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
MILSKODECL void MilskoLLSetXY(MilskoLL handle, int x, int y);
MILSKODECL void MilskoLLSetWH(MilskoLL handle, int w, int h);

MILSKODECL void MilskoLLSetTitle(MilskoLL handle, const char* title);

MILSKODECL int	MilskoLLPending(MilskoLL handle);
MILSKODECL void MilskoLLNextEvent(MilskoLL handle);
MILSKODECL void MilskoLLSleep(int ms);

#endif
