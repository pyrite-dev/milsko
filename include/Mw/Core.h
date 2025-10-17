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
 * %brief Dispatches the handler of widget class
 * %param x Widget
 * %param y Handler name
 */
#define MwDispatch(x, y) \
	if(!x->destroyed && x->widget_class != NULL && x->widget_class->y != NULL) x->widget_class->y(x)

/*!
 * %warning Used internally
 * %brief Dispatches the handler of widget class
 * %param x Widget
 * %param y Handler name
 * %return `0` for success, otherwise failed
 */
#define MwDispatch2(x, y) \
	((!x->destroyed && x->widget_class != NULL && x->widget_class->y != NULL) ? x->widget_class->y(x) : 0)

/*!
 * %warning Used internally
 * %brief Dispatches the handler of widget class
 * %param x Widget
 * %param y Handler name
 * %param z Argument
 */
#define MwDispatch3(x, y, z) \
	if(!x->destroyed && x->widget_class != NULL && x->widget_class->y != NULL) x->widget_class->y(x, z)

#define MwWaitMS 10

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
 * %return Widget
 */
MWDECL MwWidget MwCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height);

/*!
 * %brief Creates a widget
 * %param widget_class Widget class
 * %param name Widget name
 * %param parent Parent widget or `NULL`
 * %param x X
 * %param y Y
 * %param width Width
 * %param height Height
 * %param ... Same with MwVaApply
 * %return Widget
 */
MWDECL MwWidget MwVaCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, ...);

/*!
 * %brief Creates a widget
 * %param widget_class Widget class
 * %param name Widget name
 * %param parent Parent widget or `NULL`
 * %param x X
 * %param y Y
 * %param width Width
 * %param height Height
 * %param va Same with MwVaListApply
 * %return Widget
 */
MWDECL MwWidget MwVaListCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va);

/*!
 * %brief Destroys the widget and its child widgets
 * %param handle Widget
 */
MWDECL void MwDestroyWidget(MwWidget handle);

/*!
 * %brief Runs the main loop
 * %param handle Widget
 */
MWDECL void MwLoop(MwWidget handle);

/*!
 * %brief Runs the single step
 * %param handle Widget
 */
MWDECL void MwStep(MwWidget handle);

/*!
 * %brief Check if any event is pending
 * %param handle Widget
 * %return `1` if any event is pending
 */
MWDECL int MwPending(MwWidget handle);

/*!
 * %brief Sets an integer property
 * %param handle Widget
 * %param key Key
 * %param n Value
 */
MWDECL void MwSetInteger(MwWidget handle, const char* key, int n);

/*!
 * %brief Sets a text property
 * %param handle Widget
 * %param key Key
 * %param value Value
 */
MWDECL void MwSetText(MwWidget handle, const char* key, const char* value);

/*!
 * %brief Sets a void pointer property
 * %param handle Widget
 * %param key Key
 * %param value Value
 */
MWDECL void MwSetVoid(MwWidget handle, const char* key, void* value);

/*!
 * %brief Gets the integer property
 * %param handle Widget
 * %param key Key
 * %return Value
 */
MWDECL int MwGetInteger(MwWidget handle, const char* key);

/*!
 * %brief Gets the text property
 * %param handle Widget
 * %param key Key
 * %return Value
 */
MWDECL const char* MwGetText(MwWidget handle, const char* key);

/*!
 * %brief Gets the void pointer property
 * %param handle Widget
 * %param key Key
 * %return Value
 */
MWDECL void* MwGetVoid(MwWidget handle, const char* key);

/*!
 * %brief Sets the default property
 * %param handle Widget
 * %warning This is called when widget is created
 */
MWDECL void MwSetDefault(MwWidget handle);

/*!
 * %brief Sets the properties
 * %param handle Widget
 * %param ... Properties
 */
MWDECL void MwVaApply(MwWidget handle, ...);

/*!
 * %brief Sets properties
 * %param handle Widget
 * %param va Properties
 */
MWDECL void MwVaListApply(MwWidget handle, va_list va);

/*!
 * %brief Sets a user handler
 * %param handle Widget
 * %param key Key
 * %param handler Handler
 * %param user_data User data passed to handler
 */
MWDECL void MwAddUserHandler(MwWidget handle, const char* key, MwUserHandler handler, void* user_data);

/*!
 * %brief Dispatches the user handler
 * %param handle Widget
 * %param key Key
 * %param handler_data Handler data passed to handler
 */
MWDECL void MwDispatchUserHandler(MwWidget handle, const char* key, void* handler_data);

/*!
 * %brief Sets an error handler
 * %param handle Widget
 * %param handler Handler
 * %param user_data User data passed to handler
 */
MWDECL void MwSetErrorHandler(MwErrorHandler handler, void* user_data);

/*!
 * %brief Dispatches the error handler
 * %param code Error code
 * %param message Error message
 */
MWDECL void MwDispatchError(int code, const char* message);

/*!
 * %brief Gets the before_step of widget
 * %param handle Widget
 * %param jmpbuf jmp_buf
 */
MWDECL void MwGetBeforeStep(MwWidget handle, jmp_buf* jmpbuf);

/*!
 * %brief Forcefully makes widget render
 * %param handle Widget
 */
MWDECL void MwForceRender(MwWidget handle);

/*!
 * %brief Forcefully makes widget render
 * %param handle Widget
 * %param ptr Ignored
 */
MWDECL void MwForceRender2(MwWidget handle, void* ptr);

/*!
 * %brief Adds an widget to tick handler list
 * %param handle Widget
 */
MWDECL void MwAddTickList(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
