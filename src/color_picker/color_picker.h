/* $Id$ */

#ifndef __MW_INTERNAL_COLOR_PICKER_H
#define __MW_INTERNAL_COLOR_PICKER_H

#include <Mw/ColorPicker.h>
#include <Mw/Milsko.h>

#define PICKER_SIZE 360
#define IMG_POS_X(w) ((w - PICKER_SIZE) / 2)
#define IMG_POS_Y(h) ((h - PICKER_SIZE) / 2)
#define SCROLL_BAR_WIDTH 12
#define MARGIN (PICKER_SIZE / 32)
#define COLOR_DISPLAY_HEIGHT 12

typedef struct _color_picker	    color_picker;
typedef struct _color_picker_inner  color_picker_inner;
typedef struct _color_picker_vtable color_picker_vtable;

struct _color_picker_inner {
	MwWidget       parent;
	MwWidget       color_picker_img;
	MwWidget       value_slider;
	MwWidget       color_display;
	MwWidget       color_display_text;
	MwLLPixmap     color_picker_pixmap;
	MwLLPixmap     color_display_pixmap;
	double	       value;
	unsigned char* color_picker_image_data;
	unsigned char* color_display_image_data;
	MwPoint	       point;
};

struct _color_picker_vtable {
	void* reserved;
};

struct _color_picker {
	color_picker_inner  inner;
	color_picker_vtable vtable;
};

color_picker* color_picker_setup(MwWidget parent, int w, int h);

#endif
