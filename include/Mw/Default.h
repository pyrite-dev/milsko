/* $Id$ */
/*!
 * %file Mw/Default.h
 * %brief Default properties
 */
#ifndef __MW_DEFAULT_H__
#define __MW_DEFAULT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Default background color
 */
MWDECL const char* MwDefaultBackground;

/*!
 * %brief Default foreground color
 */
MWDECL const char* MwDefaultForeground;

/*!
 * %brief Default dark theme background color
 */
MWDECL const char* MwDefaultDarkBackground;

/*!
 * %brief Default dark theme foreground color
 */
MWDECL const char* MwDefaultDarkForeground;

/*!
 * %brief Gets default border width
 * %param handle Widget
 */
MWDECL int MwGetDefaultBorderWidth(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
