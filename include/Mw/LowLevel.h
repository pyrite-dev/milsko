/* $Id$ */
#ifndef __MW_LOWLEVEL_H__
#define __MW_LOWLEVEL_H__

#include <Mw/MachDep.h>

typedef struct _MwLLHandler *MwLLHandler, MwLLHandlerRec;
#ifdef _MILSKO
typedef struct _MwLL *	   MwLL, MwLLRec;
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
	void (*resize)(MwLL handle);
	void (*close)(MwLL handle);
};

#ifdef __cplusplus
extern "C" {
#endif

/* lowlevel.c, common part */
MWDECL void MwLLCreateCommon(MwLL handle);
MWDECL void MwLLDestroyCommon(MwLL handle);

/* driver-specific */
MWDECL MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height);
MWDECL void MwLLDestroy(MwLL handle);

MWDECL void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color);

MWDECL MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b);
MWDECL void	 MwLLFreeColor(MwLLColor color);

MWDECL void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
MWDECL void MwLLSetXY(MwLL handle, int x, int y);
MWDECL void MwLLSetWH(MwLL handle, int w, int h);

MWDECL void MwLLSetTitle(MwLL handle, const char* title);

MWDECL int  MwLLPending(MwLL handle);
MWDECL void MwLLNextEvent(MwLL handle);
MWDECL void MwLLSleep(int ms);

#ifdef __cplusplus
}
#endif

#endif
