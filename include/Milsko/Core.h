/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>
#include <Milsko/LowLevel.h>

MILSKODECL MilskoWidget MilskoCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL void		MilskoDestroyWidget(MilskoWidget handle);

MILSKODECL void MilskoLoop(MilskoWidget handle);
MILSKODECL void MilskoStep(MilskoWidget handle);
MILSKODECL int	MilskoPending(MilskoWidget handle);

MILSKODECL MilskoLLColor MilskoParseColor(MilskoWidget handle, const char* text);

MILSKODECL void	       MilskoSetInteger(MilskoWidget handle, const char* key, int n);
MILSKODECL void	       MilskoSetText(MilskoWidget handle, const char* key, const char* value);
MILSKODECL int	       MilskoGetInteger(MilskoWidget handle, const char* key);
MILSKODECL const char* MilskoGetText(MilskoWidget handle, const char* key);
MILSKODECL void	       MilskoApply(MilskoWidget handle, ...);

#endif
