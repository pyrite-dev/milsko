/* $Id$ */
/*!
 * %file Mw/Widget/ListBox.h
 * %brief ListBox widget
 */
#ifndef __MW_WIDGET_LISTBOX_H__
#define __MW_WIDGET_LISTBOX_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief ListBox widget class
 */
MWDECL MwClass MwListBoxClass;

/*!
 * %brief Inserts item on the listbox
 * %param handle Widget
 * %param index Index
 * %param text Text
 */
MWDECL void MwListBoxInsert(MwWidget handle, int index, const char* text);

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param text Text
 * %param count Count
 */
MWDECL void MwListBoxInsertMultiple(MwWidget handle, int index, char** text, int count);

/*!
 * %brief Deletes item from the listbox
 * %param handle Widget
 * %param index Index
 */
MWDECL void MwListBoxDelete(MwWidget handle, int index);

/*!
 * %brief Gets item from the listbox
 * %param handle Widget
 * %param index Index
 * %return Item
 */
MWDECL const char* MwListBoxGet(MwWidget handle, int index);

#ifdef __cplusplus
}
#endif

#endif
