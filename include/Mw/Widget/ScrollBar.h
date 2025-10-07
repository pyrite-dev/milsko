/* $Id$ */
/*!
 * %file Mw/Widget/ScrollBar.h
 * %brief ScrollBar widget
 * %prop MwNareaShown MwNvalue MwNminValue MwNmaxValue MwNorientation
 */
#ifndef __MW_WIDGET_SCROLLBAR_H__
#define __MW_WIDGET_SCROLLBAR_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief ScrollBar widget class
 */
MWDECL MwClass MwScrollBarClass;

/*!
 * %brief Calculates a visible length of scrollbar
 * %param handle Widget
 * %return Visible length
 */
MWDECL int MwScrollBarGetVisibleLength(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
