/* $Id: ComboBox.h 206 2025-10-07 15:10:46Z nishi $ */
/*!
 * @file Mw/Widget/ComboBox.h
 * @brief ComboBox widget
 */
#ifndef __MW_WIDGET_COMBOBOX_H__
#define __MW_WIDGET_COMBOBOX_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief ComboBox widget class
 */
MWDECL MwClass MwComboBoxClass;

/*!
 * @brief Adds the entry to ComboBox
 * @param handle Widget
 * @param index Index
 * @param text Text
 */
MwInline void MwComboBoxAdd(MwWidget handle, int index, const char* text) {
	MwVaWidgetExecute(handle, "mwComboBoxAdd", NULL, index, text);
}

#ifdef __cplusplus
}
#endif

#endif
