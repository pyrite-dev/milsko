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
	int	 len  = sizeof(harvard) / sizeof(harvard[0]) - 1;
	char**	 list = malloc(sizeof(*list) * len);
	int	 i;
	wmain = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				 MwNtitle, "listbox",
				 NULL);
	lb    = MwCreateWidget(MwListBoxClass, "listbox", wmain, 5, 5, 630, 470);

	for(i = 0; i < len; i++) {
		list[i] = malloc(16);
		sprintf(list[i], "%d", (int)strlen(harvard[i]));
	}

	MwAddUserHandler(lb, MwNactivateHandler, activate, NULL);
	MwListBoxInsert(lb, -1, NULL, "Harvard sentences", "Length", NULL);
	MwListBoxInsertMultiple(lb, -1, len, NULL, (char**)harvard, (char**)list, NULL);
	MwVaApply(lb,
		  MwNhasHeading, 1,
		  NULL);
	MwListBoxSetWidth(lb, 0, -64);

	for(i = 0; i < len; i++) free(list[i]);
	free(list);

	MwLoop(wmain);
}
