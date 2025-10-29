/* $Id$ */
#include "common.h"

#include <Mw/Widget/OpenGL.h>

MwWidget wWindow, wOpengl;
MwWidget bPlay, bPause;

int main(int argc, char** argv) {
	if(argc != 2) return 1;

	wWindow = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 5 + 640 + 5 + 64 + 5, 5 + 480 + 5,
				   MwNtitle, "mpeg player",
				   NULL);
	wOpengl = MwCreateWidget(MwOpenGLClass, "opengl", wWindow, 5, 5, 640, 480);
	bPlay	= MwVaCreateWidget(MwButtonClass, "play", wWindow, 5 + 640 + 5, 5, 64, 24,
				   MwNtext, "Play",
				   NULL);
	bPause	= MwVaCreateWidget(MwButtonClass, "pause", wWindow, 5 + 640 + 5, 5 + 24 + 5, 64, 24,
				   MwNtext, "Pause",
				   NULL);

	MwLoop(wWindow);
}
