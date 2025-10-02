/* $Id$ */
/*!
 * %file Mw/SubMenu.h
 * %brief SubMenu widget
 */
#ifndef __MW_SUBMENU_H__
#define __MW_SUBMENU_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief SubMenu widget class
 */
MWDECL MwClass MwSubMenuClass;

MWDECL void MwSubMenuAppear(MwWidget handle, MwMenu menu, MwPoint* point);

#ifdef __cplusplus
}
#endif

#endif
