/* $Id$ */
#include <Mw/Milsko.h>

void ok(MwWidget handle, void* user, void* call) {
	(void)handle;
	(void)call;
	MwMessageBoxDestroy(user);
}

void spawn(MwWidget handle, void* user, void* call) {
	MwWidget mb = MwMessageBox(user, "news has arrived!", "title", MwMB_ICONNEWS | MwMB_BUTTONOK);

	(void)handle;
	(void)call;

	MwAddUserHandler(MwMessageBoxGetChild(mb, MwMB_BUTTONOK), MwNactivateHandler, ok, mb);
}

void spawn2(MwWidget handle, void* user, void* call) {
	MwWidget mb = MwMessageBox(user, "something went wrong!", "title", MwMB_ICONERROR | MwMB_BUTTONOK);

	(void)handle;
	(void)call;

	MwAddUserHandler(MwMessageBoxGetChild(mb, MwMB_BUTTONOK), MwNactivateHandler, ok, mb);
}

void spawn3(MwWidget handle, void* user, void* call) {
	int i;

	(void)handle;
	(void)call;

	for(i = 0; i <= MwMB_ICONCLOCK; i++) {
		MwWidget mb = MwMessageBox(user, "messagebox test", "title", i | MwMB_BUTTONOK);
		MwAddUserHandler(MwMessageBoxGetChild(mb, MwMB_BUTTONOK), MwNactivateHandler, ok, mb);
	}
}

int main() {
	MwWidget msg  = MwVaCreateWidget(MwWindowClass, "test", NULL, MwDEFAULT, MwDEFAULT, 300, 100, MwNtitle, "test", NULL);
	MwWidget btn  = MwVaCreateWidget(MwButtonClass, "button", msg, 8, 8, 300 - 16, (100 - 16) / 2, MwNtext, "press me!", NULL);
	MwWidget btn2 = MwVaCreateWidget(MwButtonClass, "button", msg, 8, 8 + (100 - 16) / 2, (300 - 16) / 2, (100 - 16) / 2, MwNtext, "press me!", NULL);
	MwWidget btn3 = MwVaCreateWidget(MwButtonClass, "button", msg, 8 + (300 - 16) / 2, 8 + (100 - 16) / 2, (300 - 16) / 2, (100 - 16) / 2, MwNtext, "press me!", NULL);

	MwAddUserHandler(btn, MwNactivateHandler, spawn, msg);
	MwAddUserHandler(btn2, MwNactivateHandler, spawn2, msg);
	MwAddUserHandler(btn3, MwNactivateHandler, spawn3, msg);

	MwLoop(msg);
}
