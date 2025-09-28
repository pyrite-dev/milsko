/* $Id$ */
#include <Milsko/Milsko.h>

static void draw(MilskoWidget handle) {
	MilskoLLColor c = MilskoLLAllocColor(handle->lowlevel, 255, 0, 0);
	MilskoPoint   p[4];

	p[0].x = 0;
	p[0].y = 0;
	p[1].x = MilskoGetInteger(handle, MilskoNwidth);
	p[1].y = 0;
	p[2].x = MilskoGetInteger(handle, MilskoNwidth);
	p[2].y = MilskoGetInteger(handle, MilskoNheight);
	p[3].x = 0;
	p[3].y = MilskoGetInteger(handle, MilskoNheight);

	MilskoLLPolygon(handle->lowlevel, p, 4, c);

	MilskoLLFreeColor(c);
}

MilskoClassRec MilskoWindowClassRec = {
    NULL, /* opaque */
    NULL, /* create */
    NULL, /* destroy */
    draw, /* draw */
    NULL  /* click */
};
MilskoClass MilskoWindowClass = &MilskoWindowClassRec;
