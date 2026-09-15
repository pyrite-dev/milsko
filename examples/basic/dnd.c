#include <Mw/Milsko.h>

MwWidget window, box, instructions, cur_text, text_listbox;

static void MWAPI resize(MwWidget handle, void* user_data, void* call_data) {
	unsigned int w, h;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight);

	MwVaApply(box, MwNwidth, w, MwNheight, h, NULL);
}
static void MWAPI dnd(MwWidget handle, void* user_data, void* call_data) {
	char* filename = call_data;

	(void)handle;
	(void)user_data;

	if(filename != NULL) {
		MwListBoxSet(text_listbox, -1, -1, filename);
		MwForceRender(text_listbox);
	}
}

int main() {
	MwSizeHints hints;

	MwLibraryInit();
	hints.min_width = hints.min_height = 600;

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 600, 600,
				  MwNtitle, "dnd",
				  MwNsizeHints, &hints,
				  NULL);

	box = MwVaCreateWidget(MwBoxClass, NULL, window, 0, 0, 800, 800, MwNorientation, MwVERTICAL, MwNpadding, 25, NULL);

	instructions = MwVaCreateWidget(MwLabelClass, "button", box, 50, 50, 750, 50,
					MwNtext, "drag files onto this text box, and their names will show up below.",
					MwNacceptsDnD, 1,
					NULL);
	text_listbox = MwVaCreateWidget(MwListBoxClass, "label", box, 25, 150, 750, 700,
					MwNtext, "",
					MwNacceptsDnD, 1,
					NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(instructions, MwNdragAndDropHandler, dnd, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
