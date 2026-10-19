/*!
 * @file Mw/Widget/Viewport.h
 * @brief Viewport widget
 */
#ifndef __MW_WIDGET_VIEWPORT_H__
#define __MW_WIDGET_VIEWPORT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/Core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Viewport widget class
 */
MWDECL MwClass MwViewportClass;

/*!
 * @brief Get parent widget where widgets should be placed
 * @param handle Widget
 * @return Widget
 */
MwInline MwWidget MwViewportGetViewport(MwWidget handle) {
	MwWidget out;
	MwVaWidgetExecute(handle, "mwViewportGetViewport", &out);
	return out;
}

/*!
 * @brief Set viewport size
 * @param handle Widget
 * @param w Width
 * @param h Height
 */
MwInline void MwViewportSetSize(MwWidget handle, int w, int h) {
	MwVaWidgetExecute(handle, "mwViewportSetSize", NULL, w, h);
}

/*!
 * @brief Scroll viewport to specified position
 * @param handle Widget
 * @param x X
 * @param y Y
 */
MwInline void MwViewportScrollTo(MwWidget handle, int x, int y) {
	MwVaWidgetExecute(handle, "mwViewportScrollTo", NULL, x, y);
}

/*!
 * @brief Get size of the frame (visible area)
 * @param handle Widget
 * @param frame Frame
 */
MwInline void MwViewportGetFrame(MwWidget handle, MwRect* frame) {
	MwVaWidgetExecute(handle, "mwViewportGetFrame", NULL, frame);
};

#ifdef __cplusplus
}
#endif

#endif
