/* $Id$ */

#include <Mw/Milsko.h>

MwWidget cpicker;
MwWidget window;
MwWidget button;

void color_callback(MwWidget handle, void* user_data, void* call_data) {
	char   hexColor[8];
	MwRGB* rgb = call_data;

	(void)handle;
	(void)user_data;

	rgb->red &= 0xff;
	rgb->green &= 0xff;
	rgb->blue &= 0xff;
	sprintf(hexColor, "#%02X%02X%02X", rgb->red, rgb->green, rgb->blue);
	MwSetText(window, MwNbackground, hexColor);
}

void color_picker(MwWidget handle, void* user_data, void* call_data) {
	MwWidget cpicker = MwColorPicker(window, "cpicker");

	(void)handle;
	(void)user_data;
	(void)call_data;

	MwAddUserHandler(cpicker, MwNcolorChosenHandler, color_callback, NULL);

	MwSetText(cpicker, MwNbackground, MwDefaultBackground);
}

int main() {
	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT,
				  MwDEFAULT, 640, 480, MwNtitle, "color picker", NULL);
	MwSetText(window, MwNbackground, "#000000");

	button = MwVaCreateWidget(MwButtonClass, "button", window, 160, 180, 320, 120,
				  MwNtext, "change window background",
				  NULL);
	MwSetText(button, MwNbackground, MwDefaultBackground);

	MwAddUserHandler(button, MwNactivateHandler, color_picker, NULL);

	MwLoop(window);
}
