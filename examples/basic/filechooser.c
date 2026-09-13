#include <Mw/Milsko.h>

MwWidget fpicker;
MwWidget window;
MwWidget button;
MwWidget mb;

#ifdef BUG
MwBool fpicker_wait = MwFALSE;
MwBool msgbox_wait  = MwFALSE;
#endif

void MWAPI ok(MwWidget handle, void* user, void* call) {
	(void)handle;
	(void)call;
#ifdef BUG
	msgbox_wait = MwFALSE;
#endif

	MwDestroyWidget(mb);
}

void MWAPI file_callback(MwWidget handle, void* user_data, void* call_data) {
	mb = MwMessageBox(window, call_data, "File chosen", MwMB_ICONERROR | MwMB_BUTTONOK);

	(void)user_data;

	MwAddUserHandler(MwMessageBoxGetChild(mb, MwMB_BUTTONOK), MwNactivateHandler, ok, mb);
	MwAddUserHandler(mb, MwNcloseHandler, ok, mb);

#ifdef BUG
	msgbox_wait = MwTRUE;

	while(msgbox_wait) {
		MwStep(mb);
	}

	fpicker_wait = MwFALSE;
#endif
}

void MWAPI file_picker(MwWidget handle, void* user_data, void* call_data) {
	fpicker = MwFileChooserEx(handle, "file chooser", 0);

	(void)handle;
	(void)user_data;
	(void)call_data;

	MwAddUserHandler(fpicker, MwNfileChosenHandler, file_callback, NULL);

	MwSetText(fpicker, MwNbackground, MwGetInteger(fpicker, MwNdarkTheme) ? MwDefaultDarkBackground : MwDefaultBackground);

#ifdef BUG
	fpicker_wait = MwTRUE;

	while(fpicker_wait) {
		MwStep(fpicker);
	}

	MwDestroyWidget(fpicker);
	MwDestroyWidget(mb);
#endif
}

int main() {
	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT,
				  MwDEFAULT, 640, 480, MwNtitle, "file chooser", NULL);

	button = MwVaCreateWidget(MwButtonClass, "button", window, 160, 180, 320, 120,
				  MwNtext, "select file",
				  NULL);

	MwAddUserHandler(button, MwNactivateHandler, file_picker, NULL);

	MwLoop(window);
}
