/* $Id$ */
#include <Mw/Milsko.h>

#include "color_picker.h"
#include <Mw/LowLevelMath.h>

static void hsv2rgb(MwU32 h, MwU32 s, MwU32 v, MwU32* r, MwU32* g, MwU32* b) {
	uint8_t sextant = h >> 8;

#define HSV_SWAPPTR(a, b) \
	do { \
		MwU32* tmp = a; \
		a	   = b; \
		b	   = tmp; \
	} while(0)

	if(sextant & 2) {
		HSV_SWAPPTR(r, b);
	}
	if(sextant & 4) {

		HSV_SWAPPTR(g, b);
	}
	if(!(sextant & 6)) {
		if(!(sextant & 1)) {
			HSV_SWAPPTR(r, g);
		}
	} else {
		if(sextant & 1) {
			HSV_SWAPPTR(r, g);
		}
	}

	*g = v; // Top level

	// Perform actual calculations

	/*
	 * Bottom level: v * (1.0 - s)
	 * --> (v * (255 - s) + error_corr + 1) / 256
	 */
	uint16_t ww;	      // Intermediate result
	ww = v * (255 - (s)); // We don't use ~s to prevent size-promotion side effects
	ww += 1;	      // Error correction
	ww += ww >> 8;	      // Error correction
	*b = ww >> 8;

	MwU32 h_fraction = h & 0xff; // 0...255
	MwU32 d;		     // Intermediate result

	if(!(sextant & 1)) {
		// *r = ...slope_up...;
		d  = v * (MwU32)((0xff << 8) - (MwU16)(s * (0xff - h_fraction)));
		*r = d >> 16;
	} else {
		// *r = ...slope_down...;
		d  = v * (MwU32)((0xff << 8) - (MwU16)(s * h_fraction));
		*r = d >> 16;
	}
	return;
}

static void color_picker_image_update(color_picker* picker) {
	int    y, x;
	double h;
	for(y = 0; y < PICKER_SIZE; y++) {
		for(x = 0; x < PICKER_SIZE; x++) {
			int i  = ((y * PICKER_SIZE) + x) * 4;
			int _x = x - (PICKER_SIZE / 2);
			int _y = y - (PICKER_SIZE / 2);

			if(picker->dist_table[y][x] == 0) {
				picker->dist_table[y][x] = sqrt(_x * _x + _y * _y);
			}
			double dist = picker->dist_table[y][x];

			if(dist >= 180.) {
				picker->color_picker_image_data[i]     = 0;
				picker->color_picker_image_data[i + 1] = 0;
				picker->color_picker_image_data[i + 2] = 0;
				picker->color_picker_image_data[i + 3] = 0;
			} else {
				MwHSV hsv_v;
				if(picker->hue_table[y][x].generated == 0) {
					double xd = (M_PI / 180.) * ((double)_x);
					double yd = (M_PI / 180.) * ((double)_y);

					float angle = atan2(yd, xd) - M_PI;
					float hue   = (angle * 180.) / M_PI;

					if(hue < 0.0) {
						hue += 360;
					}
					hsv_v.h				  = (hue) * (HSV_HUE_STEPS / 360.);
					hsv_v.s				  = (dist) * (HSV_SAT_MAX / 180.);
					picker->hue_table[y][x]		  = hsv_v;
					picker->hue_table[y][x].generated = 1;
				}

				hsv_v	= picker->hue_table[y][x];
				hsv_v.v = HSV_VAL_MAX - (picker->value * HSV_VAL_MAX);

				MwRGB color;
				hsv2rgb(hsv_v.h, hsv_v.s, hsv_v.v, &color.r, &color.g, &color.b);

				picker->color_picker_image_data[i]     = color.r;
				picker->color_picker_image_data[i + 1] = color.g;
				picker->color_picker_image_data[i + 2] = color.b;

				picker->color_picker_image_data[i + 3] = 255;
			}
		}
	}
	if(picker->color_picker_pixmap == NULL) {
		picker->color_picker_pixmap = MwLoadRaw(
		    picker->parent, picker->color_picker_image_data, PICKER_SIZE, PICKER_SIZE);
	} else {
		MwReloadRaw(
		    picker->parent, picker->color_picker_image_data, PICKER_SIZE, PICKER_SIZE, picker->color_picker_pixmap);
	}
	MwVaApply(picker->color_picker_img, MwNpixmap, picker->color_picker_pixmap, NULL);
}

static void color_picker_click(MwWidget handle, void* user, void* call) {
	color_picker* picker = (color_picker*)user;
	MwLLMouse*    mouse  = (MwLLMouse*)call;
	char	      hexColor[8];
	int	      i;
	char	      fgColor[8];
	int	      fr, fg, fb;

	(void)handle;
	(void)user;
	(void)call;

	color_picker_image_update(picker);

	i = ((mouse->point.y * PICKER_SIZE) + mouse->point.x) * 4;

	picker->chosen_color.r = picker->color_picker_image_data[i];
	picker->chosen_color.g = picker->color_picker_image_data[i + 1];
	picker->chosen_color.b = picker->color_picker_image_data[i + 2];

	sprintf(hexColor, "#%02X%02X%02X", picker->chosen_color.r, picker->chosen_color.g, picker->chosen_color.b);

	fr = picker->chosen_color.r > 128 ? 0 : 255;
	fg = picker->chosen_color.g > 128 ? 0 : 255;
	fb = picker->chosen_color.b > 128 ? 0 : 255;

	sprintf(fgColor, "#%02X%02X%02X", fr, fg, fb);
	MwSetText(picker->color_display, MwNbackground, hexColor);
	MwSetText(picker->color_display_text, MwNforeground, fgColor);

	MwSetText(picker->color_display_text, MwNbackground, hexColor);
	MwSetText(picker->color_display_text, MwNtext, hexColor);
}
static void color_picker_on_change_value(MwWidget handle, void* user,
					 void* call) {
	color_picker* picker = (color_picker*)user;

	int value = MwGetInteger(handle, MwNvalue);
	int diff  = MwGetInteger(handle, MwNchangedBy);

	(void)diff;
	(void)call;

	picker->value = ((double)value / 1024.);

	color_picker_image_update(picker);
}

static void color_picker_destroy(color_picker* picker) {
	free(picker->color_picker_image_data);
	MwLLDestroyPixmap(picker->color_picker_pixmap);

	MwDestroyWidget(picker->color_display_text);
	MwDestroyWidget(picker->color_display);
	MwDestroyWidget(picker->value_slider);
	MwDestroyWidget(picker->color_picker_img);
}

static void color_picker_close(MwWidget handle, void* user,
			       void* call) {
	color_picker* picker = (color_picker*)user;
	color_picker_destroy(picker);
	MwDestroyWidget(handle);
}

static void color_picker_finish(MwWidget handle, void* user,
				void* call) {
	color_picker* picker = (color_picker*)user;

	picker->cb(picker->chosen_color);

	color_picker_destroy(picker);
	MwDestroyWidget(handle->parent);
}

color_picker* color_picker_setup(MwWidget parent, int w, int h) {
	color_picker* picker = malloc(sizeof(color_picker));
	memset(picker, 0, sizeof(color_picker));

	picker->parent = parent;

	picker->color_picker_img =
	    MwVaCreateWidget(MwImageClass, "image", picker->parent, IMG_POS_X(w), IMG_POS_Y(h),
			     PICKER_SIZE, PICKER_SIZE, NULL);

	picker->color_picker_image_data = malloc(PICKER_SIZE * PICKER_SIZE * 4);

	picker->color_picker_pixmap = NULL;
	picker->value		    = 0;

	color_picker_image_update(picker);

	MwAddUserHandler(picker->color_picker_img, MwNmouseDownHandler,
			 color_picker_click, picker);

	picker->color_display = MwCreateWidget(
	    MwFrameClass, "colorDisplayFrame", picker->parent, IMG_POS_X(w),
	    IMG_POS_Y(h) - (PICKER_SIZE / 16) - MARGIN, PICKER_SIZE, PICKER_SIZE / 16);
	MwSetText(picker->color_display, MwNbackground, "#FFFFFF");
	MwSetInteger(picker->color_display, MwnhasBorder, 1);
	MwSetInteger(picker->color_display, MwNinverted, 1);

	picker->color_display_text = MwCreateWidget(
	    MwLabelClass, "colorDisplayFrameText", picker->color_display,
	    MwDefaultBorderWidth(parent), MwDefaultBorderWidth(parent),
	    PICKER_SIZE - MwDefaultBorderWidth(parent),
	    (PICKER_SIZE / 16) - (MwDefaultBorderWidth(parent) * 2));

	MwSetText(picker->color_display_text, MwNtext, "#FFFFFF");

	picker->value_slider = MwVaCreateWidget(
	    MwScrollBarClass, "value-slider", picker->parent,
	    // x
	    IMG_POS_X(w) + PICKER_SIZE + MARGIN,

	    // y
	    IMG_POS_Y(h),

	    // width
	    SCROLL_BAR_WIDTH,

	    // height
	    PICKER_SIZE,

	    MwNorientation, MwVERTICAL, MwNminValue, 0, MwNmaxValue, 1024, NULL);

	MwAddUserHandler(picker->value_slider, MwNchangedHandler,
			 color_picker_on_change_value, picker);

	picker->finish = MwCreateWidget(
	    MwButtonClass, "colorPickerFinish", picker->parent, IMG_POS_X(w),
	    IMG_POS_Y(h) + PICKER_SIZE + MARGIN, PICKER_SIZE, PICKER_SIZE / 8);
	MwSetText(picker->finish, MwNtext, "Select");
	MwSetInteger(picker->finish, MwnhasBorder, 1);
	MwSetInteger(picker->finish, MwNinverted, 1);

	MwAddUserHandler(picker->finish, MwNactivateHandler,
			 color_picker_finish, picker);

	memset(picker->dist_table, 0, sizeof(picker->dist_table));
	memset(picker->hue_table, 0, sizeof(picker->hue_table));

	return picker;
};
MwWidget MwColorPicker(MwWidget handle, const char* title, MwColorPickerCallback cb) {
	MwPoint	      p;
	color_picker* wheel;
	MwWidget      window;

	p.x = p.y = 0;

	window = MwVaCreateWidget(MwWindowClass, "main", handle, MwDEFAULT, MwDEFAULT,
				  WIN_SIZE, WIN_SIZE, MwNtitle, title, NULL);

	wheel = color_picker_setup(window, WIN_SIZE, WIN_SIZE);

	MwAddUserHandler(window, MwNcloseHandler, color_picker_close, wheel);

	MwLLDetach(window->lowlevel, &p);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);

	wheel->cb = cb;

	return window;
}
