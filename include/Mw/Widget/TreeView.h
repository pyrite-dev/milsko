/* $Id$ */
/*!
 * @file Mw/Widget/TreeView.h
 * @brief TreeView widget
 */
#ifndef __MW_WIDGET_TREEVIEW_H__
#define __MW_WIDGET_TREEVIEW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/Core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief TreeView widget class
 */
MWDECL MwClass MwTreeViewClass;

/*!
 * @brief Adds item to the treeview
 * @param handle Widget
 * @parma parent Parent
 * @parma pixmap Pixmap
 * @param item Item
 */
MwInline void* MwTreeViewAdd(MwWidget handle, void* parent, MwLLPixmap pixmap, const char* item) {
	void* out;
	MwVaWidgetExecute(handle, "mwTreeViewAdd", &out, parent, pixmap, item);
	return out;
}

/*!
 * @brief Deletes item from the treeview
 * @param handle Widget
 * @param item Item
 */
MwInline void MwTreeViewDelete(MwWidget handle, void* item) {
	MwVaWidgetExecute(handle, "mwTreeViewDelete", NULL, item);
}

/*!
 * @brief Resets the treeview
 * @param handle Widget
 */
MwInline void MwTreeViewReset(MwWidget handle) {
	MwVaWidgetExecute(handle, "mwTreeViewReset", NULL);
}

/*!
 * @brief Gets item from the treeview
 * @param handle Widget
 * @param item Item
 * @return Item
 */
MwInline const char* MwTreeViewGet(MwWidget handle, void* item) {
	const char* out;
	MwVaWidgetExecute(handle, "mwTreeViewGet", (void*)&out, item);
	return out;
}

#ifdef __cplusplus
}
#endif

#endif
