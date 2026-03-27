#include <Mw/Milsko.h>

#define Columns 4
#define Padding 20

MwWidget window, menu, box;
MwWidget boxes[32];
int n = 0, row = -1;

static void resize(MwWidget handle, void* user, void* call){
	int w = MwGetInteger(window, MwNwidth);
	int h = MwGetInteger(window, MwNheight);

	MwVaApply(box,
		MwNx, Padding,
		MwNy, MwGetInteger(menu, MwNheight) + Padding,
		MwNwidth, w - Padding * 2,
		MwNheight, h - MwGetInteger(menu, MwNheight) - Padding * 2,
	NULL);
}

static void newrow(void){
	if(row >= 0){
		int i;

		for(i = n; i < Columns; i++) MwCreateWidget(MwFrameClass, "frame", boxes[row], 0, 0, 0, 0);
	}

	boxes[++row] = MwCreateWidget(MwBoxClass, "box", box, 0, 0, 0, 0);

	n = 0;
}

static void add(MwWidget w){
	int r = MwGetInteger(w, MwNratio);

	if(r == MwDEFAULT) r = 1;

	n += r;

	if(n == Columns){
		newrow();
	}
}

static MwWidget frame(const char* name, MwWidget w){
	MwWidget f = MwVaCreateWidget(MwFrameClass, "frame", boxes[row], 0, 0, 0, 0,
		MwNhasBorder, 1,
		MwNinverted, 1,
	NULL);

	return f;
}

int main(){
	int i;

	MwLibraryInit();
	
	window = MwVaCreateWidget(MwWindowClass, "window", NULL, MwDEFAULT, MwDEFAULT, 800, 800,
		MwNtitle, "Milsko Periodic Table",
	NULL);

	menu = MwCreateWidget(MwMenuClass, "menu", window, 0, 0, 0, 0);
	
	box = MwVaCreateWidget(MwBoxClass, "box", window, 0, 0, 0, 0,
		MwNorientation, MwVERTICAL,
	NULL);

	newrow();

	add(frame("Separator", NULL));

	add(MwVaCreateWidget(MwLabelClass, "label", boxes[row], 0, 0, 0, 0,
		MwNbold, 1,
		MwNtext, "The Periodic Table of Milsko Widgets",
		MwNratio, Columns - 2,
	NULL));

	add(frame("TODO", NULL));

	for(i = 0; i < 12; i++) add(frame("TODO", NULL));

	if(n == 0){
		MwDestroyWidget(boxes[row]);
	}

	resize(window, NULL, NULL);

	MwLoop(window);
}
