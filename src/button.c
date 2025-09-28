/* $Id$ */
#include <Milsko/Milsko.h>

static void create(MilskoWidget handle) {
	MilskoSetDefault(handle);
}

static void draw(MilskoWidget handle) {
	MilskoRect r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MilskoGetInteger(handle, MilskoNwidth);
	r.height = MilskoGetInteger(handle, MilskoNheight);

	MilskoDrawFrame(handle, &r, MilskoParseColor(handle, MilskoGetText(handle, MilskoNbackground)), 0);

	MilskoDrawRect(handle, &r, MilskoParseColor(handle, MilskoGetText(handle, MilskoNbackground)));
}

MilskoClassRec MilskoButtonClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MilskoClass MilskoButtonClass = &MilskoButtonClassRec;
