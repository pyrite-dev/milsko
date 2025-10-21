/* $Id$ */
/*!
 * %file Mw/Widget/ListBox.h
 * %brief ListBox widget
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
MwInline void MwListBoxInsert(MwWidget handle, int index, MwLLPixmap pixmap, ...) {
	va_list va;
	va_start(va, pixmap);
	MwVaWidgetExecute(handle, "mwListBoxInsert", NULL, index, pixmap, &va);
	va_end(va);
}

/*!
 * %brief Inserts multiple items on the listbox
 * %param handle Widget
 * %param index Index
 * %param count Count
 * %param pixmap Pixmap
 * %param ... Text
 */
MwInline void MwListBoxInsertMultiple(MwWidget handle, int index, int count, MwLLPixmap* pixmap, ...) {
	va_list va;
	va_start(va, pixmap);
	MwVaWidgetExecute(handle, "mwListBoxInsertMultiple", NULL, index, count, pixmap, &va);
	va_end(va);
}

/*!
 * %brief Deletes item from the listbox
 * %param handle Widget
 * %param index Index
 */
MwInline void MwListBoxDelete(MwWidget handle, int index) {
	MwVaWidgetExecute(handle, "mwListboxDelete", NULL, index);
};

/*!
 * %brief Gets item from the listbox
 * %param handle Widget
 * %param index Index
 * %return Item
 */
MwInline const char* MwListBoxGet(MwWidget handle, int index) {
	const char* out;
	MwVaWidgetExecute(handle, "mwListBoxGet", (void*)&out, index);
	return out;
};

/*!
 * %brief Sets an item width of the listbox
 * %param handle Widget
 * %param index Column index
 * %param width Width
 */
MwInline void MwListBoxSetWidth(MwWidget handle, int index, int width) {
	MwVaWidgetExecute(handle, "mwListBoxSetWidth", NULL, index, width);
};

/*!
 * %brief Resets the listbox
 * %param handle Widget
 */
MwInline void MwListBoxReset(MwWidget handle) {
	MwVaWidgetExecute(handle, "mwListBoxReset", NULL, handle);
};

#ifdef __cplusplus
}
#endif

#endif
