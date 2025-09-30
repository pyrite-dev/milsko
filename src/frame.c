/* $Id$ */
#include <Mw/Milsko.h>

MwLLPixmap pixmap;

static void create(MwWidget handle) {
	MwSetDefault(handle);

	int	       y, x;
	unsigned char* dat = malloc(640 * 480 * 3);
	for(y = 0; y < 480; y++) {
		for(x = 0; x < 640; x++) {
			unsigned char* px = &dat[(y * 640 + x) * 3];
			double	       c  = (double)x / 640 * 255 / 2 + (double)y / 480 * 255 / 2;

			px[0] = c;
			px[1] = 0;
			px[2] = 0;
		}
	}
	pixmap = MwLLCreatePixmap(handle->lowlevel, dat, 640, 480);
	free(dat);
}

static void draw(MwWidget handle) {
	MwRect	  r;
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrameEx(handle, &r, base, 1, 1);
	MwDrawFrameEx(handle, &r, base, 0, 1);
	MwLLDrawPixmap(handle->lowlevel, &r, pixmap);
	//	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

MwClassRec MwFrameClassRec = {
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MwClass MwFrameClass = &MwFrameClassRec;
