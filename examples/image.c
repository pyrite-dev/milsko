/* $Id$ */
#include <Mw/Milsko.h>

int main() {
	MwWidget   window = MwVaCreateWidget(MwWindowClass, "window", NULL, 0, 0, 500, 500,
					     MwNtitle, "image image",
					     NULL);
	MwWidget   image  = MwCreateWidget(MwImageClass, "image", window, 50, 50, 400, 400);
	MwLLPixmap px	  = MwLoadImage(window, "examples/picture.png");

	MwVaApply(window,
		  MwNiconPixmap, px,
		  NULL);

	MwVaApply(image,
		  MwNpixmap, px,
		  NULL);

	MwLoop(window);
}
