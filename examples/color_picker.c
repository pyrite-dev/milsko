/* $Id$ */
#include <Mw/Milsko.h>

MwWidget vp;

#define WIN_SIZE 512
#define PICKER_SIZE 360
#define IMG_POS ((WIN_SIZE - PICKER_SIZE) / 2)
#define SCROLL_BAR_WIDTH (PICKER_SIZE / 32)
#define MARGIN (PICKER_SIZE / 32)
#define COLOR_DISPLAY_HEIGHT (PICKER_SIZE / 4)

typedef struct {
	double r; // a fraction between 0 and 1
	double g; // a fraction between 0 and 1
	double b; // a fraction between 0 and 1
} rgb;

typedef struct {
	double h; // angle in degrees
	double s; // a fraction between 0 and 1
	double v; // a fraction between 0 and 1
} hsv;

static rgb hsv2rgb(hsv in) {
	rgb out;

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

typedef struct {
	MwWidget       parent;
	MwWidget       color_wheel_img;
	MwWidget       value_slider;
	MwWidget       alpha_slider;
	MwWidget       color_display;
	MwLLPixmap     color_wheel_pixmap;
	MwLLPixmap     color_display_pixmap;
	double	       value;
	double	       alpha;
	unsigned char* color_wheel_image_data;
	unsigned char* color_display_image_data;
} color_wheel;

void color_wheel_wheel_image_update(color_wheel* wheel) {
	for(int y = 0; y < PICKER_SIZE; y++) {
		for(int x = 0; x < PICKER_SIZE; x++) {
			int i  = ((y * PICKER_SIZE) + x) * 4;
			int _x = x - (PICKER_SIZE / 2);
			int _y = y - (PICKER_SIZE / 2);

			double dist = sqrt(_x * _x + _y * _y);

			if(dist >= 180.) {
				wheel->color_wheel_image_data[i]     = 0;
				wheel->color_wheel_image_data[i + 1] = 0;
				wheel->color_wheel_image_data[i + 2] = 0;
				wheel->color_wheel_image_data[i + 3] = 0;
			} else {
				double xd = (M_PI / 180.) * ((double)_x);
				double yd = (M_PI / 180.) * ((double)_y);

				float angle = atan2(yd, xd) - M_PI;
				float hue   = (angle * 180.) / M_PI;

				if(hue < 0.0) {
					hue += 360;
				}
				rgb color			     = hsv2rgb((hsv){
							       .h = hue / 360.,
							       .s = dist / 179.61,
							       .v = wheel->value,
				   });
				wheel->color_wheel_image_data[i]     = color.r * 255;
				wheel->color_wheel_image_data[i + 1] = color.g * 255;
				wheel->color_wheel_image_data[i + 2] = color.b * 255;
				wheel->color_wheel_image_data[i + 3] = wheel->alpha;
			}
		}
	}
	if(wheel->color_wheel_pixmap != NULL) {
		MwLLDestroyPixmap(wheel->color_wheel_pixmap);
	}
	wheel->color_wheel_pixmap = MwLoadRaw(wheel->parent, wheel->color_wheel_image_data, PICKER_SIZE, PICKER_SIZE);
	MwVaApply(wheel->color_wheel_img, MwNpixmap, wheel->color_wheel_pixmap, NULL);
}

static void color_wheel_click(MwWidget handle, void* user, void* call) {
	color_wheel* wheel = (color_wheel*)user;
	MwLLMouse*   mouse = (MwLLMouse*)call;

	color_wheel_wheel_image_update(wheel);

	int i = ((mouse->point.y * PICKER_SIZE) + mouse->point.x) * 4;

	int r = wheel->color_wheel_image_data[i];
	int g = wheel->color_wheel_image_data[i + 1];
	int b = wheel->color_wheel_image_data[i + 2];
	int a = wheel->color_wheel_image_data[i + 3];

	char* hexColor = malloc(8);
	snprintf(hexColor, 8, "#%02X%02X%02X", r, g, b);
	MwSetText(wheel->color_display, MwNbackground, hexColor);
	free(hexColor);
}
static void color_wheel_on_change_value(MwWidget handle, void* user, void* call) {
	color_wheel* wheel = (color_wheel*)user;

	int value = MwGetInteger(handle, MwNvalue);
	int diff  = MwGetInteger(handle, MwNchangedBy);

	wheel->value = 1.0 - ((double)value / 1024.);

	color_wheel_wheel_image_update(wheel);
}
static void color_wheel_on_change_alpha(MwWidget handle, void* user, void* call) {
	color_wheel* wheel = (color_wheel*)user;

	int value = MwGetInteger(handle, MwNvalue);
	int diff  = MwGetInteger(handle, MwNchangedBy);

	wheel->alpha = (1.0 - ((double)value / 1024.)) * 255.;

	color_wheel_wheel_image_update(wheel);
}
void color_wheel_setup(MwWidget parent, color_wheel* wheel) {
	wheel->parent = parent;

	wheel->color_wheel_img = MwVaCreateWidget(MwImageClass, "image", wheel->parent, IMG_POS, IMG_POS, PICKER_SIZE, PICKER_SIZE, NULL);

	wheel->color_wheel_image_data	= malloc(PICKER_SIZE * PICKER_SIZE * 4);
	wheel->color_display_image_data = malloc(PICKER_SIZE * COLOR_DISPLAY_HEIGHT * 4);

	wheel->color_wheel_pixmap   = NULL;
	wheel->color_display_pixmap = NULL;
	wheel->value		    = 1;
	wheel->alpha		    = 255;

	color_wheel_wheel_image_update(wheel);

	MwAddUserHandler(wheel->color_wheel_img, MwNmouseDownHandler, color_wheel_click, wheel);

	wheel->color_display = MwCreateWidget(MwFrameClass, "colorDisplayFrame", wheel->parent, IMG_POS, IMG_POS + PICKER_SIZE + MARGIN, PICKER_SIZE, PICKER_SIZE / 16);
	MwSetText(wheel->color_display, MwNbackground, "#000000");
	MwSetInteger(wheel->color_display, MwnhasBorder, 1);
	MwSetInteger(wheel->color_display, MwNinverted, 1);

	wheel->value_slider = MwVaCreateWidget(MwScrollBarClass, "value-slider", wheel->parent,
					       // x
					       IMG_POS +
						   PICKER_SIZE +
						   MARGIN,

					       // y
					       IMG_POS,

					       // width
					       SCROLL_BAR_WIDTH,

					       // height
					       PICKER_SIZE,

					       MwNorientation, MwVERTICAL, MwNminValue, 0, MwNmaxValue, 1024, NULL);

	MwAddUserHandler(wheel->value_slider, MwNchangedHandler, color_wheel_on_change_value, wheel);

	wheel->alpha_slider = MwVaCreateWidget(MwScrollBarClass, "alpha-slider", wheel->parent,
					       // x
					       IMG_POS,

					       // y
					       IMG_POS -
						   SCROLL_BAR_WIDTH - MARGIN,

					       // width
					       PICKER_SIZE,

					       // height
					       SCROLL_BAR_WIDTH,
					       MwNorientation, MwHORIZONTAL, MwNminValue, 0, MwNmaxValue, 1024, NULL);
	MwAddUserHandler(wheel->alpha_slider, MwNchangedHandler, color_wheel_on_change_alpha, wheel);
};

int main() {
	MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, WIN_SIZE, WIN_SIZE, MwNtitle, "test", NULL);

	color_wheel wheel;
	color_wheel_setup(w, &wheel);

	MwLoop(w);
}
