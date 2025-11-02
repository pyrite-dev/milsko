/* $Id$ */

/*!
 * @file Mw/ColorPicker.h
 * @brief Color picker
 */
#ifndef __MW_COLORPICKER_H__
#define __MW_COLORPICKER_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MwRGB MwRGB;

struct _MwRGB {
	MwU32 r;
	MwU32 g;
	MwU32 b;
};

typedef void (*MwColorPickerCallback)(MwRGB rgb);

MWDECL MwWidget MwColorPicker(MwWidget handle, const char* title, MwColorPickerCallback cb);

#ifdef __cplusplus
}
#endif

#endif
