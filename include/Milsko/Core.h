/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Milsko/MachDep.h>
#include <Milsko/TypeDefs.h>

#define MilskoDispatch(x, y) \
	if(x->class != NULL && x->class->y != NULL) x->class->y(x)

MILSKODECL MilskoWidget MilskoCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL MilskoWidget MilskoVaCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height, ...);
MILSKODECL MilskoWidget MilskoVaListCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va);
MILSKODECL void		MilskoDestroyWidget(MilskoWidget handle);

MILSKODECL void MilskoLoop(MilskoWidget handle);
MILSKODECL void MilskoStep(MilskoWidget handle);
MILSKODECL int	MilskoPending(MilskoWidget handle);

MILSKODECL void	       MilskoSetInteger(MilskoWidget handle, const char* key, int n);
MILSKODECL void	       MilskoSetText(MilskoWidget handle, const char* key, const char* value);
MILSKODECL int	       MilskoGetInteger(MilskoWidget handle, const char* key);
MILSKODECL const char* MilskoGetText(MilskoWidget handle, const char* key);
MILSKODECL void	       MilskoSetDefault(MilskoWidget handle);
MILSKODECL void	       MilskoVaApply(MilskoWidget handle, ...);
MILSKODECL void	       MilskoVaListApply(MilskoWidget handle, va_list va);

MILSKODECL void MilskoAddUserHandler(MilskoWidget handle, const char* key, MilskoUserHandler handler, void* user_data);
MILSKODECL void MilskoDispatchUserHandler(MilskoWidget handle, const char* key, void* handler_data);

#endif
