/* $Id$ */
#ifndef __MILSKO_DRAW_H__
#define __MILSKO_DRAW_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>
#include <Milsko/LowLevel.h>

MILSKODECL MilskoLLColor MilskoParseColor(MilskoWidget handle, const char* text);

MILSKODECL void MilskoDrawRect(MilskoWidget handle, MilskoRect* rect, MilskoLLColor color);

#endif
