#include <Mw/Milsko.h>

#include <stdarg.h>

#define Columns 4
#define Padding 20
#define PaddingContent 2

MwWidget window, menu, box;
MwWidget boxes[32];
int	 n = 0, row = -1;

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

int main() {
	int	 i;
	MwWidget f, w;

	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "window", NULL, MwDEFAULT, MwDEFAULT, 800, 800,
				  MwNtitle, "Milsko Periodic Table",
				  NULL);

	menu = MwCreateWidget(MwMenuClass, "menu", window, 0, 0, 0, 0);

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

	f = frame("Separator", -PaddingContent, -PaddingContent, MwSeparatorClass, NULL);
	add(f);

	if(n != 0) newrow();
	if(n == 0) MwDestroyWidget(boxes[row]);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);

	resize(window, NULL, NULL);

	MwLoop(window);
}
