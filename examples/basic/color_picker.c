/* $Id$ */

#include <Mw/Milsko.h>

int main() {
	MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT,
				      MwDEFAULT, 640, 480, MwNtitle, "test", NULL);

	MwWidget cpicker = MwColorPicker(w, "cpicker");

	(void)cpicker;

	MwLoop(w);
}
