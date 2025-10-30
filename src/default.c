/* $Id$ */
#include <Mw/Milsko.h>

const char* MwDefaultBackground = "#ddd";
const char* MwDefaultForeground = "#000";

const char* MwDefaultDarkBackground = "#333";
const char* MwDefaultDarkForeground = "#ddd";

int MwDefaultBorderWidth(MwWidget handle) {
	if(MwGetInteger(handle, MwNmodernLook)) {
		return 1;
	} else {
		return 2;
	}
}
