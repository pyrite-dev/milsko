/* $Id$ */
#include <Milsko/Milsko.h>

static void create(MilskoWidget handle) {
	MilskoSetDefault(handle);
}

static void draw(MilskoWidget handle) {
	MilskoLLColor c = MilskoParseColor(handle, MilskoGetText(handle, MilskoNbackground));
	MilskoRect    r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MilskoGetInteger(handle, MilskoNwidth);
	r.height = MilskoGetInteger(handle, MilskoNheight);

	MilskoDrawRect(handle, &r, c);

	MilskoLLFreeColor(c);
}

MilskoClassRec MilskoWindowClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MilskoClass MilskoWindowClass = &MilskoWindowClassRec;
