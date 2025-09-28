/* $Id$ */
#include <Milsko/Milsko.h>

static void draw(MilskoWidget handle) {
}

MilskoClassRec MilskoWindowClassRec = {
    NULL, /* opaque */
    NULL, /* create */
    NULL, /* destroy */
    draw, /* draw */
    NULL  /* click */
};
MilskoClass MilskoWindowClass = &MilskoWindowClassRec;
