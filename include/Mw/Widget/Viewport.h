/* $Id$ */
/*!
 * %file Mw/Widget/Viewport.h
 * %brief Viewport widget
 */
#ifndef __MW_WIDGET_VIEWPORT_H__
#define __MW_WIDGET_VIEWPORT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Viewport widget class
 */
MWDECL MwClass MwViewportClass;

MWDECL MwWidget MwViewportGetViewport(MwWidget widget);

#ifdef __cplusplus
}
#endif

#endif
