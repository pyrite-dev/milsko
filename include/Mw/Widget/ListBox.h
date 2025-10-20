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
#include <Mw/Core.h>

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
void MwListBoxInsert(MwWidget handle, int index, MwLLPixmap pixmap, ...); /* VA_HINT:pixmap */

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param count Count
 * %param pixmap Pixmap
 * %param ... Text
 */
void MwListBoxInsertMultiple(MwWidget handle, int index, int count, MwLLPixmap* pixmap, ...); /* VA_HINT:pixmap */

/*!
 * %brief Inserts item on the listbox
 * %param handle Widget
 * %param index Index
 * %param pixmap Pixmap
 * %param va Text
 */
void MwListBoxVaInsert(MwWidget handle, int index, MwLLPixmap pixmap, va_list va);

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param count Count
 * %param pixmap Pixmap
 * %param va Text
 */
void MwListBoxVaInsertMultiple(MwWidget handle, int index, int count, MwLLPixmap* pixmap, va_list va);

/*!
 * %brief Deletes item from the listbox
 * %param handle Widget
 * %param index Index
 */
MwInline void MwListBoxDelete(MwWidget handle, int index) {
	MwWidgetExecute(handle, "mwListboxDelete", NULL, index);
};

/*!
 * %brief Gets item from the listbox
 * %param handle Widget
 * %param index Index
 * %return Item
 */
MwInline const char* MwListBoxGet(MwWidget handle, int index) {
	const char* out;
	MwWidgetExecute(handle, "mwListBoxGet", &out, index);
	return out;
};

/*!
 * %brief Sets an item width of the listbox
 * %param handle Widget
 * %param index Column index
 * %param width Width
 */
MwInline void MwListBoxSetWidth(MwWidget handle, int index, int width) {
	MwWidgetExecute(handle, "mwListBoxSetWidth", NULL, index, width);
};

/*!
 * %brief Resets the listbox
 * %param handle Widget
 */
MwInline void MwListBoxReset(MwWidget handle) {
	MwWidgetExecute(handle, "mwListBoxReset", NULL, handle);
};

#ifdef __cplusplus
}
#endif

#endif
