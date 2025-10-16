/* $Id$ */
/*!
 * %file Mw/Widget/ListBox.h
 * %brief ListBox widget
 * %prop MwNleftPadding
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
 * %param pixmap Pixmap
 * %param ... Text
 */
MWDECL void MwListBoxInsert(MwWidget handle, int index, MwLLPixmap pixmap, ...); /* VA_HINT:pixmap */

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param count Count
 * %param pixmap Pixmap
 * %param ... Text
 */
MWDECL void MwListBoxInsertMultiple(MwWidget handle, int index, int count, MwLLPixmap* pixmap, ...); /* VA_HINT:pixmap */

/*!
 * %brief Inserts item on the listbox
 * %param handle Widget
 * %param index Index
 * %param pixmap Pixmap
 * %param va Text
 */
MWDECL void MwListBoxVaInsert(MwWidget handle, int index, MwLLPixmap pixmap, va_list va);

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param count Count
 * %param pixmap Pixmap
 * %param va Text
 */
MWDECL void MwListBoxVaInsertMultiple(MwWidget handle, int index, int count, MwLLPixmap* pixmap, va_list va);

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

/*!
 * %brief Sets an item width of the listbox
 * %param handle Widget
 * %param index Column index
 * %param width Width
 */
MWDECL void MwListBoxSetWidth(MwWidget handle, int index, int width);

/*!
 * %brief Resets the listbox
 * %param handle Widget
 */
MWDECL void MwListBoxReset(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
