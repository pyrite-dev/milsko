/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

MILSKODECL HMILSKO MilskoCreateWidget(MilskoClass class, HMILSKO parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL void	   MilskoDestroyWidget(HMILSKO handle);
MILSKODECL void	   MilskoLoop(HMILSKO handle);
MILSKODECL void	   MilskoStep(HMILSKO handle);
MILSKODECL int	   MilskoPending(HMILSKO handle);
MILSKODECL void	   MilskoApply(HMILSKO handle, ...);

#endif
