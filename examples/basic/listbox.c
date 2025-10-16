/* $Id$ */
#include <Mw/Milsko.h>

#include "../harvard.c"

MwWidget wmain;

void destroy(MwWidget handle, void* user, void* call) {
	(void)handle;
	(void)call;
	MwMessageBoxDestroy(user);
}

void activate(MwWidget handle, void* user, void* call) {
	char	 msg[256];
	MwWidget msgbox;

	(void)user;

	sprintf(msg, "You pressed: %s", MwListBoxGet(handle, *(int*)call));

	msgbox = MwMessageBox(wmain, msg, "wow", MwMB_ICONINFO | MwMB_BUTTONOK);
	MwAddUserHandler(MwMessageBoxGetChild(msgbox, MwMB_BUTTONOK), MwNactivateHandler, destroy, msgbox);
}

int main() {
	MwWidget lb;
	wmain = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				 MwNtitle, "listbox",
				 NULL);
	lb    = MwCreateWidget(MwListBoxClass, "listbox", wmain, 5, 5, 630, 470);

	MwAddUserHandler(lb, MwNactivateHandler, activate, NULL);
	MwListBoxInsertMultiple(lb, -1, (char**)harvard, NULL, sizeof(harvard) / sizeof(harvard[0]) - 1);

	MwLoop(wmain);
}
