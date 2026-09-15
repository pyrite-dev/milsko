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
	char* text = malloc(64 * 1024);
	int   i;

	strcpy(text, "# Hello world!\nThis is a __test__ text...\n\n`Some monospace here`\n\n~~Removed~~");
	strcat(text, "\n```\nint main(){\n  printf(\"Hello, world!\\n\");\n}\n```");
	for(i = 0; i < 10; i++) {
		strcat(text, "\n\n**Lorem ipsum** dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	}

	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				  MwNtitle, "markdown document",
				  NULL);

	viewport = MwCreateWidget(MwViewportClass, "viewport", window, 0, 0, 0, 0);

	document = MwVaCreateWidget(MwDocumentClass, "document", MwViewportGetViewport(viewport), 0, 0, 0, 0,
				    MwNtext, text,
				    NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize_window, NULL);
	MwAddUserHandler(document, MwNlayoutHandler, layout_document, NULL);

	resize_window(window, NULL, NULL);

	free(text);

	MwLoop(window);
}
