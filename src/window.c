/* $Id$ */
#include <Milsko/Milsko.h>

MilskoClassRec MilskoWindowClassRec = {
    NULL, /* opaque */
    NULL, /* create */
    NULL, /* destroy */
    NULL, /* draw */
    NULL  /* click */
};
MilskoClass MilskoWindowClass = &MilskoWindowClassRec;
