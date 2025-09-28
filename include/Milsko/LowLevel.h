/* $Id$ */
#ifndef __MILSKO_PLATFORM_H__
#define __MILSKO_PLATFORM_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <Milsko/TypeDef.h>

#ifdef _MILSKO
#ifdef USE_X11
#include <Milsko/X11.h>
#endif
#ifdef USE_GDI
#include <Milsko/GDI.h>
#endif
#else
typedef void* HMILSKO;
typedef void* HMILSKOCOLOR;
#endif

HMILSKO MilskoLLCreate(HMILSKO parent, int x, int y, int width, int height);
void MilskoLLPolygon(HMILSKO handle, MilskoPoint* points, int points_count, HMILSKOCOLOR color);
HMILSKOCOLOR MilskoLLAllocColor(HMILSKO handle, int r, int g, int b);

#endif
