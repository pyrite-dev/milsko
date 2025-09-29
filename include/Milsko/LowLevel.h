/* $Id$ */
#ifndef __MILSKO_LOWLEVEL_H__
#define __MILSKO_LOWLEVEL_H__

#include <Mw/MachDep.h>

typedef struct _MwLLHandler *MwLLHandler, MwLLHandlerRec;
#ifdef _MILSKO
typedef struct _MwLL *     MwLL, MwLLRec;
typedef struct _MwLLColor *MwLLColor, MwLLColorRec;
#else
typedef void* MwLL;
typedef void* MwLLColor;
#endif

#ifdef _MILSKO
#ifdef USE_X11
#include <Mw/X11.h>
#endif
#ifdef USE_GDI
#include <Mw/GDI.h>
#endif
#endif
#include <Mw/TypeDefs.h>

#define MwLLDispatch(x, y) \
	if(x->handler != NULL && x->handler->y != NULL) x->handler->y(x)

struct _MwLLHandler {
	void (*draw)(MwLL handle);
	void (*up)(MwLL handle);
	void (*down)(MwLL handle);
};

/* lowlevel.c, common part */
MILSKODECL void MwLLCreateCommon(MwLL handle);
MILSKODECL void MwLLDestroyCommon(MwLL handle);

/* driver-specific */
MILSKODECL MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height);
MILSKODECL void	    MwLLDestroy(MwLL handle);

MILSKODECL void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color);

MILSKODECL MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b);
MILSKODECL void		 MwLLFreeColor(MwLLColor color);

MILSKODECL void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
MILSKODECL void MwLLSetXY(MwLL handle, int x, int y);
MILSKODECL void MwLLSetWH(MwLL handle, int w, int h);

MILSKODECL void MwLLSetTitle(MwLL handle, const char* title);

MILSKODECL int	MwLLPending(MwLL handle);
MILSKODECL void MwLLNextEvent(MwLL handle);
MILSKODECL void MwLLSleep(int ms);

#endif
