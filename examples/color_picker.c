/* $Id$ */
#include <Mw/Milsko.h>

MwWidget vp;

#define WIN_SIZE 512
#define PICKER_SIZE 360
#define IMG_POS ((WIN_SIZE - PICKER_SIZE) / 2)

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
	MwWidget       slider;
	MwWidget       color_display;
	MwLLPixmap     color_wheel_pixmap;
	MwLLPixmap     color_display_pixmap;
	double	       value;
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
				wheel->color_wheel_image_data[i + 3] = 255;
			}
		}
	}
	if(wheel->color_wheel_pixmap != NULL) {
		MwLLDestroyPixmap(wheel->color_wheel_pixmap);
	}
	wheel->color_wheel_pixmap = MwLoadRaw(wheel->parent, wheel->color_wheel_image_data, PICKER_SIZE, PICKER_SIZE);
	MwVaApply(wheel->color_wheel_img, MwNpixmap, wheel->color_wheel_pixmap, NULL);
}

void color_wheel_color_display_update(color_wheel* wheel, int r, int g, int b) {
	for(int y = 0; y < PICKER_SIZE / 16; y++) {
		for(int x = 0; x < PICKER_SIZE; x++) {
			int i = ((y * PICKER_SIZE) + x) * 4;

			wheel->color_display_image_data[i]     = r;
			wheel->color_display_image_data[i + 1] = g;
			wheel->color_display_image_data[i + 2] = b;
			wheel->color_display_image_data[i + 3] = 255;
		}
	}
	if(wheel->color_display_pixmap != NULL) {
		MwLLDestroyPixmap(wheel->color_display_pixmap);
	}
	wheel->color_display_pixmap = MwLoadRaw(wheel->parent, wheel->color_display_image_data, PICKER_SIZE, (PICKER_SIZE / 16));
	MwVaApply(wheel->color_display, MwNpixmap, wheel->color_display_pixmap, NULL);
}
static void color_wheel_click(MwWidget handle, void* user, void* call) {
	color_wheel* wheel = (color_wheel*)user;
	MwLLMouse*   mouse = (MwLLMouse*)call;

	color_wheel_wheel_image_update(wheel);

	int i = ((mouse->point.y * PICKER_SIZE) + mouse->point.x) * 4;

	int r = wheel->color_wheel_image_data[i];
	int g = wheel->color_wheel_image_data[i + 1];
	int b = wheel->color_wheel_image_data[i + 2];

	color_wheel_color_display_update(wheel, r, g, b);

	printf("%d %d %d\n", r, g, b);
}
static void color_wheel_on_change(MwWidget handle, void* user, void* call) {
	color_wheel* wheel = (color_wheel*)user;

	int value = MwGetInteger(handle, MwNvalue);
	int diff  = MwGetInteger(handle, MwNchangedBy);

	wheel->value = 1.0 - ((double)value / 1024.);

	color_wheel_wheel_image_update(wheel);
}

void color_wheel_setup(MwWidget parent, color_wheel* wheel) {
	wheel->parent = parent;

	wheel->color_wheel_img		= MwVaCreateWidget(MwImageClass, "image", wheel->parent, IMG_POS, IMG_POS, PICKER_SIZE, PICKER_SIZE, NULL);
	wheel->color_wheel_image_data	= malloc(PICKER_SIZE * PICKER_SIZE * 4);
	wheel->color_display_image_data = malloc((PICKER_SIZE / 16) * PICKER_SIZE * 4);

	wheel->color_wheel_pixmap   = NULL;
	wheel->color_display_pixmap = NULL;
	wheel->value		    = 1;

	color_wheel_wheel_image_update(wheel);

	MwAddUserHandler(wheel->color_wheel_img, MwNmouseDownHandler, color_wheel_click, wheel);

	wheel->slider = MwVaCreateWidget(MwScrollBarClass, "scroll", wheel->parent, IMG_POS + PICKER_SIZE, IMG_POS, PICKER_SIZE / 32, PICKER_SIZE, MwNorientation, MwVERTICAL, MwNminValue, 0, MwNmaxValue, 1024, NULL);
	MwAddUserHandler(wheel->slider, MwNchangedHandler, color_wheel_on_change, wheel);

	wheel->color_display = MwCreateWidget(MwImageClass, "colorDisplay", wheel->parent, IMG_POS, IMG_POS + PICKER_SIZE + (PICKER_SIZE / 64), PICKER_SIZE, PICKER_SIZE / 16);
};

int main() {
	MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, WIN_SIZE, WIN_SIZE, MwNtitle, "test", NULL);

	color_wheel wheel;
	color_wheel_setup(w, &wheel);

	MwLoop(w);
}
