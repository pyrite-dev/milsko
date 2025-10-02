/* $Id$ */
#include <Mw/Milsko.h>

MwWidget window, menu, button, button2, button3, button4;

void handler(MwWidget handle, void* user_data, void* call_data) {
	(void)handle;
	(void)user_data;
	(void)call_data;

	printf("hello world!\n");
}

void resize(MwWidget handle, void* user_data, void* call_data) {
	unsigned int w, h, mh;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight) - (mh = MwGetInteger(menu, MwNheight));

	MwVaApply(button,
		  MwNy, 50 + mh,
		  MwNwidth, w - 50 * 2,
		  MwNheight, h - 125 - 50 * 3,
		  NULL);

	MwVaApply(button2,
		  MwNx, 50 + (w - 50 * 2) / 3 * 0,
		  MwNy, h - 50 - 125 + mh,
		  MwNwidth, (w - 50 * 2) / 3,
		  NULL);

	MwVaApply(button3,
		  MwNx, 50 + (w - 50 * 2) / 3 * 1,
		  MwNy, h - 50 - 125 + mh,
		  MwNwidth, (w - 50 * 2) / 3,
		  NULL);

	MwVaApply(button4,
		  MwNx, 50 + (w - 50 * 2) / 3 * 2,
		  MwNy, h - 50 - 125 + mh,
		  MwNwidth, (w - 50 * 2) / 3,
		  NULL);
}

int main() {
	MwMenu m;

	window	= MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 400,
				   MwNtitle, "hello world",
				   NULL);
	menu	= MwCreateWidget(MwMenuClass, "menu", window, 0, 0, 0, 0);
	button	= MwVaCreateWidget(MwButtonClass, "button", window, 50, 50, 300, 125,
				   MwNtext, "lorem ipsum",
				   NULL);
	button2 = MwVaCreateWidget(MwButtonClass, "button", window, 50, 225, 100, 125,
				   MwNtext, "lorem ipsum",
				   MwNbackground, "#f66",
				   NULL);
	button3 = MwVaCreateWidget(MwButtonClass, "button", window, 150, 225, 100, 125,
				   MwNtext, "lorem ipsum",
				   MwNbackground, "#6f6",
				   NULL);
	button4 = MwVaCreateWidget(MwButtonClass, "button", window, 250, 225, 100, 125,
				   MwNtext, "lorem ipsum",
				   MwNbackground, "#66f",
				   NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(button, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button2, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button3, MwNactivateHandler, handler, NULL);
	MwAddUserHandler(button4, MwNactivateHandler, handler, NULL);

	m = MwMenuAdd(menu, NULL, "test 1");
	MwMenuAdd(menu, m, "test 2");
	m = MwMenuAdd(menu, m, "test 3");
	MwMenuAdd(menu, m, "test 4");
	MwMenuAdd(menu, m, "test 6");
	MwMenuAdd(menu, NULL, "?test 5");

	MwLoop(window);
}
