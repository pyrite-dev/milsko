/* $Id$ */

#include <Mw/Milsko.h>

void handler(MwWidget handle, void* user_data, void* call_data){
	(void)handle;
	(void)user_data;
	(void)call_data;

	printf("hello world!\n");
}

int main(){
	MwWidget window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 400,
		MwNtitle, "hello world",
	NULL);
	MwWidget button = MwVaCreateWidget(MwButtonClass, "button", window, 50, 50, 300, 125,
		MwNtext, "lorem ipsum",
	NULL);
	MwWidget button2 = MwVaCreateWidget(MwButtonClass, "button", window, 50, 225, 100, 125,
		MwNtext, "lorem ipsum",
		MwNbackground, "#f66",
	NULL);
	MwWidget button3 = MwVaCreateWidget(MwButtonClass, "button", window, 150, 225, 100, 125,
		MwNtext, "lorem ipsum",
		MwNbackground, "#6f6",
	NULL);
	MwWidget button4 = MwVaCreateWidget(MwButtonClass, "button", window, 250, 225, 100, 125,
		MwNtext, "lorem ipsum",
		MwNbackground, "#66f",
	NULL);

	MwAddUserHandler(button, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button2, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button3, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button4, MwNactivateHandler, handler, NULL);

	MwLoop(window);
}
