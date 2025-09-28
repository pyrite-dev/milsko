/* $Id$ */
#ifndef __MILSKO_LOWLEVEL_H__
#define __MILSKO_LOWLEVEL_H__

#include <Milsko/MachDep.h>

typedef struct _MilskoLLHandler *MilskoLLHandler, MilskoLLHandlerRec;
#ifdef _MILSKO
typedef struct _MilskoLL *     MilskoLL, MilskoLLRec;
typedef struct _MilskoLLColor *MilskoLLColor, MilskoLLColorRec;
#else
typedef void* MilskoLL;
typedef void* MilskoLLColor;
#endif

#ifdef _MILSKO
#ifdef USE_X11
#include <Milsko/X11.h>
#endif
#ifdef USE_GDI
#include <Milsko/GDI.h>
#endif
#endif
#include <Milsko/TypeDefs.h>

#define MilskoLLDispatch(x, y) \
	if(x->handler != NULL && x->handler->y != NULL) x->handler->y(x)

struct _MilskoLLHandler {
	void (*draw)(MilskoLL handle);
	void (*up)(MilskoLL handle);
	void (*down)(MilskoLL handle);
};

/* lowlevel.c, common part */
MILSKODECL void MilskoLLCreateCommon(MilskoLL handle);
MILSKODECL void MilskoLLDestroyCommon(MilskoLL handle);

/* driver-specific */
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
