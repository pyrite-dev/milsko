/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

#define MilskoDispatch(x, y) \
	if(x->class != NULL && x->class->y != NULL) x->class->y(x)

MILSKODECL MilskoWidget MilskoCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL void		MilskoDestroyWidget(MilskoWidget handle);

MILSKODECL void MilskoLoop(MilskoWidget handle);
MILSKODECL void MilskoStep(MilskoWidget handle);
MILSKODECL int	MilskoPending(MilskoWidget handle);

MILSKODECL void		 MilskoSetInteger(MilskoWidget handle, const char* key, int n);
MILSKODECL void		 MilskoSetText(MilskoWidget handle, const char* key, const char* value);
MILSKODECL void		 MilskoSetHandler(MilskoWidget handle, const char* key, MilskoHandler value);
MILSKODECL int		 MilskoGetInteger(MilskoWidget handle, const char* key);
MILSKODECL const char*	 MilskoGetText(MilskoWidget handle, const char* key);
MILSKODECL MilskoHandler MilskoGetHandler(MilskoWidget handle, const char* key);
MILSKODECL void		 MilskoDispatchHandler(MilskoWidget handle, const char* key);
MILSKODECL void		 MilskoSetDefault(MilskoWidget handle);
MILSKODECL void		 MilskoApply(MilskoWidget handle, ...);

#endif
