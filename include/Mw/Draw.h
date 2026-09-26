/*!
 * @file Mw/Draw.h
 * @brief Common drawing API
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
 * @brief Get a color difference for shadow
 * @param handle Widget
 * @return Color difference
 */
MWDECL int MWAPI MwGetColorDifference(MwWidget handle);

/*!
 * @brief Parses a color text
 * @param handle Widget
 * @param text Color text
 * @return Color
 */
MWDECL MwColor MWAPI MwParseColor(MwWidget handle, const char* text);

/*!
 * @brief Parses a color text
 * @param text Color text
 * @param rgb RGB
 */
MWDECL void MWAPI MwParseColorNoAllocate(const char* text, MwRGB* rgb);

/*!
 * @brief Initialize color table
 * @warning This is called in MwLibraryInit - you do not have to call this
 */
MWDECL void MWAPI MwColorTableInit(void);

/*!
 * @brief Lighten a color
 * @param handle Widget
 * @param color Color
 * @param r Red
 * @param g Green
 * @param b Blue
 * @return Color
 */
MWDECL MwColor MWAPI MwLightenColor(MwWidget handle, MwColor color, int r, int g, int b);

/*!
 * @brief Fix a rectangle
 * @param handle Widget
 * @param rect Rectangle area
 */
MWDECL void MWAPI MwFixRect(MwRect* rect);

/*!
 * @brief Draws a filled rectangle
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 */
MWDECL void MWAPI MwDrawRect(MwWidget handle, MwRect* rect, MwColor color);

/*!
 * @brief Draws a rectangle
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 */
MWDECL void MWAPI MwDrawRectLine(MwWidget handle, MwRect* rect, MwColor color);

/*!
 * @brief Draws a filled rectangle that fades to a darker color
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 */
MWDECL void MWAPI MwDrawRectFading(MwWidget handle, MwRect* rect, MwColor color);

/*!
 * @brief Draws a frame
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 * @warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MWAPI MwDrawFrame(MwWidget handle, MwRect* rect, MwColor color, int invert);

/*!
 * @brief Draws a frame
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 * @param border Border
 * @warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MWAPI MwDrawFrameWithBorder(MwWidget handle, MwRect* rect, MwColor color, int invert, int border);

/*!
 * @brief Does the DrawFrame/DrawRect combo used for drawing widget.
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 * @warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MWAPI MwDrawWidgetBack(MwWidget handle, MwRect* rect, MwColor color, int invert, int border);

/*!
 * @brief Draws a triangle
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 */
MWDECL void MWAPI MwDrawTriangle(MwWidget handle, MwRect* rect, MwColor color, int invert, int direction);

/*!
 * @brief Draws a frame with specified border width
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 * @param border Border width
 * @param diff Difference (set this to 0 if you don't know what this does)
 * @param same Same as dark color
 * @warning `rect` gets changed to the area of rectangle inside
 */
MWDECL void MWAPI MwDrawFrameEx(MwWidget handle, MwRect* rect, MwColor color, int invert, int border, int diff, int same);

/*!
 * @brief Creates a pixmap from image
 * @param handle Widget
 * @param path Path
 * @return Pixmap
 */
MWDECL MwPixmap MWAPI MwLoadImage(MwWidget handle, const char* path);

/*!
 * @brief Creates a pixmap from raw data
 * @param handle Widget
 * @param rgb RGBA data
 * @param width Width
 * @param height Height
 * @return Pixmap
 */
MWDECL MwPixmap MWAPI MwLoadRaw(MwWidget handle, unsigned char* rgb, int width, int height);

/*!
 * @brief Updates a pixmap using raw data
 * @param pixmap Pixmap to update
 * @param rgb RGBA data
 */
MWDECL void MWAPI MwPixmapReloadRaw(MwPixmap pixmap, unsigned char* rgb);

/*!
 * @brief Gets the raw data of pixmap
 * @param pixmap Pixmap
 * @return RGBA data
 */
MWDECL unsigned char* MWAPI MwPixmapGetRaw(MwPixmap pixmap);

/*!
 * @brief Gets the size of pixmap
 * @param pixmap Pixmap
 * @param rect Size
 */
MWDECL void MWAPI MwPixmapGetSize(MwPixmap pixmap, MwRect* rect);

/*!
 * @brief Updates the pixmap
 * @param pixmap Pixmap
 */
MWDECL void MWAPI MwPixmapUpdate(MwPixmap pixmap);

/*!
 * @brief Destroys the pixmap
 * @param pixmap Pixmap
 */
MWDECL void MWAPI MwDestroyPixmap(MwPixmap pixmap);

/*!
 * @brief Draws the pixmap
 * @param handle Handle
 * @param rect Size
 * @param pixmap Pixmap
 */
MWDECL void MWAPI MwDrawPixmap(MwWidget handle, MwRect* rect, MwPixmap pixmap);

/*!
 * @brief Allocates a color
 * @param handle Widget
 * @param red Red
 * @param green Green
 * @param bilue Blue
 * @return Color
 */
MWDECL MwColor MWAPI MwAllocColor(MwWidget handle, unsigned int red, unsigned int green, unsigned int blue);

/*!
 * @brief Get color components
 * @param color Color
 * @param red Pointer to red color
 * @param green Pointer to green color
 * @param blue Pointer to blue color
 */
MWDECL void MWAPI MwColorGet(MwColor color, int* red, int* green, int* blue);

/*!
 * @brief Update color
 * @param color Color
 * @param red Pointer to red color
 * @param green Pointer to green color
 * @param blue Pointer to blue color
 */
MWDECL void MWAPI MwColorUpdate(MwColor c, int r, int g, int b);

/*!
 * @brief Allocates a color
 * @param color Color
 */
MWDECL void MWAPI MwFreeColor(MwColor color);

/*!
 * @brief Creates a pixmap from XPM data
 * @param handle Widget
 * @param data Data
 * @return Pixmap
 */
MWDECL MwPixmap MWAPI MwLoadXPM(MwWidget handle, char** data);

/*!
 * @brief Creates a pixmap from icon data
 * @param handle Widget
 * @param data Data
 * @return Pixmap
 */
MWDECL MwPixmap MWAPI MwLoadIcon(MwWidget handle, MwU32* data);

/*!
 * @brief Draws a diamond
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param invert Invert the 3D border color or not
 */
MWDECL void MWAPI MwDrawDiamond(MwWidget handle, MwRect* rect, MwColor color, int invert);

/*!
 * @brief Draws a circle
 * @param handle Widget
 * @param rect Rectangle area
 * @param color Color
 * @param color Background to fade to; widget background used if NULL
 * @param filled Fill the circle or not
 * @param outward
 */
MWDECL void MWAPI MwDrawCircle(MwWidget handle, MwRect* rect, MwColor color, MwColor background, int filled);

/*!
 * @brief Calculate intersect of 2 rectangles
 * @param a 1st rectangle
 * @param a 2nd rectangle
 */
MWDECL void MwIntersectRect(MwRect* a, const MwRect* b);

/* text.c */

/*!
 * @brief Draws a text
 * @param handle Widget
 * @param ttf TTF, NULLable
 * @param point Center point of the text
 * @param text Text
 * @param align Align
 * @param color Color
 */
MWDECL void MWAPI MwDrawText(MwWidget handle, MwFLFont ttf, MwPoint* point, const char* text, int align, MwColor color);

/*!
 * @brief Calculates a text width
 * @param handle Widget
 * @param text Text
 * @param font Font, NULLable
 * @return Text width
 */
MWDECL int MWAPI MwTextWidth(MwWidget handle, MwFLFont font, const char* text);

/*!
 * @brief Calculates a text height
 * @param handle Widget
 * @param text Text
 * @param font Font, NULLable
 * @return Text height
 */
MWDECL int MWAPI MwTextHeight(MwWidget handle, MwFLFont font, const char* text);

/* color.c */

/*!
 * @brief Parses a color name
 * @param handle Widget
 * @param color Color name
 * @return Color
 */
MWDECL MwColor MWAPI MwParseColorName(MwWidget handle, const char* color);

/*!
 * @brief Parses a color name
 * @param color Color name
 * @param rgb RGB
 */
MWDECL void MWAPI MwParseColorNameNoAllocate(const char* color, MwRGB* rgb);

#ifdef __cplusplus
}
#endif

#endif
