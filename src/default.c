/* $Id$ */
#include <Mw/Milsko.h>

const char* MwDefaultBackground = "#ddd";
const char* MwDefaultForeground = "#000";

int MwGetDefaultBorderWidth(MwWidget handle) {
	if(MwGetInteger(handle, MwNmodernLook)) {
		return 1;
	} else {
		return 2;
	}
}
