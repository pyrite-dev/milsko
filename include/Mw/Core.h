/* $Id$ */
/*!
 * %file Mw/Core.h
 * %brief Core
 */
#ifndef __MW_CORE_H__
#define __MW_CORE_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

/*!
 * %warning Used internally
 * %brief Dispatches a handler of widget class
 * %param x Widget
 * %param y Handler name
 */
#define MwDispatch(x, y) \
	if(x->widget_class != NULL && x->widget_class->y != NULL) x->widget_class->y(x)

#define MwWaitMS 5

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Creates a widget
 * %param widget_class Widget class
 * %param name Widget name
 * %param parent Parent widget or `NULL`
 * %param x X
 * %param y Y
 * %param width Width
 * %param height Height
 */
MWDECL MwWidget MwCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height);
MWDECL MwWidget MwVaCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, ...);
MWDECL MwWidget MwVaListCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va);
MWDECL void	MwDestroyWidget(MwWidget handle);

MWDECL void MwLoop(MwWidget handle);
MWDECL void MwStep(MwWidget handle);
MWDECL int  MwPending(MwWidget handle);

MWDECL void	   MwSetInteger(MwWidget handle, const char* key, int n);
MWDECL void	   MwSetText(MwWidget handle, const char* key, const char* value);
MWDECL int	   MwGetInteger(MwWidget handle, const char* key);
MWDECL const char* MwGetText(MwWidget handle, const char* key);
MWDECL void	   MwSetDefault(MwWidget handle);
MWDECL void	   MwVaApply(MwWidget handle, ...);
MWDECL void	   MwVaListApply(MwWidget handle, va_list va);

MWDECL void MwAddUserHandler(MwWidget handle, const char* key, MwUserHandler handler, void* user_data);
MWDECL void MwDispatchUserHandler(MwWidget handle, const char* key, void* handler_data);

MWDECL void MwSetErrorHandler(MwErrorHandler handler, void* user_data);
MWDECL void MwDispatchError(int code, const char* message);

MWDECL jmp_buf MwGetBeforeStep(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
