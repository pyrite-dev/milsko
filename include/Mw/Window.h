/* $Id$ */
/*!
 * %file Mw/Window.h
 * %brief Window widget
 */
#ifndef __MW_WINDOW_H__
#define __MW_WINDOW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Window widget class
 */
MWDECL MwClass MwWindowClass;

/*!
 * %brief Sets a window icon
 * %param handle Widget
 * %param pixmap Pixmap
 */
MWDECL void MwWindowSetIcon(MwWidget handle, MwLLPixmap pixmap);

#ifdef __cplusplus
}
#endif

#endif
