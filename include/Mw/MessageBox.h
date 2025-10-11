/* $Id$ */
/*!
 * %file Mw/MessageBox.h
 * %brief Message box
 */
#ifndef __MW_MESSAGEBOX_H__
#define __MW_MESSAGEBOX_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %param handle Widget
 * %param text Text
 * %param title Title text
 * %param flag Flag
 * %return Widget
 */
MWDECL MwWidget MwMessageBox(MwWidget handle, const char* text, const char* title, unsigned int flag);

#ifdef __cplusplus
}
#endif

#endif
