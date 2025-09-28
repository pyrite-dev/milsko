/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

MILSKODECL HMILSKO MilskoCreateWidget(MilskoClass class, HMILSKO parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL void	   MilskoDestroyWidget(HMILSKO handle);

MILSKODECL void MilskoLoop(HMILSKO handle);
MILSKODECL void MilskoStep(HMILSKO handle);
MILSKODECL int	MilskoPending(HMILSKO handle);

MILSKODECL void	       MilskoSetInteger(HMILSKO handle, const char* key, int n);
MILSKODECL void	       MilskoSetText(HMILSKO handle, const char* key, const char* value);
MILSKODECL int	       MilskoGetInteger(HMILSKO handle, const char* key);
MILSKODECL const char* MilskoGetText(HMILSKO handle, const char* key);
MILSKODECL void	       MilskoApply(HMILSKO handle, ...);

#endif
