/* $Id$ */

#include <Mw/Milsko.h>

MwWidget cpicker;
MwWidget window;
MwWidget button;

void color_callback(MwRGB rgb) {
	char hexColor[8];

	sprintf(hexColor, "#%02X%02X%02X", rgb.r, rgb.g, rgb.b);
	MwSetText(window, MwNbackground, hexColor);
}

void color_picker(MwWidget handle, void* user_data, void* call_data) {
	MwWidget cpicker = MwColorPicker(window, "cpicker", color_callback);
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
