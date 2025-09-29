/* $Id$ */
#ifndef __MILSKO_DRAW_H__
#define __MILSKO_DRAW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

MILSKODECL MwLLColor MwParseColor(MwWidget handle, const char* text);

MILSKODECL void MwDrawRect(MwWidget handle, MwRect* rect, MwLLColor color);
MILSKODECL void MwDrawFrame(MwWidget handle, MwRect* rect, MwLLColor color, int invert);

#endif
