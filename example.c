/* $Id$ */

#include <Milsko/Milsko.h>

int main(){
	HMILSKO window = MilskoCreateWidget(MilskoWindowClass, NULL, 0, 0, 640, 480);

	MilskoApply(window,
		MilskoNwidth, 480 * 2,
		MilskoNtitle, "hello world",
	NULL);

	MilskoLoop(window);
}
