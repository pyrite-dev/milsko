/*!
 * @file Mw/Widget/SubMenu.h
 * @brief SubMenu widget
 */
#ifndef __MW_WIDGET_SUBMENU_H__
#define __MW_WIDGET_SUBMENU_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/Core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief SubMenu widget class
 */
MWDECL MwClass MwSubMenuClass;

MwInline void MwSubMenuAppear(MwWidget handle, MwMenu menu, MwPoint* point) {
	MwVaWidgetExecute(handle, "mwSubMenuAppear", NULL, menu, point);
}

#ifdef __cplusplus
}
#endif

#endif
