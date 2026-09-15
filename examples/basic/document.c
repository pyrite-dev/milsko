#include <Mw/Milsko.h>

MwWidget window, viewport, document;

static void MWAPI resize_window(MwWidget handle, void* user, void* call) {
	int width  = MwGetInteger(window, MwNwidth);
	int height = MwGetInteger(window, MwNheight);

	MwVaApply(viewport,
		  MwNx, 10,
		  MwNy, 10,
		  MwNwidth, width - 20,
		  MwNheight, height - 20,
		  NULL);

	width  = MwGetInteger(MwGetParent(MwViewportGetViewport(viewport)), MwNwidth);
	height = MwGetInteger(MwGetParent(MwViewportGetViewport(viewport)), MwNheight);

	MwVaApply(document,
		  MwNwidth, width,
		  NULL);
}

static void MWAPI layout_document(MwWidget handle, void* user, void* call) {
	int height = *(int*)call;

	MwViewportSetSize(viewport, MwGetInteger(document, MwNwidth), height);
	MwVaApply(document,
		  MwNheight, height,
		  NULL);
}

int main() {
	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				  MwNtitle, "markdown document",
				  NULL);

	viewport = MwCreateWidget(MwViewportClass, "viewport", window, 0, 0, 0, 0);

	document = MwVaCreateWidget(MwDocumentClass, "document", MwViewportGetViewport(viewport), 0, 0, 0, 0,
				    MwNtext, "# Hello world!\nThis is a __test__ text...\n\n`Some monospace here`\n\n~~Removed~~\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
				    MwNbackground, "#fff",
				    MwNforeground, "#000",
				    NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize_window, NULL);
	MwAddUserHandler(document, MwNlayoutHandler, layout_document, NULL);

	resize_window(window, NULL, NULL);

	MwLoop(window);
}
