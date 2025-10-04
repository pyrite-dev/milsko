/* $Id$ */

#include <Mw/Milsko.h>

int main(){
	MwWidget window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 500, 500,
		MwNtitle, "main",
	NULL);
	int i;

	for(i = 0; i < 500 / 16; i++){
		MwVaCreateWidget(MwScrollBarClass, "scroll", window, 16 * i, 0, 16, 500,
			MwNorientation, MwVERTICAL,
		NULL);
	}

	MwLoop(window);
}
