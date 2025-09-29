/* $Id$ */

#include <Milsko/Milsko.h>

void handler(MilskoWidget handle, void* user_data, void* call_data){
	printf("hello world!\n");
}

int main(){
	MilskoWidget window = MilskoVaCreateWidget(MilskoWindowClass, "main", NULL, 0, 0, 400, 400,
		MilskoNtitle, "hello world",
	NULL);
	MilskoWidget button = MilskoVaCreateWidget(MilskoButtonClass, "button", window, 50, 50, 300, 300, NULL);

	MilskoAddUserHandler(button, MilskoNactivateHandler, handler, NULL);

	MilskoLoop(window);
}
