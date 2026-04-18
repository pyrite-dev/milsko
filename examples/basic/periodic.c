#include <Mw/Milsko.h>

#include <stdarg.h>

#define Columns 5
#define Padding 20
#define PaddingContent 2

MwWidget window, menu, box;
MwWidget boxes[32];
int	 n = 0, row = -1;
MwMenu	 e;

static void resize(MwWidget handle, void* user, void* call) {
	int w = MwGetInteger(window, MwNwidth);
	int h = MwGetInteger(window, MwNheight);

	MwVaApply(box,
		  MwNx, Padding,
		  MwNy, MwGetInteger(menu, MwNheight) + Padding,
		  MwNwidth, w - Padding * 2,
		  MwNheight, h - MwGetInteger(menu, MwNheight) - Padding * 2,
		  NULL);
}

static void newrow(void) {
	if(row >= 0) {
		int i;

		for(i = n; i < Columns; i++) MwCreateWidget(MwFrameClass, "frame", boxes[row], 0, 0, 0, 0);
	}

	boxes[++row] = MwCreateWidget(MwBoxClass, "box", box, 0, 0, 0, 0);

	n = 0;
}

static void add(MwWidget w) {
	int r = MwGetInteger(w, MwNratio);

	if(r == MwDEFAULT) r = 1;

	n += r;

	if(n == Columns) {
		newrow();
	}
}

static void resize_content(MwWidget handle, void* user, void* call) {
	MwWidget* ws = MwGetChildren(handle);
	if(ws != NULL) {
		int pw = MwGetInteger(ws[0], "IpWidth");
		int ph = MwGetInteger(ws[0], "IpHeight");

		if(pw < 0) {
			pw += MwGetInteger(handle, MwNwidth);
		} else if(pw == 0) {
			pw = MwGetInteger(handle, MwNwidth);
		}

		if(ph < 0) {
			ph += MwGetInteger(handle, MwNheight);
		} else if(ph == 0) {
			ph = MwGetInteger(handle, MwNheight);
		}

		MwVaApply(ws[0],
			  MwNx, (MwGetInteger(handle, MwNwidth) - pw) / 2,
			  MwNy, (MwGetInteger(handle, MwNheight) - ph) / 2,
			  MwNwidth, pw,
			  MwNheight, ph,
			  NULL);

		free(ws);
	}
}

static void resize_frame(MwWidget handle, void* user, void* call) {
	MwWidget* ws = MwGetChildren(handle);
	if(ws != NULL) {
		int w = MwGetInteger(handle, MwNwidth);
		int h = MwGetInteger(handle, MwNheight);

		if(ws[0] != NULL) {
			MwVaApply(ws[0],
				  MwNx, MwDefaultBorderWidth(handle) + PaddingContent,
				  MwNy, MwDefaultBorderWidth(handle) + PaddingContent,
				  MwNwidth, w - (MwDefaultBorderWidth(handle) + PaddingContent) * 2,
				  MwNheight, h - (MwDefaultBorderWidth(handle) + PaddingContent) * 2,
				  NULL);
		}
		free(ws);
	}
}

static MwWidget frame(const char* name, int width, int height, MwClass cl, ...) {
	MwWidget f     = MwVaCreateWidget(MwFrameClass, "frame", boxes[row], 0, 0, 0, 0,
					  MwNhasBorder, 1,
					  MwNinverted, 1,
					  NULL);
	MwWidget box   = MwVaCreateWidget(MwBoxClass, "box", f, 0, 0, 0, 0,
					  MwNorientation, MwVERTICAL,
					  NULL);
	MwWidget label = MwVaCreateWidget(MwLabelClass, "label", box, 0, 0, 0, 0,
					  MwNtext, name,
					  MwNalignment, MwALIGNMENT_BEGINNING,
					  MwNfixedSize, 20,
					  NULL);
	va_list	 va;

	if(cl != NULL) {
		MwWidget frame = MwCreateWidget(MwFrameClass, "frame", box, 0, 0, 0, 0);
		MwWidget w;

		va_start(va, cl);
		w = MwVaListCreateWidget(cl, "widget", frame, 0, 0, 0, 0, va);
		va_end(va);

		MwVaApply(w,
			  "IpWidth", width,
			  "IpHeight", height,
			  NULL);

		MwAddUserHandler(frame, MwNresizeHandler, resize_content, NULL);

		MwVaApply(f,
			  "VhandledWidget", w,
			  NULL);
	}

	MwAddUserHandler(f, MwNresizeHandler, resize_frame, NULL);

	resize_frame(f, NULL, NULL);

	return f;
}

static MwWidget child(MwWidget w) {
	return MwGetVoid(w, "VhandledWidget");
}

static void menu_handler(MwWidget handle, void* user, void* call) {
	if(call == e) MwDestroyWidget(window);
}

int main() {
	int	 i;
	MwWidget f, w;
	int	 index;
	MwMenu	 m, m2;
	void*	 v;

	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "window", NULL, MwDEFAULT, MwDEFAULT, 800, 800,
				  MwNtitle, "Milsko Periodic Table",
				  NULL);

	menu = MwCreateWidget(MwMenuClass, "menu", window, 0, 0, 0, 0);

	m = MwMenuAdd(menu, NULL, "File");
	MwMenuAdd(menu, m, "New");
	MwMenuAdd(menu, m, "Open");
	MwMenuAdd(menu, m, "Save");
	MwMenuAdd(menu, m, "Save As");
	MwMenuAdd(menu, m, "Print");
	e = MwMenuAdd(menu, m, "Exit");

	m = MwMenuAdd(menu, NULL, "Edit");
	MwMenuAdd(menu, m, "Undo");
	MwMenuAdd(menu, m, "----");
	MwMenuAdd(menu, m, "Cut");
	MwMenuAdd(menu, m, "Copy");
	MwMenuAdd(menu, m, "Paste");
	MwMenuAdd(menu, m, "----");
	MwMenuAdd(menu, m, "Clear");
	MwMenuAdd(menu, m, "Delete");

	m = MwMenuAdd(menu, NULL, "View");
	MwMenuAdd(menu, m, "Stack");
	MwMenuAdd(menu, m, "Tile");
	MwMenuAdd(menu, m, "----");
	MwMenuAdd(menu, m, "Day");
	MwMenuAdd(menu, m, "Week");
	MwMenuAdd(menu, m, "Month");
	MwMenuAdd(menu, m, "Year");

	m  = MwMenuAdd(menu, NULL, "Options");
	m2 = MwMenuAdd(menu, m, "Font");
	MwMenuAdd(menu, m2, "Small");
	MwMenuAdd(menu, m2, "Medium");
	MwMenuAdd(menu, m2, "Large");
	m2 = MwMenuAdd(menu, m, "Direction");
	MwMenuAdd(menu, m2, "Up");
	MwMenuAdd(menu, m2, "Down");
	MwMenuAdd(menu, m2, "Left");
	MwMenuAdd(menu, m2, "Right");
	MwMenuAdd(menu, m, "Case Sensitive");
	MwMenuAdd(menu, m, "Word Wrap");

	m = MwMenuAdd(menu, NULL, "?Help");
	MwMenuAdd(menu, m, "On Context");
	MwMenuAdd(menu, m, "On Window");
	MwMenuAdd(menu, m, "Tutorial");
	MwMenuAdd(menu, m, "On Version");

	MwAddUserHandler(menu, MwNmenuHandler, menu_handler, NULL);

	box = MwVaCreateWidget(MwBoxClass, "box", window, 0, 0, 0, 0,
			       MwNorientation, MwVERTICAL,
			       NULL);

	newrow();

	f = frame("Button", -PaddingContent, -PaddingContent, MwButtonClass,
		  MwNtext, "Press me",
		  NULL);
	add(f);

	add(MwVaCreateWidget(MwLabelClass, "label", boxes[row], 0, 0, 0, 0,
			     MwNbold, 1,
			     MwNtext, "The Periodic Table of Milsko Widgets",
			     MwNratio, Columns - 2,
			     NULL));

	f = frame("ComboBox", -PaddingContent, 24, MwComboBoxClass, NULL);
	w = child(f);
	MwComboBoxAdd(w, -1, "Hello!");
	add(f);

	f = frame("Entry", -PaddingContent, 24, MwEntryClass, NULL);
	add(f);

	f = frame("Label", -PaddingContent, -PaddingContent, MwLabelClass,
		  MwNtext, "Epic text",
		  NULL);
	add(f);

	f     = frame("ListBox", -PaddingContent, -PaddingContent, MwListBoxClass,
		      MwNhasHeading, 1,
		      NULL);
	w     = child(f);
	index = MwListBoxInsert(w, -1, 0, "Epic title...");
	index = MwListBoxInsert(w, -1, 0, "Hello");
	add(f);

	f = frame("NumberEntry", -PaddingContent, 24, MwNumberEntryClass, NULL);
	add(f);

	f = frame("ProgressBar", -PaddingContent, 24, MwProgressBarClass,
		  MwNvalue, 25,
		  NULL);
	add(f);

	f = frame("ScrollBar", -PaddingContent, 16, MwScrollBarClass,
		  MwNorientation, MwHORIZONTAL,
		  NULL);
	add(f);

	f = frame("Separator", -PaddingContent, -PaddingContent, MwSeparatorClass, NULL);
	add(f);

	f = frame("TreeView", -PaddingContent, -PaddingContent, MwTreeViewClass, NULL);
	w = child(f);
	v = MwTreeViewAdd(w, NULL, NULL, "abc");
	v = MwTreeViewAdd(w, v, NULL, "def");
	v = MwTreeViewAdd(w, v, NULL, "ghi");
	add(f);

	if(n != 0) newrow();
	if(n == 0) MwDestroyWidget(boxes[row]);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
