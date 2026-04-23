#include <Mw/Milsko.h>

#define WIDGET_AMOUNT 16

int main() {
	MwWidget window, widget;

	MwLibraryInit();

	window = MwCreateWidget(MwWindowClass, "window", NULL, 0, 0, 400, 400);

	while(MwPending(window)) {
		int	r = rand() % WIDGET_AMOUNT;
		MwClass cls;
		switch(r) {
		case 0:
			cls = MwBoxClass;
			break;
		case 1:
			cls = MwButtonClass;
			break;
		case 2:
			cls = MwCheckBoxClass;
			break;
		case 3:
			cls = MwComboBoxClass;
			break;
		case 4:
			cls = MwEntryClass;
			break;
		case 5:
			cls = MwFrameClass;
			break;
		case 6:
			cls = MwImageClass;
			break;
		case 7:
			cls = MwLabelClass;
			break;
		case 8:
			cls = MwListBoxClass;
			break;
		case 9:
			cls = MwMenuClass;
			break;
		case 10:
			cls = MwNumberEntryClass;
			break;
		case 11:
			cls = MwProgressBarClass;
			break;
		case 12:
			cls = MwRadioBoxClass;
			break;
		case 13:
			cls = MwScrollBarClass;
			break;
		case 14:
			cls = MwSeparatorClass;
			break;

		case 15:
			cls = MwTreeViewClass;
			break;
		case 16:
			cls = MwViewportClass;
			break;
		default:
			cls = NULL;
			break;
		}
		printf("%d\n", r);
		if(cls) {
			widget = MwCreateWidget(cls, "Cls", window, 0, 0, 100, 100);
		}

		MwStep(window);
		if(cls) {
			MwDestroyWidget(widget);
		}
	}
}
