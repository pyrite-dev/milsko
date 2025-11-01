/* $Id$ */
#include <Mw/Milsko.h>

#include "color_picker.h"
#include <Mw/LowLevelMath.h>

static MwRGB hsv2rgb(MwHSV in) {
	MwRGB out;

	int    i = (int)(floor(in.h * 6));
	double f = in.h * 6 - i;
	double p = in.v * (1 - in.s);
	double q = in.v * (1 - f * in.s);
	double t = in.v * (1 - (1 - f) * in.s);

	switch(i % 6) {
	case 0:
		out.r = in.v, out.g = t, out.b = p;
		break;
	case 1:
		out.r = q, out.g = in.v, out.b = p;
		break;
	case 2:
		out.r = p, out.g = in.v, out.b = t;
		break;
	case 3:
		out.r = p, out.g = q, out.b = in.v;
		break;
	case 4:
		out.r = t, out.g = p, out.b = in.v;
		break;
	case 5:
		out.r = in.v, out.g = p, out.b = q;
		break;
	}

	return out;
}

static void color_picker_wheel_image_update(color_picker* picker) {
	int y, x;
	for(y = 0; y < PICKER_SIZE; y++) {
		for(x = 0; x < PICKER_SIZE; x++) {
			int i  = ((y * PICKER_SIZE) + x) * 4;
			int _x = x - (PICKER_SIZE / 2);
			int _y = y - (PICKER_SIZE / 2);

			double dist = sqrt(_x * _x + _y * _y);

			if(dist >= 180.) {
				picker->inner.color_picker_image_data[i]     = 0;
				picker->inner.color_picker_image_data[i + 1] = 0;
				picker->inner.color_picker_image_data[i + 2] = 0;
				picker->inner.color_picker_image_data[i + 3] = 0;
			} else {
				double xd = (M_PI / 180.) * ((double)_x);
				double yd = (M_PI / 180.) * ((double)_y);

				float angle = atan2(yd, xd) - M_PI;
				float hue   = (angle * 180.) / M_PI;

				MwHSV hsv_v;
				MwRGB color;

				if(hue < 0.0) {
					hue += 360;
				}
				hsv_v.h = hue / 360.;
				hsv_v.s = (dist / 179.61);
				hsv_v.v = picker->inner.value;
				color	= hsv2rgb(hsv_v);

				picker->inner.color_picker_image_data[i]     = color.r * 255;
				picker->inner.color_picker_image_data[i + 1] = color.g * 255;
				picker->inner.color_picker_image_data[i + 2] = color.b * 255;

				picker->inner.color_picker_image_data[i + 3] = 255;
			}
		}
	}
	if(picker->inner.color_picker_pixmap != NULL) {
		MwLLDestroyPixmap(picker->inner.color_picker_pixmap);
	}
	picker->inner.color_picker_pixmap = MwLoadRaw(
	    picker->inner.parent, picker->inner.color_picker_image_data, PICKER_SIZE, PICKER_SIZE);
	MwVaApply(picker->inner.color_picker_img, MwNpixmap, picker->inner.color_picker_pixmap, NULL);
}

static void color_picker_click(MwWidget handle, void* user, void* call) {
	color_picker* picker = (color_picker*)user;
	MwLLMouse*    mouse  = (MwLLMouse*)call;
	char*	      hexColor;
	int	      i, r, g, b, a;
	char*	      fgColor;
	int	      fr, fg, fb;

	(void)handle;
	(void)user;
	(void)call;

	color_picker_wheel_image_update(picker);

	i = ((mouse->point.y * PICKER_SIZE) + mouse->point.x) * 4;

	r = picker->inner.color_picker_image_data[i];
	g = picker->inner.color_picker_image_data[i + 1];
	b = picker->inner.color_picker_image_data[i + 2];
	a = picker->inner.color_picker_image_data[i + 3];

	(void)a;

	hexColor = malloc(8);
	fgColor	 = malloc(8);
	sprintf(hexColor, "#%02X%02X%02X", r, g, b);

	fr = r > 128 ? 0 : 255;
	fg = g > 128 ? 0 : 255;
	fb = b > 128 ? 0 : 255;

	sprintf(fgColor, "#%02X%02X%02X", fr, fg, fb);
	MwSetText(picker->inner.color_display, MwNbackground, hexColor);
	MwSetText(picker->inner.color_display_text, MwNforeground, fgColor);

	MwSetText(picker->inner.color_display_text, MwNbackground, hexColor);
	MwSetText(picker->inner.color_display_text, MwNtext, hexColor);
	free(hexColor);
}
static void color_picker_on_change_value(MwWidget handle, void* user,
					 void* call) {
	color_picker* picker = (color_picker*)user;

	int value = MwGetInteger(handle, MwNvalue);
	int diff  = MwGetInteger(handle, MwNchangedBy);

	(void)diff;
	(void)call;

	picker->inner.value = 1.0 - ((double)value / 1024.);

	color_picker_wheel_image_update(picker);
}
color_picker* color_picker_setup(MwWidget parent, int w, int h) {
	color_picker* picker = malloc(sizeof(color_picker));
	memset(picker, 0, sizeof(color_picker));

	picker->inner.parent = parent;

	picker->inner.color_picker_img =
	    MwVaCreateWidget(MwImageClass, "image", picker->inner.parent, IMG_POS_X(w), IMG_POS_Y(h),
			     PICKER_SIZE, PICKER_SIZE, NULL);

	picker->inner.color_picker_image_data = malloc(PICKER_SIZE * PICKER_SIZE * 4);
	picker->inner.color_display_image_data =
	    malloc(PICKER_SIZE * COLOR_DISPLAY_HEIGHT * 4);

	picker->inner.color_picker_pixmap  = NULL;
	picker->inner.color_display_pixmap = NULL;
	picker->inner.value		   = 1;

	color_picker_wheel_image_update(picker);

	MwAddUserHandler(picker->inner.color_picker_img, MwNmouseDownHandler,
			 color_picker_click, picker);

	picker->inner.color_display = MwCreateWidget(
	    MwFrameClass, "colorDisplayFrame", picker->inner.parent, IMG_POS_X(w),
	    IMG_POS_Y(h) + PICKER_SIZE + MARGIN, PICKER_SIZE, PICKER_SIZE / 16);
	MwSetText(picker->inner.color_display, MwNbackground, "#000000");
	MwSetInteger(picker->inner.color_display, MwnhasBorder, 1);
	MwSetInteger(picker->inner.color_display, MwNinverted, 1);

	picker->inner.color_display_text = MwCreateWidget(
	    MwLabelClass, "colorDisplayFrameText", picker->inner.color_display,
	    MwDefaultBorderWidth(parent), MwDefaultBorderWidth(parent),
	    PICKER_SIZE - MwDefaultBorderWidth(parent),
	    (PICKER_SIZE / 16) - (MwDefaultBorderWidth(parent) * 2));

	picker->inner.value_slider = MwVaCreateWidget(
	    MwScrollBarClass, "value-slider", picker->inner.parent,
	    // x
	    IMG_POS_X(w) + PICKER_SIZE + MARGIN,

	    // y
	    IMG_POS_Y(h),

	    // width
	    SCROLL_BAR_WIDTH,

	    // height
	    PICKER_SIZE,

	    MwNorientation, MwVERTICAL, MwNminValue, 0, MwNmaxValue, 1024, NULL);

	MwAddUserHandler(picker->inner.value_slider, MwNchangedHandler,
			 color_picker_on_change_value, picker);

	return picker;
};

MwWidget MwColorPicker(MwWidget handle, const char* title) {
	MwPoint	      p;
	color_picker* wheel;
	MwWidget      window;
	int	      ww, wh;

	// remove later
	MwLLVec test_1	 = MwLLVecU32x2(2, 5);
	MwLLVec test_2	 = MwLLVecU32x2(2, 12);
	MwLLVec test_out = MwLLVecU32x2(0, 0);

	MwLLMathAdd(&test_1, &test_2, &test_out);

	printf("simd result:\n");
	printf("%d + %d = %d\n", test_1.un.u32.a, test_2.un.u32.a, test_out.un.u32.a);
	printf("%d + %d = %d\n", test_1.un.u32.b, test_2.un.u32.b, test_out.un.u32.b);

	ww  = MwGetInteger(handle, MwNwidth);
	wh  = MwGetInteger(handle, MwNheight);
	p.x = p.y = 0;

	window = MwVaCreateWidget(MwWindowClass, "main", handle, MwDEFAULT, MwDEFAULT,
				  ww, wh, MwNtitle, title, NULL);

	wheel = color_picker_setup(window, ww, wh);

	MwLLDetach(window->lowlevel, &p);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);

	return window;
}
