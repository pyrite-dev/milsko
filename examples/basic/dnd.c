#include <Mw/Milsko.h>

MwWidget window, instructions, text1, cur_text;

static void MWAPI resize(MwWidget handle, void* user_data, void* call_data) {
	unsigned int w, h;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight);

	MwVaApply(instructions,
		  MwNy, 50,
		  MwNwidth, w - 50 * 2,
		  MwNheight, h - 125 - 50 * 3,
		  NULL);

	MwVaApply(text1,
		  MwNy, 150,
		  MwNwidth, w - 25 * 2,
		  MwNheight, h - 125 - 50 * 3,
		  NULL);
}
static void MWAPI dnd(MwWidget handle, void* user_data, void* call_data) {
	char* filename = call_data;

	(void)handle;
	(void)user_data;

	if(filename != NULL) {
		MwVaApply(cur_text, MwNtext, filename, NULL);
		MwForceRender(cur_text);
	}
}

int main() {
	MwLibraryInit();

	window	     = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 400, 400,
					MwNtitle, "dnd",
					NULL);
	instructions = MwVaCreateWidget(MwLabelClass, "button", window, 50, 50, 300, 125,
					MwNtext, "drag a file and its name will show up below.",
					NULL);
	text1	     = MwVaCreateWidget(MwLabelClass, "label", window, 25, 150, 300, 75,
					MwNtext, "",
					NULL);

	cur_text = text1;

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(window, MwNdragAndDropHandler, dnd, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
