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
	MwWidget	 lb;
	int		 len = sizeof(harvard) / sizeof(harvard[0]) - 1;
	int		 i;
	MwListBoxPacket* packet;
	int		 index;
	wmain = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				 MwNtitle, "listbox",
				 NULL);
	lb    = MwCreateWidget(MwListBoxClass, "listbox", wmain, 5, 5, 630, 470);

	packet = MwListBoxCreatePacket();
	index  = MwListBoxPacketInsert(packet, -1);
	MwListBoxPacketSet(packet, index, 0, "Harvard sentence");
	MwListBoxPacketSet(packet, index, 1, "Length");

	for(i = 0; i < len; i++) {
		char sz[16];
		sprintf(sz, "%d", (int)strlen(harvard[i]));
		index = MwListBoxPacketInsert(packet, -1);
		MwListBoxPacketSet(packet, index, 0, harvard[i]);
		MwListBoxPacketSet(packet, index, 1, sz);
	}

	MwAddUserHandler(lb, MwNactivateHandler, activate, NULL);
	MwVaApply(lb,
		  MwNhasHeading, 1,
		  NULL);
	MwListBoxSetWidth(lb, 0, -64);
	MwListBoxInsert(lb, -1, packet);

	MwListBoxDestroyPacket(packet);

	MwLoop(wmain);
}
