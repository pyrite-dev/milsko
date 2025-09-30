/* $Id$ */
#include <Mw/Milsko.h>

int main() {
	MwWidget   window = MwVaCreateWidget(MwWindowClass, "window", NULL, 0, 0, 500, 500,
					     MwNtitle, "image button",
					     NULL);
	MwWidget   button = MwCreateWidget(MwButtonClass, "button", window, 50, 50, 400, 400);
	MwLLPixmap px	  = MwLoadImage(button, "examples/picture.png");

	MwVaApply(button,
		  MwNpixmap, px,
		  NULL);

	MwLoop(window);
}
