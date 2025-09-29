/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#define MwDispatch(x, y) \
	if(x->class != NULL && x->class->y != NULL) x->class->y(x)

MILSKODECL MwWidget MwCreateWidget(MwClass class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height);
MILSKODECL MwWidget MwVaCreateWidget(MwClass class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, ...);
MILSKODECL MwWidget MwVaListCreateWidget(MwClass class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va);
MILSKODECL void		MwDestroyWidget(MwWidget handle);

MILSKODECL void MwLoop(MwWidget handle);
MILSKODECL void MwStep(MwWidget handle);
MILSKODECL int	MwPending(MwWidget handle);

MILSKODECL void	       MwSetInteger(MwWidget handle, const char* key, int n);
MILSKODECL void	       MwSetText(MwWidget handle, const char* key, const char* value);
MILSKODECL int	       MwGetInteger(MwWidget handle, const char* key);
MILSKODECL const char* MwGetText(MwWidget handle, const char* key);
MILSKODECL void	       MwSetDefault(MwWidget handle);
MILSKODECL void	       MwVaApply(MwWidget handle, ...);
MILSKODECL void	       MwVaListApply(MwWidget handle, va_list va);

MILSKODECL void MwAddUserHandler(MwWidget handle, const char* key, MwUserHandler handler, void* user_data);
MILSKODECL void MwDispatchUserHandler(MwWidget handle, const char* key, void* handler_data);

#endif
