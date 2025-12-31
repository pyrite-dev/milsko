#define _MILSKO
#include <Mw/Milsko.h>

MwWidget window, instructions, text;

void resize(MwWidget handle, void* user_data, void* call_data) {
	unsigned int w, h, mh;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight);

	MwVaApply(instructions,
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
void clipboard(MwWidget handle, void* user_data, void* call_data) {
	char* clipboard = call_data;

	(void)handle;
	(void)user_data;

	if(clipboard != NULL) {
		MwVaApply(text, MwNtext, clipboard);
		MwForceRender(text);
	}
	MwForceRender(window);

	MwLLSetClipboard(window->lowlevel, "Milsko clipboard test");
}

int main() {
	MwLibraryInit();

	window	     = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 400, 400,
					MwNtitle, "clipboard",
					NULL);
	instructions = MwVaCreateWidget(MwLabelClass, "button", window, 50, 50, 300, 125,
					MwNtext, "clipboard contents will show up below.",
					NULL);
	text	     = MwVaCreateWidget(MwLabelClass, "label", window, 50, 200, 300, 125,
					MwNtext, "",
					NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(window, MwNclipboardHandler, clipboard, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
