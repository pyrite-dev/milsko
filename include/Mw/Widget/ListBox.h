/*!
 * @file Mw/Widget/ListBox.h
 * @brief ListBox widget
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
 * @brief ListBox widget class
 */
MWDECL MwClass MwListBoxClass;

/*!
 * @brief Creates a listbox packet
 * @return Packet
 */
MWDECL MwListBoxPacket* MwListBoxCreatePacket(void);

/*!
 * @brief Destroys a listbox packet
 * @param packet Packet
 */
MWDECL void MwListBoxDestroyPacket(MwListBoxPacket* packet);

/*!
 * @brief Inserts a new item to a packet
 * @param packet Packet
 * @param index Index
 * @return Index
 */
MWDECL int MwListBoxPacketInsert(MwListBoxPacket* packet, int index);

/*!
 * @brief Sets a column of item in a packet
 * @param packet Packet
 * @param index Index
 * @param col Column
 * @param text Text
 */
MWDECL void MwListBoxPacketSet(MwListBoxPacket* packet, int index, int col, const char* text);

/*!
 * @brief Sets an icon of item in a packet
 * @param packet Packet
 * @param index Index
 * @param icon Icon
 */
MWDECL void MwListBoxPacketSetIcon(MwListBoxPacket* packet, int index, MwLLPixmap icon);

/*!
 * @brief Inserts item on the listbox
 * @param handle Widget
 * @param index Index
 * @param packet Packet
 */
MwInline void MwListBoxInsert(MwWidget handle, int index, void* packet) {
	MwVaWidgetExecute(handle, "mwListBoxInsert", NULL, index, packet);
}

/*!
 * @brief Deletes item from the listbox
 * @param handle Widget
 * @param index Index
 */
MwInline void MwListBoxDelete(MwWidget handle, int index) {
	MwVaWidgetExecute(handle, "mwListBoxDelete", NULL, index);
}

/*!
 * @brief Gets item from the listbox
 * @param handle Widget
 * @param index Index
 * @return Item
 */
MwInline const char* MwListBoxGet(MwWidget handle, int index) {
	const char* out;
	MwVaWidgetExecute(handle, "mwListBoxGet", (void*)&out, index);
	return out;
}

/*!
 * @brief Sets an item width of the listbox
 * @param handle Widget
 * @param index Column index
 * @param width Width
 */
MwInline void MwListBoxSetWidth(MwWidget handle, int index, int width) {
	MwVaWidgetExecute(handle, "mwListBoxSetWidth", NULL, index, width);
}

/*!
 * @brief Sets an item alignment of the listbox
 * @param handle Widget
 * @param index Column index
 * @param alignment Alignment
 */
MwInline void MwListBoxSetAlignment(MwWidget handle, int index, int alignment) {
	MwVaWidgetExecute(handle, "mwListBoxSetAlignment", NULL, index, alignment);
}

/*!
 * @brief Resets the listbox
 * @param handle Widget
 */
MwInline void MwListBoxReset(MwWidget handle) {
	MwVaWidgetExecute(handle, "mwListBoxReset", NULL);
}

#ifdef __cplusplus
}
#endif

#endif
