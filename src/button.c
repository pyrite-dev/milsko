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

	MilskoDrawFrame(handle, &r, MilskoParseColor(handle, MilskoGetText(handle, MilskoNbackground)), handle->pressed);

	MilskoDrawRect(handle, &r, MilskoParseColor(handle, MilskoGetText(handle, MilskoNbackground)));
}

static void click(MilskoWidget handle) {
	MilskoDispatchHandler(handle, MilskoNactivateHandler);
}

MilskoClassRec MilskoButtonClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    click   /* click */
};
MilskoClass MilskoButtonClass = &MilskoButtonClassRec;
