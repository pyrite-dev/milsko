/* $Id$ */
#include <Milsko/Milsko.h>

static void create(MilskoWidget handle) {
	MilskoSetDefault(handle);
}

static void draw(MilskoWidget handle) {
	MilskoPoint p[6];
}

MilskoClassRec MilskoButtonClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MilskoClass MilskoButtonClass = &MilskoButtonClassRec;
