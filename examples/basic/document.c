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
}

static void MWAPI resize_document(MwWidget handle, void* user, void* call) {
	MwViewportSetSize(viewport, MwGetInteger(document, MwNwidth), MwGetInteger(document, MwNheight));
}

int main() {
	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
				  MwNtitle, "markdown document",
				  NULL);

	viewport = MwCreateWidget(MwViewportClass, "viewport", window, 0, 0, 0, 0);

	MwVaApply(MwGetParent(MwViewportGetViewport(viewport)),
		  MwNbackground, "#fff",
		  MwNforeground, "#000",
		  NULL);

	document = MwVaCreateWidget(MwDocumentClass, "document", MwViewportGetViewport(viewport), 0, 0, 0, 0,
				    MwNtext, "# Hello world!\nThis is a test text...",
				    NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize_window, NULL);
	MwAddUserHandler(document, MwNresizeHandler, resize_document, NULL);

	resize_window(window, NULL, NULL);

	MwLoop(window);
}
