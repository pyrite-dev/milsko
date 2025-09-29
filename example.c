/* $Id$ */

#include <Mw/Milsko.h>

void handler(MwWidget handle, void* user_data, void* call_data){
	printf("hello world!\n");
}

int main(){
	int i;
	MwWidget window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 400,
		MwNtitle, "hello world",
	NULL);

	for(i = 0; i < 6; i++){
		MwWidget button = MwVaCreateWidget(MwButtonClass, "button", window, 5, 5 + 55 * i, 390, 50,
			MwNtext, "lorem ipsum",
		NULL);

		MwAddUserHandler(button, MwNactivateHandler, handler, NULL);
	}

	MwLoop(window);
}
