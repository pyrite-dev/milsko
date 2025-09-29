/* $Id$ */
#ifndef __MW_DRAW_H__
#define __MW_DRAW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

MWDECL MwLLColor MwParseColor(MwWidget handle, const char* text);

MWDECL void MwDrawRect(MwWidget handle, MwRect* rect, MwLLColor color);
MWDECL void MwDrawFrame(MwWidget handle, MwRect* rect, MwLLColor color, int invert);

#endif
