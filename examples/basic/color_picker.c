/* $Id$ */

#include "Mw/Core.h"
#include <Mw/Milsko.h>
#include <Mw/LowLevelMath.h>

int main() {
	MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT,
				      MwDEFAULT, 640, 480, MwNtitle, "test", NULL);

	MwWidget cpicker = MwColorPicker(w, "cpicker");

	MwLoop(w);
}
