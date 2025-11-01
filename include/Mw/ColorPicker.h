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
typedef struct _MwHSV MwHSV;

struct _MwRGB {
	double r;
	double g;
	double b;
};

struct _MwHSV {
	double h; // angle in degrees
	double s; // a fraction between 0 and 1
	double v; // a fraction between 0 and 1
};

MWDECL MwWidget MwColorPicker(MwWidget handle, const char* title);

#ifdef __cplusplus
}
#endif

#endif
