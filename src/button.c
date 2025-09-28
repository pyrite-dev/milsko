/* $Id$ */
#include <Milsko/Milsko.h>

static void create(MilskoWidget handle) {
}

static void draw(MilskoWidget handle) {
}

MilskoClassRec MilskoButtonClassRec = {
    NULL,   /* opaque */
    create, /* create */
    NULL,   /* destroy */
    draw,   /* draw */
    NULL    /* click */
};
MilskoClass MilskoButtonClass = &MilskoButtonClassRec;
