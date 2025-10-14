/* $Id$ */
#include <Mw/Milsko.h>

int main() {
	MwWidget   window = MwVaCreateWidget(MwWindowClass, "window", NULL, MwDEFAULT, MwDEFAULT, 500, 500,
					     MwNtitle, "image image",
					     NULL);
	MwWidget   image  = MwCreateWidget(MwImageClass, "image", window, 50, 50, 200, 400);
	MwWidget   image2 = MwCreateWidget(MwImageClass, "image", window, 250, 50, 200, 200);
	MwWidget   image3 = MwCreateWidget(MwImageClass, "image", window, 250, 250, 200, 200);
	MwLLPixmap px	  = MwLoadImage(window, "examples/picture.png");
	MwLLPixmap px2	  = MwLoadImage(window, "examples/picture.jpg");
	MwLLPixmap px3	  = MwLoadImage(window, "resource/logo.png");

	MwVaApply(window,
		  MwNiconPixmap, px,
		  NULL);

	MwVaApply(image,
		  MwNpixmap, px,
		  NULL);
	MwVaApply(image2,
		  MwNpixmap, px2,
		  NULL);
	MwVaApply(image3,
		  MwNpixmap, px3,
		  NULL);

	MwLoop(window);
}
