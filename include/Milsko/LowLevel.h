/* $Id$ */
#ifndef __MILSKO_LL_H__
#define __MILSKO_LL_H__

#include <Milsko/TypeDef.h>

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

HMILSKOLL    MilskoLLCreate(HMILSKOLL parent, int x, int y, int width, int height);
void	     MilskoLLDestroy(HMILSKOLL handle);
void	     MilskoLLPolygon(HMILSKOLL handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color);
HMILSKOCOLOR MilskoLLAllocColor(HMILSKOLL handle, int r, int g, int b);
void	     MilskoLLGetXYWH(HMILSKOLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
int	     MilskoLLPending(HMILSKOLL handle);
void	     MilskoLLNextEvent(HMILSKOLL handle);

#endif
