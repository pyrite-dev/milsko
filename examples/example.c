/* $Id$ */

#include <Mw/Milsko.h>

void handler(MwWidget handle, void* user_data, void* call_data){
	(void)handle;
	(void)user_data;
	(void)call_data;

	printf("hello world!\n");
}

int main(){
	int y, x;
	MwWidget window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 260,
		MwNtitle, "hello world",
	NULL);

	for(y = 0; y < 5; y++){
		for(x = 0; x < 2; x++){
			MwWidget button = MwVaCreateWidget(MwButtonClass, "button", window, 5 + 195 * x, 5 + 50 * y, 195, 50,
				MwNtext, "lorem ipsum",
			NULL);

			MwAddUserHandler(button, MwNactivateHandler, handler, NULL);
		}
	}

	MwLoop(window);
}
