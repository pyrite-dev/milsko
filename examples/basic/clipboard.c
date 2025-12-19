#define _MILSKO
#include <Mw/Milsko.h>

MwWidget window, button, text;

void resize(MwWidget handle, void* user_data, void* call_data) {
	unsigned int w, h, mh;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight);

	MwVaApply(button,
		  MwNy, 50 + mh,
		  MwNwidth, w - 50 * 2,
		  MwNheight, h - 125 - 50 * 3,
		  NULL);

	MwVaApply(text,
		  MwNy, 200 + mh,
		  MwNwidth, w - 50 * 2,
		  MwNheight, h - 125 - 50 * 3,
		  NULL);
}

void handler(MwWidget handle, void* user_data, void* call_data) {
	(void)handle;
	(void)user_data;
	(void)call_data;
	char* clipboard;

	clipboard = MwLLGetClipboard(handle->lowlevel);

	if(clipboard != NULL) {
		MwVaApply(text, MwNtext, clipboard);
		MwForceRender(text);
	}

	resize(window, NULL, NULL);
}

int main() {
	MwMenu m, m2;

	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 400, 400,
				  MwNtitle, "clipboard",
				  NULL);
	button = MwVaCreateWidget(MwButtonClass, "button", window, 50, 50, 300, 125,
				  MwNtext, "get clipboard contents",
				  NULL);
	text   = MwVaCreateWidget(MwLabelClass, "label", window, 50, 200, 300, 125,
				  MwNtext, "",
				  NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(button, MwNactivateHandler, handler, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
