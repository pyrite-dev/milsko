/* $Id$ */

#include <Milsko/Milsko.h>

int main(){
	MilskoWidget window = MilskoCreateWidget(MilskoWindowClass, "main", NULL, 0, 0, 640, 480);

	MilskoApply(window,
		MilskoNwidth, 480 * 2,
		MilskoNtitle, "hello world",
	NULL);

	MilskoLoop(window);
}
