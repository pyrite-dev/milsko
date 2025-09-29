/* $Id$ */

#include <Mw/Mw.h>

void handler(MwWidget handle, void* user_data, void* call_data){
	printf("hello world!\n");
}

int main(){
	MwWidget window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 400,
		MwNtitle, "hello world",
	NULL);
	MwWidget button = MwVaCreateWidget(MwButtonClass, "button", window, 50, 50, 300, 300, NULL);

	MwAddUserHandler(button, MwNactivateHandler, handler, NULL);

	MwLoop(window);
}
