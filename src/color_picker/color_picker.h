/* $Id$ */

#ifndef __MW_INTERNAL_COLOR_PICKER_H
#define __MW_INTERNAL_COLOR_PICKER_H

#include <Mw/ColorPicker.h>
#include <Mw/Milsko.h>

#define WIN_SIZE 512
#define PICKER_SIZE 360
#define IMG_POS_X(w) ((w - PICKER_SIZE) / 2)
#define IMG_POS_Y(h) ((h - PICKER_SIZE) / 2)
#define SCROLL_BAR_WIDTH 12
#define MARGIN (PICKER_SIZE / 32)
#define COLOR_DISPLAY_HEIGHT 12

#define HSV_HUE_SEXTANT 256.
#define HSV_HUE_STEPS (6. * HSV_HUE_SEXTANT)

#define HSV_HUE_MIN 0.
#define HSV_HUE_MAX (HSV_HUE_STEPS - 1.)
#define HSV_SAT_MIN 0.
#define HSV_SAT_MAX 255.
#define HSV_VAL_MIN 0.
#define HSV_VAL_MAX 255.

typedef struct _color_picker	    color_picker;
typedef struct _color_picker_vtable color_picker_vtable;
typedef struct _MwHSV		    MwHSV;

struct _MwHSV {
	double h; // angle in degrees
	double s; // a fraction between 0 and 1
	double v; // a fraction between 0 and 1
	MwBool generated;
};

struct _color_picker {
	MwWidget       parent;
	MwWidget       color_picker_img;
	MwWidget       value_slider;
	MwWidget       color_display;
	MwWidget       color_display_text;
	MwWidget       finish;
	MwLLPixmap     color_picker_pixmap;
	double	       value;
	unsigned char* color_picker_image_data;
	MwPoint	       point;
	double	       dist_table[PICKER_SIZE][PICKER_SIZE];
	MwHSV	       hue_table[PICKER_SIZE][PICKER_SIZE];
	MwRGB	       chosen_color;
};

color_picker* color_picker_setup(MwWidget parent, int w, int h);

#endif
