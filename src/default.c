#include <Mw/Milsko.h>

#define USE_CATPPUCCIN_DARK

#ifdef USE_CATPPUCCIN_LIGHT
const char* MwDefaultBackground	     = "#eff1f5";
const char* MwDefaultForeground	     = "#4c4f69";
const char* MwDefaultSubBackground   = "#9ca0b0";
const char* MwDefaultSubForeground   = "#6c6f85";
const char* MwDefaultTitleBackground = "#bcc0cc";
const char* MwDefaultTitleForeground = "#5c5f77";
#else
const char* MwDefaultBackground	     = "#d2d2d2";
const char* MwDefaultForeground	     = "#000";
const char* MwDefaultSubBackground   = "#fff";
const char* MwDefaultSubForeground   = "#000";
const char* MwDefaultTitleBackground = "#008";
const char* MwDefaultTitleForeground = "#fff";
#endif

#ifdef USE_CATPPUCCIN_DARK
const char* MwDefaultDarkBackground	 = "#1e1e2e";
const char* MwDefaultDarkForeground	 = "#cdd6f4";
const char* MwDefaultDarkSubBackground	 = "#313244";
const char* MwDefaultDarkSubForeground	 = "#a6adc8";
const char* MwDefaultDarkTitleBackground = "#45475a";
const char* MwDefaultDarkTitleForeground = "#bac2de";
#else
const char* MwDefaultDarkBackground	 = "#333";
const char* MwDefaultDarkForeground	 = "#ddd";
const char* MwDefaultDarkSubBackground	 = "#333";
const char* MwDefaultDarkSubForeground	 = "#ddd";
const char* MwDefaultDarkTitleBackground = "#008";
const char* MwDefaultDarkTitleForeground = "#fff";
#endif

const int MwDefaultShadow = -32;

int MwDefaultBorderWidth(MwWidget handle) {
	int bw = MwGetInteger(handle, MwNborderWidth);

	if(bw != MwDEFAULT) return bw;

	if(MwGetInteger(handle, MwNmodernLook)) {
		return 2;
	} else {
		return 2;
	}
}

int MwDefaultThinBorderWidth(MwWidget handle) {
	int bw = MwGetInteger(handle, MwNborderWidth);

	if(bw != MwDEFAULT) return bw;

	if(MwGetInteger(handle, MwNmodernLook)) {
		return 1;
	} else {
		return 2;
	}
}
