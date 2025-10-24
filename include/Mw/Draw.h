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
 * %brief Lighten a color
 * %param handle Widget
 * %param color Color
 * %param r Red
 * %param g Green
 * %param b Blue
 * %return Color
 */
MWDECL MwLLColor MwLightenColor(MwWidget handle, MwLLColor color, int r, int g, int b);

/*!
 * %brief Draws a filled rectangle
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 */
MWDECL void MwDrawRect(MwWidget handle, MwRect* rect, MwLLColor color);

/*!
 * %brief Draws a filled rectangle that fades to a darker color
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 */
MWDECL void MwDrawRectFading(MwWidget handle, MwRect* rect, MwLLColor color);

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
 * %brief Does the DrawFrame/DrawRect combo used for drawing widget.
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 * %param invert Invert the 3D border color or not
 * %warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MwDrawWidgetBack(MwWidget handle, MwRect* rect, MwLLColor color, int invert, int border);

/*!
 * %brief Draws a triangle
 * %param handle Widget
 * %param rect Rectangle area
 * %param color Color
 * %param invert Invert the 3D border color or not
 */
MWDECL void MwDrawTriangle(MwWidget handle, MwRect* rect, MwLLColor color, int invert, int direction);

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
 * %param align Align
 * %param color Color
 */
MWDECL void MwDrawText(MwWidget handle, MwPoint* point, const char* text, int bold, int align, MwLLColor color);

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

/*!
 * %brief Get color components
 * %param color Color
 * %param red Pointer to red color
 * %param green Pointer to green color
 * %param blue Pointer to blue color
 */
MWDECL void MwGetColor(MwLLColor color, int* red, int* green, int* blue);

/*!
 * %brief Creates a pixmap from raw data
 * %param handle Widget
 * %param rgb RGBA data
 * %param width Width
 * %param height Height
 * %return Pixmap
 */
MWDECL MwLLPixmap MwLoadRaw(MwWidget handle, unsigned char* rgb, int width, int height);

/*!
 * %brief Creates a pixmap from XPM data
 * %param handle Widget
 * %param data Data
 * %return Pixmap
 */
MWDECL MwLLPixmap MwLoadXPM(MwWidget handle, char** data);

/*!
 * %brief Creates a pixmap from icon data
 * %param handle Widget
 * %param data Data
 * %return Pixmap
 */
MWDECL MwLLPixmap MwLoadIcon(MwWidget handle, unsigned int* data);

/* color.c */

/*!
 * %brief Parses a color name
 * %param handle Widget
 * %param color Color name
 * %return Color
 */
MWDECL MwLLColor MwParseColorName(MwWidget handle, const char* color);

#ifdef __cplusplus
}
#endif

#endif
