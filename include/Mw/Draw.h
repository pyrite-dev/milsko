/* $Id$ */
/*!
 * %file Mw/Draw.h
 * %brief Common drawing API
 */
#ifndef __MW_DRAW_H__
#define __MW_DRAW_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Parses a color text
 * %param handle Widget
 * %param text Color text
 * %return Color
 */
MWDECL MwLLColor MwParseColor(MwWidget handle, const char* text);

/*!
 * %brief Draws a filled rectangle
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 */
MWDECL void MwDrawRect(MwWidget handle, MwRect* rect, MwLLColor color);

/*!
 * %brief Draws a frame
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 * %param invert Invert the 3D border color or not
 * %warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MwDrawFrame(MwWidget handle, MwRect* rect, MwLLColor color, int invert);

/*!
 * %brief Draws a frame with specified border width
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 * %param invert Invert the 3D border color or not
 * %param border Border width
 * %warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MwDrawFrameEx(MwWidget handle, MwRect* rect, MwLLColor color, int invert, int border);

/*!
 * %brief Draws a text
 * %param handle Widget
 * %param point Center point of the text
 * %param text Text
 * %param bold Bold
 * %param color Color
 */
MWDECL void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, MwLLColor color);

/*!
 * %brief Creates a pixmap from image
 * %param handle Widget
 * %param path Path
 * %return Pixmap
 */
MWDECL MwLLPixmap MwLoadImage(MwWidget handle, const char* path);

/*!
 * %brief Calculates a text width
 * %param handle Widget
 * %param text Text
 * %return Text width
 */
MWDECL int MwTextWidth(MwWidget handle, const char* text);

/*!
 * %brief Calculates a text height
 * %param handle Widget
 * %param text Text
 * %return Text height
 */
MWDECL int MwTextHeight(MwWidget handle, const char* text);

#ifdef __cplusplus
}
#endif

#endif
