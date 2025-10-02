/* $Id$ */
/*!
 * %file Mw/Menu.h
 * %brief Menu widget
 */
#ifndef __MW_MENU_H__
#define __MW_MENU_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Menu widget class
 */
MWDECL MwClass MwMenuClass;

/*!
 * %brief Adds a menu
 * %param handle Widget
 * %param menu Menu
 * %param name Menu name
 * %return Menu
 */
MWDECL MwMenu MwMenuAdd(MwWidget handle, MwMenu menu, const char* name);

#ifdef __cplusplus
}
#endif

#endif
