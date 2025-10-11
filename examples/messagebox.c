/* $Id$ */
#include <Mw/Milsko.h>

void ok(MwWidget handle, void* user, void* call) {
	MwDestroyWidget(user);
}

void spawn(MwWidget handle, void* user, void* call) {
	MwWidget mb = MwMessageBox(user, "new news arrived!", "title", MwMB_ICONNEWS | MwMB_BUTTONOK);
	MwAddUserHandler(MwMessageBoxGetChild(mb, MwMB_BUTTONOK), MwNactivateHandler, ok, mb);
}

int main() {
	MwWidget msg = MwVaCreateWidget(MwWindowClass, "test", NULL, MwDEFAULT, MwDEFAULT, 300, 100, MwNtitle, "test", NULL);
	MwWidget btn = MwVaCreateWidget(MwButtonClass, "button", msg, 8, 8, 300 - 16, 100 - 16, MwNtext, "press me!", NULL);

	MwAddUserHandler(btn, MwNactivateHandler, spawn, msg);

	MwLoop(msg);
}
