#include <Mw/Milsko.h>

const char* MwDefaultBackground	   = "#d2d2d2";
const char* MwDefaultForeground	   = "#000";
const char* MwDefaultSubBackground = "#fff";
const char* MwDefaultSubForeground = "#000";

const char* MwDefaultDarkBackground    = "#333";
const char* MwDefaultDarkForeground    = "#ddd";
const char* MwDefaultDarkSubBackground = "#333";
const char* MwDefaultDarkSubForeground = "#ddd";

int MwDefaultBorderWidth(MwWidget handle) {
	if(MwGetInteger(handle, MwNmodernLook)) {
		return 1;
	} else {
		return 2;
	}
}
