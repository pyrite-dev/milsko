/* $Id$ */

#include <Milsko/Milsko.h>

void handler(MilskoWidget handle){
	printf("hello world!\n");
}

int main(){
	MilskoWidget window = MilskoCreateWidget(MilskoWindowClass, "main", NULL, 0, 0, 400, 400);
	MilskoWidget button = MilskoCreateWidget(MilskoButtonClass, "button", window, 50, 50, 300, 300);

	MilskoApply(window,
		MilskoNtitle, "hello world",
	NULL);

	MilskoApply(button,
		MilskoNactivateHandler, handler,
	NULL);

	MilskoLoop(window);
}
