/*!
 * @file Mw/Widget/Chart.h
 * @brief Chart widget
 */
#ifndef __MW_WIDGET_CHART_H__
#define __MW_WIDGET_CHART_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Chart widget class
 */
MWDECL MwClass MwChartClass;

/*!
 * @brief Adds the entry to the chart
 * @param handle Widget
 * @param index Index
 * @param text Text
 * @param value Value
 * @return Index
 */
MwInline int MwChartAdd(MwWidget handle, int index, const char* text, double value, const char* color) {
	int out;

	MwVaWidgetExecute(handle, "mwChartAdd", (void*)&out, index, text, value, color);

	return out;
}

/*!
 * @brief Deletes item from the chart
 * @param handle Widget
 * @param index Index
 */
MwInline void MwChartDelete(MwWidget handle, int index) {
	MwVaWidgetExecute(handle, "mwChartDelete", NULL, index);
}

/*!
 * @brief Resets the chart
 * @param handle Widget
 */
MwInline void MwChartReset(MwWidget handle) {
	MwVaWidgetExecute(handle, "mwChartReset", NULL);
}

#ifdef __cplusplus
}
#endif

#endif
