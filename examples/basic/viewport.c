/* $Id$ */
#include <Mw/Milsko.h>

MwWidget vp;

static void resize(MwWidget handle, void* user, void* call) {
	int w = MwGetInteger(handle, MwNwidth);
	int h = MwGetInteger(handle, MwNheight);

	MwVaApply(vp,
		  MwNwidth, w - 10,
		  MwNheight, h - 10,
		  NULL);
}

int main() {
	MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480, MwNtitle, "test", NULL);

	vp = MwCreateWidget(MwViewportClass, "vp", w, 5, 5, 630, 470);

	MwVaCreateWidget(MwImageClass, "image", MwViewportGetViewport(vp), 0, 0, 1024, 1024,
			 MwNpixmap, MwLoadImage(vp, "examples/picture.png"),
			 NULL);
	MwViewportSetSize(vp, 1024, 1024);

	MwAddUserHandler(w, MwNresizeHandler, resize, NULL);

	MwLoop(w);
}
