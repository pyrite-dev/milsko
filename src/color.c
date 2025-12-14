#include <Mw/Milsko.h>

MwLLColor MwParseColorName(MwWidget handle, const char* color) {
	MwRGB rgb;
	MwParseColorNameNoAllocate(color, &rgb);
	return MwLLAllocColor(handle->lowlevel, rgb.red, rgb.green, rgb.blue);
}

void MwParseColorNameNoAllocate(const char* color, MwRGB* rgb) {
	if(strcmp(color, "snow") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "ghost white") == 0) {
		rgb->red   = 248;
		rgb->green = 248;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "GhostWhite") == 0) {
		rgb->red   = 248;
		rgb->green = 248;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "white smoke") == 0) {
		rgb->red   = 245;
		rgb->green = 245;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "WhiteSmoke") == 0) {
		rgb->red   = 245;
		rgb->green = 245;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "gainsboro") == 0) {
		rgb->red   = 220;
		rgb->green = 220;
		rgb->green = 220;
		return;
	}
	if(strcmp(color, "floral white") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "FloralWhite") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "old lace") == 0) {
		rgb->red   = 253;
		rgb->green = 245;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "OldLace") == 0) {
		rgb->red   = 253;
		rgb->green = 245;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "linen") == 0) {
		rgb->red   = 250;
		rgb->green = 240;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "antique white") == 0) {
		rgb->red   = 250;
		rgb->green = 235;
		rgb->green = 215;
		return;
	}
	if(strcmp(color, "AntiqueWhite") == 0) {
		rgb->red   = 250;
		rgb->green = 235;
		rgb->green = 215;
		return;
	}
	if(strcmp(color, "papaya whip") == 0) {
		rgb->red   = 255;
		rgb->green = 239;
		rgb->green = 213;
		return;
	}
	if(strcmp(color, "PapayaWhip") == 0) {
		rgb->red   = 255;
		rgb->green = 239;
		rgb->green = 213;
		return;
	}
	if(strcmp(color, "blanched almond") == 0) {
		rgb->red   = 255;
		rgb->green = 235;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "BlanchedAlmond") == 0) {
		rgb->red   = 255;
		rgb->green = 235;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "bisque") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 196;
		return;
	}
	if(strcmp(color, "peach puff") == 0) {
		rgb->red   = 255;
		rgb->green = 218;
		rgb->green = 185;
		return;
	}
	if(strcmp(color, "PeachPuff") == 0) {
		rgb->red   = 255;
		rgb->green = 218;
		rgb->green = 185;
		return;
	}
	if(strcmp(color, "navajo white") == 0) {
		rgb->red   = 255;
		rgb->green = 222;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "NavajoWhite") == 0) {
		rgb->red   = 255;
		rgb->green = 222;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "moccasin") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 181;
		return;
	}
	if(strcmp(color, "cornsilk") == 0) {
		rgb->red   = 255;
		rgb->green = 248;
		rgb->green = 220;
		return;
	}
	if(strcmp(color, "ivory") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "lemon chiffon") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LemonChiffon") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "seashell") == 0) {
		rgb->red   = 255;
		rgb->green = 245;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "honeydew") == 0) {
		rgb->red   = 240;
		rgb->green = 255;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "mint cream") == 0) {
		rgb->red   = 245;
		rgb->green = 255;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "MintCream") == 0) {
		rgb->red   = 245;
		rgb->green = 255;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "azure") == 0) {
		rgb->red   = 240;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "alice blue") == 0) {
		rgb->red   = 240;
		rgb->green = 248;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "AliceBlue") == 0) {
		rgb->red   = 240;
		rgb->green = 248;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "lavender") == 0) {
		rgb->red   = 230;
		rgb->green = 230;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "lavender blush") == 0) {
		rgb->red   = 255;
		rgb->green = 240;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "LavenderBlush") == 0) {
		rgb->red   = 255;
		rgb->green = 240;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "misty rose") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 225;
		return;
	}
	if(strcmp(color, "MistyRose") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 225;
		return;
	}
	if(strcmp(color, "white") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "black") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "dark slate gray") == 0) {
		rgb->red   = 47;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "DarkSlateGray") == 0) {
		rgb->red   = 47;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "dark slate grey") == 0) {
		rgb->red   = 47;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "DarkSlateGrey") == 0) {
		rgb->red   = 47;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "dim gray") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "DimGray") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "dim grey") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "DimGrey") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "slate gray") == 0) {
		rgb->red   = 112;
		rgb->green = 128;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "SlateGray") == 0) {
		rgb->red   = 112;
		rgb->green = 128;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "slate grey") == 0) {
		rgb->red   = 112;
		rgb->green = 128;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "SlateGrey") == 0) {
		rgb->red   = 112;
		rgb->green = 128;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "light slate gray") == 0) {
		rgb->red   = 119;
		rgb->green = 136;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "LightSlateGray") == 0) {
		rgb->red   = 119;
		rgb->green = 136;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "light slate grey") == 0) {
		rgb->red   = 119;
		rgb->green = 136;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "LightSlateGrey") == 0) {
		rgb->red   = 119;
		rgb->green = 136;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "gray") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "grey") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "x11 gray") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "X11Gray") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "x11 grey") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "X11Grey") == 0) {
		rgb->red   = 190;
		rgb->green = 190;
		rgb->green = 190;
		return;
	}
	if(strcmp(color, "web gray") == 0) {
		rgb->red   = 128;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "WebGray") == 0) {
		rgb->red   = 128;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "web grey") == 0) {
		rgb->red   = 128;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "WebGrey") == 0) {
		rgb->red   = 128;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "light grey") == 0) {
		rgb->red   = 211;
		rgb->green = 211;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "LightGrey") == 0) {
		rgb->red   = 211;
		rgb->green = 211;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "light gray") == 0) {
		rgb->red   = 211;
		rgb->green = 211;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "LightGray") == 0) {
		rgb->red   = 211;
		rgb->green = 211;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "midnight blue") == 0) {
		rgb->red   = 25;
		rgb->green = 25;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "MidnightBlue") == 0) {
		rgb->red   = 25;
		rgb->green = 25;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "navy") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "navy blue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "NavyBlue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "cornflower blue") == 0) {
		rgb->red   = 100;
		rgb->green = 149;
		rgb->green = 237;
		return;
	}
	if(strcmp(color, "CornflowerBlue") == 0) {
		rgb->red   = 100;
		rgb->green = 149;
		rgb->green = 237;
		return;
	}
	if(strcmp(color, "dark slate blue") == 0) {
		rgb->red   = 72;
		rgb->green = 61;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkSlateBlue") == 0) {
		rgb->red   = 72;
		rgb->green = 61;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "slate blue") == 0) {
		rgb->red   = 106;
		rgb->green = 90;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "SlateBlue") == 0) {
		rgb->red   = 106;
		rgb->green = 90;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "medium slate blue") == 0) {
		rgb->red   = 123;
		rgb->green = 104;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "MediumSlateBlue") == 0) {
		rgb->red   = 123;
		rgb->green = 104;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "light slate blue") == 0) {
		rgb->red   = 132;
		rgb->green = 112;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightSlateBlue") == 0) {
		rgb->red   = 132;
		rgb->green = 112;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "medium blue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "MediumBlue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "royal blue") == 0) {
		rgb->red   = 65;
		rgb->green = 105;
		rgb->green = 225;
		return;
	}
	if(strcmp(color, "RoyalBlue") == 0) {
		rgb->red   = 65;
		rgb->green = 105;
		rgb->green = 225;
		return;
	}
	if(strcmp(color, "blue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "dodger blue") == 0) {
		rgb->red   = 30;
		rgb->green = 144;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DodgerBlue") == 0) {
		rgb->red   = 30;
		rgb->green = 144;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "deep sky blue") == 0) {
		rgb->red   = 0;
		rgb->green = 191;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DeepSkyBlue") == 0) {
		rgb->red   = 0;
		rgb->green = 191;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "sky blue") == 0) {
		rgb->red   = 135;
		rgb->green = 206;
		rgb->green = 235;
		return;
	}
	if(strcmp(color, "SkyBlue") == 0) {
		rgb->red   = 135;
		rgb->green = 206;
		rgb->green = 235;
		return;
	}
	if(strcmp(color, "light sky blue") == 0) {
		rgb->red   = 135;
		rgb->green = 206;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "LightSkyBlue") == 0) {
		rgb->red   = 135;
		rgb->green = 206;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "steel blue") == 0) {
		rgb->red   = 70;
		rgb->green = 130;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "SteelBlue") == 0) {
		rgb->red   = 70;
		rgb->green = 130;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "light steel blue") == 0) {
		rgb->red   = 176;
		rgb->green = 196;
		rgb->green = 222;
		return;
	}
	if(strcmp(color, "LightSteelBlue") == 0) {
		rgb->red   = 176;
		rgb->green = 196;
		rgb->green = 222;
		return;
	}
	if(strcmp(color, "light blue") == 0) {
		rgb->red   = 173;
		rgb->green = 216;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "LightBlue") == 0) {
		rgb->red   = 173;
		rgb->green = 216;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "powder blue") == 0) {
		rgb->red   = 176;
		rgb->green = 224;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "PowderBlue") == 0) {
		rgb->red   = 176;
		rgb->green = 224;
		rgb->green = 230;
		return;
	}
	if(strcmp(color, "pale turquoise") == 0) {
		rgb->red   = 175;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "PaleTurquoise") == 0) {
		rgb->red   = 175;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "dark turquoise") == 0) {
		rgb->red   = 0;
		rgb->green = 206;
		rgb->green = 209;
		return;
	}
	if(strcmp(color, "DarkTurquoise") == 0) {
		rgb->red   = 0;
		rgb->green = 206;
		rgb->green = 209;
		return;
	}
	if(strcmp(color, "medium turquoise") == 0) {
		rgb->red   = 72;
		rgb->green = 209;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "MediumTurquoise") == 0) {
		rgb->red   = 72;
		rgb->green = 209;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "turquoise") == 0) {
		rgb->red   = 64;
		rgb->green = 224;
		rgb->green = 208;
		return;
	}
	if(strcmp(color, "cyan") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "aqua") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "light cyan") == 0) {
		rgb->red   = 224;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightCyan") == 0) {
		rgb->red   = 224;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "cadet blue") == 0) {
		rgb->red   = 95;
		rgb->green = 158;
		rgb->green = 160;
		return;
	}
	if(strcmp(color, "CadetBlue") == 0) {
		rgb->red   = 95;
		rgb->green = 158;
		rgb->green = 160;
		return;
	}
	if(strcmp(color, "medium aquamarine") == 0) {
		rgb->red   = 102;
		rgb->green = 205;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "MediumAquamarine") == 0) {
		rgb->red   = 102;
		rgb->green = 205;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "aquamarine") == 0) {
		rgb->red   = 127;
		rgb->green = 255;
		rgb->green = 212;
		return;
	}
	if(strcmp(color, "dark green") == 0) {
		rgb->red   = 0;
		rgb->green = 100;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkGreen") == 0) {
		rgb->red   = 0;
		rgb->green = 100;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "dark olive green") == 0) {
		rgb->red   = 85;
		rgb->green = 107;
		rgb->green = 47;
		return;
	}
	if(strcmp(color, "DarkOliveGreen") == 0) {
		rgb->red   = 85;
		rgb->green = 107;
		rgb->green = 47;
		return;
	}
	if(strcmp(color, "dark sea green") == 0) {
		rgb->red   = 143;
		rgb->green = 188;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "DarkSeaGreen") == 0) {
		rgb->red   = 143;
		rgb->green = 188;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "sea green") == 0) {
		rgb->red   = 46;
		rgb->green = 139;
		rgb->green = 87;
		return;
	}
	if(strcmp(color, "SeaGreen") == 0) {
		rgb->red   = 46;
		rgb->green = 139;
		rgb->green = 87;
		return;
	}
	if(strcmp(color, "medium sea green") == 0) {
		rgb->red   = 60;
		rgb->green = 179;
		rgb->green = 113;
		return;
	}
	if(strcmp(color, "MediumSeaGreen") == 0) {
		rgb->red   = 60;
		rgb->green = 179;
		rgb->green = 113;
		return;
	}
	if(strcmp(color, "light sea green") == 0) {
		rgb->red   = 32;
		rgb->green = 178;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "LightSeaGreen") == 0) {
		rgb->red   = 32;
		rgb->green = 178;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "pale green") == 0) {
		rgb->red   = 152;
		rgb->green = 251;
		rgb->green = 152;
		return;
	}
	if(strcmp(color, "PaleGreen") == 0) {
		rgb->red   = 152;
		rgb->green = 251;
		rgb->green = 152;
		return;
	}
	if(strcmp(color, "spring green") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 127;
		return;
	}
	if(strcmp(color, "SpringGreen") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 127;
		return;
	}
	if(strcmp(color, "lawn green") == 0) {
		rgb->red   = 124;
		rgb->green = 252;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "LawnGreen") == 0) {
		rgb->red   = 124;
		rgb->green = 252;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "green") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "lime") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "x11 green") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "X11Green") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "web green") == 0) {
		rgb->red   = 0;
		rgb->green = 128;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "WebGreen") == 0) {
		rgb->red   = 0;
		rgb->green = 128;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "chartreuse") == 0) {
		rgb->red   = 127;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "medium spring green") == 0) {
		rgb->red   = 0;
		rgb->green = 250;
		rgb->green = 154;
		return;
	}
	if(strcmp(color, "MediumSpringGreen") == 0) {
		rgb->red   = 0;
		rgb->green = 250;
		rgb->green = 154;
		return;
	}
	if(strcmp(color, "green yellow") == 0) {
		rgb->red   = 173;
		rgb->green = 255;
		rgb->green = 47;
		return;
	}
	if(strcmp(color, "GreenYellow") == 0) {
		rgb->red   = 173;
		rgb->green = 255;
		rgb->green = 47;
		return;
	}
	if(strcmp(color, "lime green") == 0) {
		rgb->red   = 50;
		rgb->green = 205;
		rgb->green = 50;
		return;
	}
	if(strcmp(color, "LimeGreen") == 0) {
		rgb->red   = 50;
		rgb->green = 205;
		rgb->green = 50;
		return;
	}
	if(strcmp(color, "yellow green") == 0) {
		rgb->red   = 154;
		rgb->green = 205;
		rgb->green = 50;
		return;
	}
	if(strcmp(color, "YellowGreen") == 0) {
		rgb->red   = 154;
		rgb->green = 205;
		rgb->green = 50;
		return;
	}
	if(strcmp(color, "forest green") == 0) {
		rgb->red   = 34;
		rgb->green = 139;
		rgb->green = 34;
		return;
	}
	if(strcmp(color, "ForestGreen") == 0) {
		rgb->red   = 34;
		rgb->green = 139;
		rgb->green = 34;
		return;
	}
	if(strcmp(color, "olive drab") == 0) {
		rgb->red   = 107;
		rgb->green = 142;
		rgb->green = 35;
		return;
	}
	if(strcmp(color, "OliveDrab") == 0) {
		rgb->red   = 107;
		rgb->green = 142;
		rgb->green = 35;
		return;
	}
	if(strcmp(color, "dark khaki") == 0) {
		rgb->red   = 189;
		rgb->green = 183;
		rgb->green = 107;
		return;
	}
	if(strcmp(color, "DarkKhaki") == 0) {
		rgb->red   = 189;
		rgb->green = 183;
		rgb->green = 107;
		return;
	}
	if(strcmp(color, "khaki") == 0) {
		rgb->red   = 240;
		rgb->green = 230;
		rgb->green = 140;
		return;
	}
	if(strcmp(color, "pale goldenrod") == 0) {
		rgb->red   = 238;
		rgb->green = 232;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "PaleGoldenrod") == 0) {
		rgb->red   = 238;
		rgb->green = 232;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "light goldenrod yellow") == 0) {
		rgb->red   = 250;
		rgb->green = 250;
		rgb->green = 210;
		return;
	}
	if(strcmp(color, "LightGoldenrodYellow") == 0) {
		rgb->red   = 250;
		rgb->green = 250;
		rgb->green = 210;
		return;
	}
	if(strcmp(color, "light yellow") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "LightYellow") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "yellow") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gold") == 0) {
		rgb->red   = 255;
		rgb->green = 215;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "light goldenrod") == 0) {
		rgb->red   = 238;
		rgb->green = 221;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "LightGoldenrod") == 0) {
		rgb->red   = 238;
		rgb->green = 221;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "goldenrod") == 0) {
		rgb->red   = 218;
		rgb->green = 165;
		rgb->green = 32;
		return;
	}
	if(strcmp(color, "dark goldenrod") == 0) {
		rgb->red   = 184;
		rgb->green = 134;
		rgb->green = 11;
		return;
	}
	if(strcmp(color, "DarkGoldenrod") == 0) {
		rgb->red   = 184;
		rgb->green = 134;
		rgb->green = 11;
		return;
	}
	if(strcmp(color, "rosy brown") == 0) {
		rgb->red   = 188;
		rgb->green = 143;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "RosyBrown") == 0) {
		rgb->red   = 188;
		rgb->green = 143;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "indian red") == 0) {
		rgb->red   = 205;
		rgb->green = 92;
		rgb->green = 92;
		return;
	}
	if(strcmp(color, "IndianRed") == 0) {
		rgb->red   = 205;
		rgb->green = 92;
		rgb->green = 92;
		return;
	}
	if(strcmp(color, "saddle brown") == 0) {
		rgb->red   = 139;
		rgb->green = 69;
		rgb->green = 19;
		return;
	}
	if(strcmp(color, "SaddleBrown") == 0) {
		rgb->red   = 139;
		rgb->green = 69;
		rgb->green = 19;
		return;
	}
	if(strcmp(color, "sienna") == 0) {
		rgb->red   = 160;
		rgb->green = 82;
		rgb->green = 45;
		return;
	}
	if(strcmp(color, "peru") == 0) {
		rgb->red   = 205;
		rgb->green = 133;
		rgb->green = 63;
		return;
	}
	if(strcmp(color, "burlywood") == 0) {
		rgb->red   = 222;
		rgb->green = 184;
		rgb->green = 135;
		return;
	}
	if(strcmp(color, "beige") == 0) {
		rgb->red   = 245;
		rgb->green = 245;
		rgb->green = 220;
		return;
	}
	if(strcmp(color, "wheat") == 0) {
		rgb->red   = 245;
		rgb->green = 222;
		rgb->green = 179;
		return;
	}
	if(strcmp(color, "sandy brown") == 0) {
		rgb->red   = 244;
		rgb->green = 164;
		rgb->green = 96;
		return;
	}
	if(strcmp(color, "SandyBrown") == 0) {
		rgb->red   = 244;
		rgb->green = 164;
		rgb->green = 96;
		return;
	}
	if(strcmp(color, "tan") == 0) {
		rgb->red   = 210;
		rgb->green = 180;
		rgb->green = 140;
		return;
	}
	if(strcmp(color, "chocolate") == 0) {
		rgb->red   = 210;
		rgb->green = 105;
		rgb->green = 30;
		return;
	}
	if(strcmp(color, "firebrick") == 0) {
		rgb->red   = 178;
		rgb->green = 34;
		rgb->green = 34;
		return;
	}
	if(strcmp(color, "brown") == 0) {
		rgb->red   = 165;
		rgb->green = 42;
		rgb->green = 42;
		return;
	}
	if(strcmp(color, "dark salmon") == 0) {
		rgb->red   = 233;
		rgb->green = 150;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "DarkSalmon") == 0) {
		rgb->red   = 233;
		rgb->green = 150;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "salmon") == 0) {
		rgb->red   = 250;
		rgb->green = 128;
		rgb->green = 114;
		return;
	}
	if(strcmp(color, "light salmon") == 0) {
		rgb->red   = 255;
		rgb->green = 160;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "LightSalmon") == 0) {
		rgb->red   = 255;
		rgb->green = 160;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "orange") == 0) {
		rgb->red   = 255;
		rgb->green = 165;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "dark orange") == 0) {
		rgb->red   = 255;
		rgb->green = 140;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkOrange") == 0) {
		rgb->red   = 255;
		rgb->green = 140;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "coral") == 0) {
		rgb->red   = 255;
		rgb->green = 127;
		rgb->green = 80;
		return;
	}
	if(strcmp(color, "light coral") == 0) {
		rgb->red   = 240;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "LightCoral") == 0) {
		rgb->red   = 240;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "tomato") == 0) {
		rgb->red   = 255;
		rgb->green = 99;
		rgb->green = 71;
		return;
	}
	if(strcmp(color, "orange red") == 0) {
		rgb->red   = 255;
		rgb->green = 69;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "OrangeRed") == 0) {
		rgb->red   = 255;
		rgb->green = 69;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "red") == 0) {
		rgb->red   = 255;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "hot pink") == 0) {
		rgb->red   = 255;
		rgb->green = 105;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "HotPink") == 0) {
		rgb->red   = 255;
		rgb->green = 105;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "deep pink") == 0) {
		rgb->red   = 255;
		rgb->green = 20;
		rgb->green = 147;
		return;
	}
	if(strcmp(color, "DeepPink") == 0) {
		rgb->red   = 255;
		rgb->green = 20;
		rgb->green = 147;
		return;
	}
	if(strcmp(color, "pink") == 0) {
		rgb->red   = 255;
		rgb->green = 192;
		rgb->green = 203;
		return;
	}
	if(strcmp(color, "light pink") == 0) {
		rgb->red   = 255;
		rgb->green = 182;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "LightPink") == 0) {
		rgb->red   = 255;
		rgb->green = 182;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "pale violet red") == 0) {
		rgb->red   = 219;
		rgb->green = 112;
		rgb->green = 147;
		return;
	}
	if(strcmp(color, "PaleVioletRed") == 0) {
		rgb->red   = 219;
		rgb->green = 112;
		rgb->green = 147;
		return;
	}
	if(strcmp(color, "maroon") == 0) {
		rgb->red   = 176;
		rgb->green = 48;
		rgb->green = 96;
		return;
	}
	if(strcmp(color, "x11 maroon") == 0) {
		rgb->red   = 176;
		rgb->green = 48;
		rgb->green = 96;
		return;
	}
	if(strcmp(color, "X11Maroon") == 0) {
		rgb->red   = 176;
		rgb->green = 48;
		rgb->green = 96;
		return;
	}
	if(strcmp(color, "web maroon") == 0) {
		rgb->red   = 128;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "WebMaroon") == 0) {
		rgb->red   = 128;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "medium violet red") == 0) {
		rgb->red   = 199;
		rgb->green = 21;
		rgb->green = 133;
		return;
	}
	if(strcmp(color, "MediumVioletRed") == 0) {
		rgb->red   = 199;
		rgb->green = 21;
		rgb->green = 133;
		return;
	}
	if(strcmp(color, "violet red") == 0) {
		rgb->red   = 208;
		rgb->green = 32;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "VioletRed") == 0) {
		rgb->red   = 208;
		rgb->green = 32;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "magenta") == 0) {
		rgb->red   = 255;
		rgb->green = 0;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "fuchsia") == 0) {
		rgb->red   = 255;
		rgb->green = 0;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "violet") == 0) {
		rgb->red   = 238;
		rgb->green = 130;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "plum") == 0) {
		rgb->red   = 221;
		rgb->green = 160;
		rgb->green = 221;
		return;
	}
	if(strcmp(color, "orchid") == 0) {
		rgb->red   = 218;
		rgb->green = 112;
		rgb->green = 214;
		return;
	}
	if(strcmp(color, "medium orchid") == 0) {
		rgb->red   = 186;
		rgb->green = 85;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "MediumOrchid") == 0) {
		rgb->red   = 186;
		rgb->green = 85;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "dark orchid") == 0) {
		rgb->red   = 153;
		rgb->green = 50;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "DarkOrchid") == 0) {
		rgb->red   = 153;
		rgb->green = 50;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "dark violet") == 0) {
		rgb->red   = 148;
		rgb->green = 0;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "DarkViolet") == 0) {
		rgb->red   = 148;
		rgb->green = 0;
		rgb->green = 211;
		return;
	}
	if(strcmp(color, "blue violet") == 0) {
		rgb->red   = 138;
		rgb->green = 43;
		rgb->green = 226;
		return;
	}
	if(strcmp(color, "BlueViolet") == 0) {
		rgb->red   = 138;
		rgb->green = 43;
		rgb->green = 226;
		return;
	}
	if(strcmp(color, "purple") == 0) {
		rgb->red   = 160;
		rgb->green = 32;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "x11 purple") == 0) {
		rgb->red   = 160;
		rgb->green = 32;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "X11Purple") == 0) {
		rgb->red   = 160;
		rgb->green = 32;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "web purple") == 0) {
		rgb->red   = 128;
		rgb->green = 0;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "WebPurple") == 0) {
		rgb->red   = 128;
		rgb->green = 0;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "medium purple") == 0) {
		rgb->red   = 147;
		rgb->green = 112;
		rgb->green = 219;
		return;
	}
	if(strcmp(color, "MediumPurple") == 0) {
		rgb->red   = 147;
		rgb->green = 112;
		rgb->green = 219;
		return;
	}
	if(strcmp(color, "thistle") == 0) {
		rgb->red   = 216;
		rgb->green = 191;
		rgb->green = 216;
		return;
	}
	if(strcmp(color, "snow1") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "snow2") == 0) {
		rgb->red   = 238;
		rgb->green = 233;
		rgb->green = 233;
		return;
	}
	if(strcmp(color, "snow3") == 0) {
		rgb->red   = 205;
		rgb->green = 201;
		rgb->green = 201;
		return;
	}
	if(strcmp(color, "snow4") == 0) {
		rgb->red   = 139;
		rgb->green = 137;
		rgb->green = 137;
		return;
	}
	if(strcmp(color, "seashell1") == 0) {
		rgb->red   = 255;
		rgb->green = 245;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "seashell2") == 0) {
		rgb->red   = 238;
		rgb->green = 229;
		rgb->green = 222;
		return;
	}
	if(strcmp(color, "seashell3") == 0) {
		rgb->red   = 205;
		rgb->green = 197;
		rgb->green = 191;
		return;
	}
	if(strcmp(color, "seashell4") == 0) {
		rgb->red   = 139;
		rgb->green = 134;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "AntiqueWhite1") == 0) {
		rgb->red   = 255;
		rgb->green = 239;
		rgb->green = 219;
		return;
	}
	if(strcmp(color, "AntiqueWhite2") == 0) {
		rgb->red   = 238;
		rgb->green = 223;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "AntiqueWhite3") == 0) {
		rgb->red   = 205;
		rgb->green = 192;
		rgb->green = 176;
		return;
	}
	if(strcmp(color, "AntiqueWhite4") == 0) {
		rgb->red   = 139;
		rgb->green = 131;
		rgb->green = 120;
		return;
	}
	if(strcmp(color, "bisque1") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 196;
		return;
	}
	if(strcmp(color, "bisque2") == 0) {
		rgb->red   = 238;
		rgb->green = 213;
		rgb->green = 183;
		return;
	}
	if(strcmp(color, "bisque3") == 0) {
		rgb->red   = 205;
		rgb->green = 183;
		rgb->green = 158;
		return;
	}
	if(strcmp(color, "bisque4") == 0) {
		rgb->red   = 139;
		rgb->green = 125;
		rgb->green = 107;
		return;
	}
	if(strcmp(color, "PeachPuff1") == 0) {
		rgb->red   = 255;
		rgb->green = 218;
		rgb->green = 185;
		return;
	}
	if(strcmp(color, "PeachPuff2") == 0) {
		rgb->red   = 238;
		rgb->green = 203;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "PeachPuff3") == 0) {
		rgb->red   = 205;
		rgb->green = 175;
		rgb->green = 149;
		return;
	}
	if(strcmp(color, "PeachPuff4") == 0) {
		rgb->red   = 139;
		rgb->green = 119;
		rgb->green = 101;
		return;
	}
	if(strcmp(color, "NavajoWhite1") == 0) {
		rgb->red   = 255;
		rgb->green = 222;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "NavajoWhite2") == 0) {
		rgb->red   = 238;
		rgb->green = 207;
		rgb->green = 161;
		return;
	}
	if(strcmp(color, "NavajoWhite3") == 0) {
		rgb->red   = 205;
		rgb->green = 179;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "NavajoWhite4") == 0) {
		rgb->red   = 139;
		rgb->green = 121;
		rgb->green = 94;
		return;
	}
	if(strcmp(color, "LemonChiffon1") == 0) {
		rgb->red   = 255;
		rgb->green = 250;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LemonChiffon2") == 0) {
		rgb->red   = 238;
		rgb->green = 233;
		rgb->green = 191;
		return;
	}
	if(strcmp(color, "LemonChiffon3") == 0) {
		rgb->red   = 205;
		rgb->green = 201;
		rgb->green = 165;
		return;
	}
	if(strcmp(color, "LemonChiffon4") == 0) {
		rgb->red   = 139;
		rgb->green = 137;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "cornsilk1") == 0) {
		rgb->red   = 255;
		rgb->green = 248;
		rgb->green = 220;
		return;
	}
	if(strcmp(color, "cornsilk2") == 0) {
		rgb->red   = 238;
		rgb->green = 232;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "cornsilk3") == 0) {
		rgb->red   = 205;
		rgb->green = 200;
		rgb->green = 177;
		return;
	}
	if(strcmp(color, "cornsilk4") == 0) {
		rgb->red   = 139;
		rgb->green = 136;
		rgb->green = 120;
		return;
	}
	if(strcmp(color, "ivory1") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "ivory2") == 0) {
		rgb->red   = 238;
		rgb->green = 238;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "ivory3") == 0) {
		rgb->red   = 205;
		rgb->green = 205;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "ivory4") == 0) {
		rgb->red   = 139;
		rgb->green = 139;
		rgb->green = 131;
		return;
	}
	if(strcmp(color, "honeydew1") == 0) {
		rgb->red   = 240;
		rgb->green = 255;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "honeydew2") == 0) {
		rgb->red   = 224;
		rgb->green = 238;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "honeydew3") == 0) {
		rgb->red   = 193;
		rgb->green = 205;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "honeydew4") == 0) {
		rgb->red   = 131;
		rgb->green = 139;
		rgb->green = 131;
		return;
	}
	if(strcmp(color, "LavenderBlush1") == 0) {
		rgb->red   = 255;
		rgb->green = 240;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "LavenderBlush2") == 0) {
		rgb->red   = 238;
		rgb->green = 224;
		rgb->green = 229;
		return;
	}
	if(strcmp(color, "LavenderBlush3") == 0) {
		rgb->red   = 205;
		rgb->green = 193;
		rgb->green = 197;
		return;
	}
	if(strcmp(color, "LavenderBlush4") == 0) {
		rgb->red   = 139;
		rgb->green = 131;
		rgb->green = 134;
		return;
	}
	if(strcmp(color, "MistyRose1") == 0) {
		rgb->red   = 255;
		rgb->green = 228;
		rgb->green = 225;
		return;
	}
	if(strcmp(color, "MistyRose2") == 0) {
		rgb->red   = 238;
		rgb->green = 213;
		rgb->green = 210;
		return;
	}
	if(strcmp(color, "MistyRose3") == 0) {
		rgb->red   = 205;
		rgb->green = 183;
		rgb->green = 181;
		return;
	}
	if(strcmp(color, "MistyRose4") == 0) {
		rgb->red   = 139;
		rgb->green = 125;
		rgb->green = 123;
		return;
	}
	if(strcmp(color, "azure1") == 0) {
		rgb->red   = 240;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "azure2") == 0) {
		rgb->red   = 224;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "azure3") == 0) {
		rgb->red   = 193;
		rgb->green = 205;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "azure4") == 0) {
		rgb->red   = 131;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "SlateBlue1") == 0) {
		rgb->red   = 131;
		rgb->green = 111;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "SlateBlue2") == 0) {
		rgb->red   = 122;
		rgb->green = 103;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "SlateBlue3") == 0) {
		rgb->red   = 105;
		rgb->green = 89;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "SlateBlue4") == 0) {
		rgb->red   = 71;
		rgb->green = 60;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "RoyalBlue1") == 0) {
		rgb->red   = 72;
		rgb->green = 118;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "RoyalBlue2") == 0) {
		rgb->red   = 67;
		rgb->green = 110;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "RoyalBlue3") == 0) {
		rgb->red   = 58;
		rgb->green = 95;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "RoyalBlue4") == 0) {
		rgb->red   = 39;
		rgb->green = 64;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "blue1") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "blue2") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "blue3") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "blue4") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DodgerBlue1") == 0) {
		rgb->red   = 30;
		rgb->green = 144;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DodgerBlue2") == 0) {
		rgb->red   = 28;
		rgb->green = 134;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "DodgerBlue3") == 0) {
		rgb->red   = 24;
		rgb->green = 116;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "DodgerBlue4") == 0) {
		rgb->red   = 16;
		rgb->green = 78;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "SteelBlue1") == 0) {
		rgb->red   = 99;
		rgb->green = 184;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "SteelBlue2") == 0) {
		rgb->red   = 92;
		rgb->green = 172;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "SteelBlue3") == 0) {
		rgb->red   = 79;
		rgb->green = 148;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "SteelBlue4") == 0) {
		rgb->red   = 54;
		rgb->green = 100;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DeepSkyBlue1") == 0) {
		rgb->red   = 0;
		rgb->green = 191;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DeepSkyBlue2") == 0) {
		rgb->red   = 0;
		rgb->green = 178;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "DeepSkyBlue3") == 0) {
		rgb->red   = 0;
		rgb->green = 154;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "DeepSkyBlue4") == 0) {
		rgb->red   = 0;
		rgb->green = 104;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "SkyBlue1") == 0) {
		rgb->red   = 135;
		rgb->green = 206;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "SkyBlue2") == 0) {
		rgb->red   = 126;
		rgb->green = 192;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "SkyBlue3") == 0) {
		rgb->red   = 108;
		rgb->green = 166;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "SkyBlue4") == 0) {
		rgb->red   = 74;
		rgb->green = 112;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "LightSkyBlue1") == 0) {
		rgb->red   = 176;
		rgb->green = 226;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightSkyBlue2") == 0) {
		rgb->red   = 164;
		rgb->green = 211;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "LightSkyBlue3") == 0) {
		rgb->red   = 141;
		rgb->green = 182;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LightSkyBlue4") == 0) {
		rgb->red   = 96;
		rgb->green = 123;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "SlateGray1") == 0) {
		rgb->red   = 198;
		rgb->green = 226;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "SlateGray2") == 0) {
		rgb->red   = 185;
		rgb->green = 211;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "SlateGray3") == 0) {
		rgb->red   = 159;
		rgb->green = 182;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "SlateGray4") == 0) {
		rgb->red   = 108;
		rgb->green = 123;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "LightSteelBlue1") == 0) {
		rgb->red   = 202;
		rgb->green = 225;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightSteelBlue2") == 0) {
		rgb->red   = 188;
		rgb->green = 210;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "LightSteelBlue3") == 0) {
		rgb->red   = 162;
		rgb->green = 181;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LightSteelBlue4") == 0) {
		rgb->red   = 110;
		rgb->green = 123;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "LightBlue1") == 0) {
		rgb->red   = 191;
		rgb->green = 239;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightBlue2") == 0) {
		rgb->red   = 178;
		rgb->green = 223;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "LightBlue3") == 0) {
		rgb->red   = 154;
		rgb->green = 192;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LightBlue4") == 0) {
		rgb->red   = 104;
		rgb->green = 131;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "LightCyan1") == 0) {
		rgb->red   = 224;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "LightCyan2") == 0) {
		rgb->red   = 209;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "LightCyan3") == 0) {
		rgb->red   = 180;
		rgb->green = 205;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "LightCyan4") == 0) {
		rgb->red   = 122;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "PaleTurquoise1") == 0) {
		rgb->red   = 187;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "PaleTurquoise2") == 0) {
		rgb->red   = 174;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "PaleTurquoise3") == 0) {
		rgb->red   = 150;
		rgb->green = 205;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "PaleTurquoise4") == 0) {
		rgb->red   = 102;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "CadetBlue1") == 0) {
		rgb->red   = 152;
		rgb->green = 245;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "CadetBlue2") == 0) {
		rgb->red   = 142;
		rgb->green = 229;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "CadetBlue3") == 0) {
		rgb->red   = 122;
		rgb->green = 197;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "CadetBlue4") == 0) {
		rgb->red   = 83;
		rgb->green = 134;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "turquoise1") == 0) {
		rgb->red   = 0;
		rgb->green = 245;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "turquoise2") == 0) {
		rgb->red   = 0;
		rgb->green = 229;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "turquoise3") == 0) {
		rgb->red   = 0;
		rgb->green = 197;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "turquoise4") == 0) {
		rgb->red   = 0;
		rgb->green = 134;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "cyan1") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "cyan2") == 0) {
		rgb->red   = 0;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "cyan3") == 0) {
		rgb->red   = 0;
		rgb->green = 205;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "cyan4") == 0) {
		rgb->red   = 0;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkSlateGray1") == 0) {
		rgb->red   = 151;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DarkSlateGray2") == 0) {
		rgb->red   = 141;
		rgb->green = 238;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "DarkSlateGray3") == 0) {
		rgb->red   = 121;
		rgb->green = 205;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "DarkSlateGray4") == 0) {
		rgb->red   = 82;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "aquamarine1") == 0) {
		rgb->red   = 127;
		rgb->green = 255;
		rgb->green = 212;
		return;
	}
	if(strcmp(color, "aquamarine2") == 0) {
		rgb->red   = 118;
		rgb->green = 238;
		rgb->green = 198;
		return;
	}
	if(strcmp(color, "aquamarine3") == 0) {
		rgb->red   = 102;
		rgb->green = 205;
		rgb->green = 170;
		return;
	}
	if(strcmp(color, "aquamarine4") == 0) {
		rgb->red   = 69;
		rgb->green = 139;
		rgb->green = 116;
		return;
	}
	if(strcmp(color, "DarkSeaGreen1") == 0) {
		rgb->red   = 193;
		rgb->green = 255;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "DarkSeaGreen2") == 0) {
		rgb->red   = 180;
		rgb->green = 238;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "DarkSeaGreen3") == 0) {
		rgb->red   = 155;
		rgb->green = 205;
		rgb->green = 155;
		return;
	}
	if(strcmp(color, "DarkSeaGreen4") == 0) {
		rgb->red   = 105;
		rgb->green = 139;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "SeaGreen1") == 0) {
		rgb->red   = 84;
		rgb->green = 255;
		rgb->green = 159;
		return;
	}
	if(strcmp(color, "SeaGreen2") == 0) {
		rgb->red   = 78;
		rgb->green = 238;
		rgb->green = 148;
		return;
	}
	if(strcmp(color, "SeaGreen3") == 0) {
		rgb->red   = 67;
		rgb->green = 205;
		rgb->green = 128;
		return;
	}
	if(strcmp(color, "SeaGreen4") == 0) {
		rgb->red   = 46;
		rgb->green = 139;
		rgb->green = 87;
		return;
	}
	if(strcmp(color, "PaleGreen1") == 0) {
		rgb->red   = 154;
		rgb->green = 255;
		rgb->green = 154;
		return;
	}
	if(strcmp(color, "PaleGreen2") == 0) {
		rgb->red   = 144;
		rgb->green = 238;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "PaleGreen3") == 0) {
		rgb->red   = 124;
		rgb->green = 205;
		rgb->green = 124;
		return;
	}
	if(strcmp(color, "PaleGreen4") == 0) {
		rgb->red   = 84;
		rgb->green = 139;
		rgb->green = 84;
		return;
	}
	if(strcmp(color, "SpringGreen1") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 127;
		return;
	}
	if(strcmp(color, "SpringGreen2") == 0) {
		rgb->red   = 0;
		rgb->green = 238;
		rgb->green = 118;
		return;
	}
	if(strcmp(color, "SpringGreen3") == 0) {
		rgb->red   = 0;
		rgb->green = 205;
		rgb->green = 102;
		return;
	}
	if(strcmp(color, "SpringGreen4") == 0) {
		rgb->red   = 0;
		rgb->green = 139;
		rgb->green = 69;
		return;
	}
	if(strcmp(color, "green1") == 0) {
		rgb->red   = 0;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "green2") == 0) {
		rgb->red   = 0;
		rgb->green = 238;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "green3") == 0) {
		rgb->red   = 0;
		rgb->green = 205;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "green4") == 0) {
		rgb->red   = 0;
		rgb->green = 139;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "chartreuse1") == 0) {
		rgb->red   = 127;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "chartreuse2") == 0) {
		rgb->red   = 118;
		rgb->green = 238;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "chartreuse3") == 0) {
		rgb->red   = 102;
		rgb->green = 205;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "chartreuse4") == 0) {
		rgb->red   = 69;
		rgb->green = 139;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "OliveDrab1") == 0) {
		rgb->red   = 192;
		rgb->green = 255;
		rgb->green = 62;
		return;
	}
	if(strcmp(color, "OliveDrab2") == 0) {
		rgb->red   = 179;
		rgb->green = 238;
		rgb->green = 58;
		return;
	}
	if(strcmp(color, "OliveDrab3") == 0) {
		rgb->red   = 154;
		rgb->green = 205;
		rgb->green = 50;
		return;
	}
	if(strcmp(color, "OliveDrab4") == 0) {
		rgb->red   = 105;
		rgb->green = 139;
		rgb->green = 34;
		return;
	}
	if(strcmp(color, "DarkOliveGreen1") == 0) {
		rgb->red   = 202;
		rgb->green = 255;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "DarkOliveGreen2") == 0) {
		rgb->red   = 188;
		rgb->green = 238;
		rgb->green = 104;
		return;
	}
	if(strcmp(color, "DarkOliveGreen3") == 0) {
		rgb->red   = 162;
		rgb->green = 205;
		rgb->green = 90;
		return;
	}
	if(strcmp(color, "DarkOliveGreen4") == 0) {
		rgb->red   = 110;
		rgb->green = 139;
		rgb->green = 61;
		return;
	}
	if(strcmp(color, "khaki1") == 0) {
		rgb->red   = 255;
		rgb->green = 246;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "khaki2") == 0) {
		rgb->red   = 238;
		rgb->green = 230;
		rgb->green = 133;
		return;
	}
	if(strcmp(color, "khaki3") == 0) {
		rgb->red   = 205;
		rgb->green = 198;
		rgb->green = 115;
		return;
	}
	if(strcmp(color, "khaki4") == 0) {
		rgb->red   = 139;
		rgb->green = 134;
		rgb->green = 78;
		return;
	}
	if(strcmp(color, "LightGoldenrod1") == 0) {
		rgb->red   = 255;
		rgb->green = 236;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "LightGoldenrod2") == 0) {
		rgb->red   = 238;
		rgb->green = 220;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "LightGoldenrod3") == 0) {
		rgb->red   = 205;
		rgb->green = 190;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "LightGoldenrod4") == 0) {
		rgb->red   = 139;
		rgb->green = 129;
		rgb->green = 76;
		return;
	}
	if(strcmp(color, "LightYellow1") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "LightYellow2") == 0) {
		rgb->red   = 238;
		rgb->green = 238;
		rgb->green = 209;
		return;
	}
	if(strcmp(color, "LightYellow3") == 0) {
		rgb->red   = 205;
		rgb->green = 205;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "LightYellow4") == 0) {
		rgb->red   = 139;
		rgb->green = 139;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "yellow1") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "yellow2") == 0) {
		rgb->red   = 238;
		rgb->green = 238;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "yellow3") == 0) {
		rgb->red   = 205;
		rgb->green = 205;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "yellow4") == 0) {
		rgb->red   = 139;
		rgb->green = 139;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gold1") == 0) {
		rgb->red   = 255;
		rgb->green = 215;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gold2") == 0) {
		rgb->red   = 238;
		rgb->green = 201;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gold3") == 0) {
		rgb->red   = 205;
		rgb->green = 173;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gold4") == 0) {
		rgb->red   = 139;
		rgb->green = 117;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "goldenrod1") == 0) {
		rgb->red   = 255;
		rgb->green = 193;
		rgb->green = 37;
		return;
	}
	if(strcmp(color, "goldenrod2") == 0) {
		rgb->red   = 238;
		rgb->green = 180;
		rgb->green = 34;
		return;
	}
	if(strcmp(color, "goldenrod3") == 0) {
		rgb->red   = 205;
		rgb->green = 155;
		rgb->green = 29;
		return;
	}
	if(strcmp(color, "goldenrod4") == 0) {
		rgb->red   = 139;
		rgb->green = 105;
		rgb->green = 20;
		return;
	}
	if(strcmp(color, "DarkGoldenrod1") == 0) {
		rgb->red   = 255;
		rgb->green = 185;
		rgb->green = 15;
		return;
	}
	if(strcmp(color, "DarkGoldenrod2") == 0) {
		rgb->red   = 238;
		rgb->green = 173;
		rgb->green = 14;
		return;
	}
	if(strcmp(color, "DarkGoldenrod3") == 0) {
		rgb->red   = 205;
		rgb->green = 149;
		rgb->green = 12;
		return;
	}
	if(strcmp(color, "DarkGoldenrod4") == 0) {
		rgb->red   = 139;
		rgb->green = 101;
		rgb->green = 8;
		return;
	}
	if(strcmp(color, "RosyBrown1") == 0) {
		rgb->red   = 255;
		rgb->green = 193;
		rgb->green = 193;
		return;
	}
	if(strcmp(color, "RosyBrown2") == 0) {
		rgb->red   = 238;
		rgb->green = 180;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "RosyBrown3") == 0) {
		rgb->red   = 205;
		rgb->green = 155;
		rgb->green = 155;
		return;
	}
	if(strcmp(color, "RosyBrown4") == 0) {
		rgb->red   = 139;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "IndianRed1") == 0) {
		rgb->red   = 255;
		rgb->green = 106;
		rgb->green = 106;
		return;
	}
	if(strcmp(color, "IndianRed2") == 0) {
		rgb->red   = 238;
		rgb->green = 99;
		rgb->green = 99;
		return;
	}
	if(strcmp(color, "IndianRed3") == 0) {
		rgb->red   = 205;
		rgb->green = 85;
		rgb->green = 85;
		return;
	}
	if(strcmp(color, "IndianRed4") == 0) {
		rgb->red   = 139;
		rgb->green = 58;
		rgb->green = 58;
		return;
	}
	if(strcmp(color, "sienna1") == 0) {
		rgb->red   = 255;
		rgb->green = 130;
		rgb->green = 71;
		return;
	}
	if(strcmp(color, "sienna2") == 0) {
		rgb->red   = 238;
		rgb->green = 121;
		rgb->green = 66;
		return;
	}
	if(strcmp(color, "sienna3") == 0) {
		rgb->red   = 205;
		rgb->green = 104;
		rgb->green = 57;
		return;
	}
	if(strcmp(color, "sienna4") == 0) {
		rgb->red   = 139;
		rgb->green = 71;
		rgb->green = 38;
		return;
	}
	if(strcmp(color, "burlywood1") == 0) {
		rgb->red   = 255;
		rgb->green = 211;
		rgb->green = 155;
		return;
	}
	if(strcmp(color, "burlywood2") == 0) {
		rgb->red   = 238;
		rgb->green = 197;
		rgb->green = 145;
		return;
	}
	if(strcmp(color, "burlywood3") == 0) {
		rgb->red   = 205;
		rgb->green = 170;
		rgb->green = 125;
		return;
	}
	if(strcmp(color, "burlywood4") == 0) {
		rgb->red   = 139;
		rgb->green = 115;
		rgb->green = 85;
		return;
	}
	if(strcmp(color, "wheat1") == 0) {
		rgb->red   = 255;
		rgb->green = 231;
		rgb->green = 186;
		return;
	}
	if(strcmp(color, "wheat2") == 0) {
		rgb->red   = 238;
		rgb->green = 216;
		rgb->green = 174;
		return;
	}
	if(strcmp(color, "wheat3") == 0) {
		rgb->red   = 205;
		rgb->green = 186;
		rgb->green = 150;
		return;
	}
	if(strcmp(color, "wheat4") == 0) {
		rgb->red   = 139;
		rgb->green = 126;
		rgb->green = 102;
		return;
	}
	if(strcmp(color, "tan1") == 0) {
		rgb->red   = 255;
		rgb->green = 165;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "tan2") == 0) {
		rgb->red   = 238;
		rgb->green = 154;
		rgb->green = 73;
		return;
	}
	if(strcmp(color, "tan3") == 0) {
		rgb->red   = 205;
		rgb->green = 133;
		rgb->green = 63;
		return;
	}
	if(strcmp(color, "tan4") == 0) {
		rgb->red   = 139;
		rgb->green = 90;
		rgb->green = 43;
		return;
	}
	if(strcmp(color, "chocolate1") == 0) {
		rgb->red   = 255;
		rgb->green = 127;
		rgb->green = 36;
		return;
	}
	if(strcmp(color, "chocolate2") == 0) {
		rgb->red   = 238;
		rgb->green = 118;
		rgb->green = 33;
		return;
	}
	if(strcmp(color, "chocolate3") == 0) {
		rgb->red   = 205;
		rgb->green = 102;
		rgb->green = 29;
		return;
	}
	if(strcmp(color, "chocolate4") == 0) {
		rgb->red   = 139;
		rgb->green = 69;
		rgb->green = 19;
		return;
	}
	if(strcmp(color, "firebrick1") == 0) {
		rgb->red   = 255;
		rgb->green = 48;
		rgb->green = 48;
		return;
	}
	if(strcmp(color, "firebrick2") == 0) {
		rgb->red   = 238;
		rgb->green = 44;
		rgb->green = 44;
		return;
	}
	if(strcmp(color, "firebrick3") == 0) {
		rgb->red   = 205;
		rgb->green = 38;
		rgb->green = 38;
		return;
	}
	if(strcmp(color, "firebrick4") == 0) {
		rgb->red   = 139;
		rgb->green = 26;
		rgb->green = 26;
		return;
	}
	if(strcmp(color, "brown1") == 0) {
		rgb->red   = 255;
		rgb->green = 64;
		rgb->green = 64;
		return;
	}
	if(strcmp(color, "brown2") == 0) {
		rgb->red   = 238;
		rgb->green = 59;
		rgb->green = 59;
		return;
	}
	if(strcmp(color, "brown3") == 0) {
		rgb->red   = 205;
		rgb->green = 51;
		rgb->green = 51;
		return;
	}
	if(strcmp(color, "brown4") == 0) {
		rgb->red   = 139;
		rgb->green = 35;
		rgb->green = 35;
		return;
	}
	if(strcmp(color, "salmon1") == 0) {
		rgb->red   = 255;
		rgb->green = 140;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "salmon2") == 0) {
		rgb->red   = 238;
		rgb->green = 130;
		rgb->green = 98;
		return;
	}
	if(strcmp(color, "salmon3") == 0) {
		rgb->red   = 205;
		rgb->green = 112;
		rgb->green = 84;
		return;
	}
	if(strcmp(color, "salmon4") == 0) {
		rgb->red   = 139;
		rgb->green = 76;
		rgb->green = 57;
		return;
	}
	if(strcmp(color, "LightSalmon1") == 0) {
		rgb->red   = 255;
		rgb->green = 160;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "LightSalmon2") == 0) {
		rgb->red   = 238;
		rgb->green = 149;
		rgb->green = 114;
		return;
	}
	if(strcmp(color, "LightSalmon3") == 0) {
		rgb->red   = 205;
		rgb->green = 129;
		rgb->green = 98;
		return;
	}
	if(strcmp(color, "LightSalmon4") == 0) {
		rgb->red   = 139;
		rgb->green = 87;
		rgb->green = 66;
		return;
	}
	if(strcmp(color, "orange1") == 0) {
		rgb->red   = 255;
		rgb->green = 165;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "orange2") == 0) {
		rgb->red   = 238;
		rgb->green = 154;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "orange3") == 0) {
		rgb->red   = 205;
		rgb->green = 133;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "orange4") == 0) {
		rgb->red   = 139;
		rgb->green = 90;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkOrange1") == 0) {
		rgb->red   = 255;
		rgb->green = 127;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkOrange2") == 0) {
		rgb->red   = 238;
		rgb->green = 118;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkOrange3") == 0) {
		rgb->red   = 205;
		rgb->green = 102;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkOrange4") == 0) {
		rgb->red   = 139;
		rgb->green = 69;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "coral1") == 0) {
		rgb->red   = 255;
		rgb->green = 114;
		rgb->green = 86;
		return;
	}
	if(strcmp(color, "coral2") == 0) {
		rgb->red   = 238;
		rgb->green = 106;
		rgb->green = 80;
		return;
	}
	if(strcmp(color, "coral3") == 0) {
		rgb->red   = 205;
		rgb->green = 91;
		rgb->green = 69;
		return;
	}
	if(strcmp(color, "coral4") == 0) {
		rgb->red   = 139;
		rgb->green = 62;
		rgb->green = 47;
		return;
	}
	if(strcmp(color, "tomato1") == 0) {
		rgb->red   = 255;
		rgb->green = 99;
		rgb->green = 71;
		return;
	}
	if(strcmp(color, "tomato2") == 0) {
		rgb->red   = 238;
		rgb->green = 92;
		rgb->green = 66;
		return;
	}
	if(strcmp(color, "tomato3") == 0) {
		rgb->red   = 205;
		rgb->green = 79;
		rgb->green = 57;
		return;
	}
	if(strcmp(color, "tomato4") == 0) {
		rgb->red   = 139;
		rgb->green = 54;
		rgb->green = 38;
		return;
	}
	if(strcmp(color, "OrangeRed1") == 0) {
		rgb->red   = 255;
		rgb->green = 69;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "OrangeRed2") == 0) {
		rgb->red   = 238;
		rgb->green = 64;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "OrangeRed3") == 0) {
		rgb->red   = 205;
		rgb->green = 55;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "OrangeRed4") == 0) {
		rgb->red   = 139;
		rgb->green = 37;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "red1") == 0) {
		rgb->red   = 255;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "red2") == 0) {
		rgb->red   = 238;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "red3") == 0) {
		rgb->red   = 205;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "red4") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DeepPink1") == 0) {
		rgb->red   = 255;
		rgb->green = 20;
		rgb->green = 147;
		return;
	}
	if(strcmp(color, "DeepPink2") == 0) {
		rgb->red   = 238;
		rgb->green = 18;
		rgb->green = 137;
		return;
	}
	if(strcmp(color, "DeepPink3") == 0) {
		rgb->red   = 205;
		rgb->green = 16;
		rgb->green = 118;
		return;
	}
	if(strcmp(color, "DeepPink4") == 0) {
		rgb->red   = 139;
		rgb->green = 10;
		rgb->green = 80;
		return;
	}
	if(strcmp(color, "HotPink1") == 0) {
		rgb->red   = 255;
		rgb->green = 110;
		rgb->green = 180;
		return;
	}
	if(strcmp(color, "HotPink2") == 0) {
		rgb->red   = 238;
		rgb->green = 106;
		rgb->green = 167;
		return;
	}
	if(strcmp(color, "HotPink3") == 0) {
		rgb->red   = 205;
		rgb->green = 96;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "HotPink4") == 0) {
		rgb->red   = 139;
		rgb->green = 58;
		rgb->green = 98;
		return;
	}
	if(strcmp(color, "pink1") == 0) {
		rgb->red   = 255;
		rgb->green = 181;
		rgb->green = 197;
		return;
	}
	if(strcmp(color, "pink2") == 0) {
		rgb->red   = 238;
		rgb->green = 169;
		rgb->green = 184;
		return;
	}
	if(strcmp(color, "pink3") == 0) {
		rgb->red   = 205;
		rgb->green = 145;
		rgb->green = 158;
		return;
	}
	if(strcmp(color, "pink4") == 0) {
		rgb->red   = 139;
		rgb->green = 99;
		rgb->green = 108;
		return;
	}
	if(strcmp(color, "LightPink1") == 0) {
		rgb->red   = 255;
		rgb->green = 174;
		rgb->green = 185;
		return;
	}
	if(strcmp(color, "LightPink2") == 0) {
		rgb->red   = 238;
		rgb->green = 162;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "LightPink3") == 0) {
		rgb->red   = 205;
		rgb->green = 140;
		rgb->green = 149;
		return;
	}
	if(strcmp(color, "LightPink4") == 0) {
		rgb->red   = 139;
		rgb->green = 95;
		rgb->green = 101;
		return;
	}
	if(strcmp(color, "PaleVioletRed1") == 0) {
		rgb->red   = 255;
		rgb->green = 130;
		rgb->green = 171;
		return;
	}
	if(strcmp(color, "PaleVioletRed2") == 0) {
		rgb->red   = 238;
		rgb->green = 121;
		rgb->green = 159;
		return;
	}
	if(strcmp(color, "PaleVioletRed3") == 0) {
		rgb->red   = 205;
		rgb->green = 104;
		rgb->green = 137;
		return;
	}
	if(strcmp(color, "PaleVioletRed4") == 0) {
		rgb->red   = 139;
		rgb->green = 71;
		rgb->green = 93;
		return;
	}
	if(strcmp(color, "maroon1") == 0) {
		rgb->red   = 255;
		rgb->green = 52;
		rgb->green = 179;
		return;
	}
	if(strcmp(color, "maroon2") == 0) {
		rgb->red   = 238;
		rgb->green = 48;
		rgb->green = 167;
		return;
	}
	if(strcmp(color, "maroon3") == 0) {
		rgb->red   = 205;
		rgb->green = 41;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "maroon4") == 0) {
		rgb->red   = 139;
		rgb->green = 28;
		rgb->green = 98;
		return;
	}
	if(strcmp(color, "VioletRed1") == 0) {
		rgb->red   = 255;
		rgb->green = 62;
		rgb->green = 150;
		return;
	}
	if(strcmp(color, "VioletRed2") == 0) {
		rgb->red   = 238;
		rgb->green = 58;
		rgb->green = 140;
		return;
	}
	if(strcmp(color, "VioletRed3") == 0) {
		rgb->red   = 205;
		rgb->green = 50;
		rgb->green = 120;
		return;
	}
	if(strcmp(color, "VioletRed4") == 0) {
		rgb->red   = 139;
		rgb->green = 34;
		rgb->green = 82;
		return;
	}
	if(strcmp(color, "magenta1") == 0) {
		rgb->red   = 255;
		rgb->green = 0;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "magenta2") == 0) {
		rgb->red   = 238;
		rgb->green = 0;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "magenta3") == 0) {
		rgb->red   = 205;
		rgb->green = 0;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "magenta4") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "orchid1") == 0) {
		rgb->red   = 255;
		rgb->green = 131;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "orchid2") == 0) {
		rgb->red   = 238;
		rgb->green = 122;
		rgb->green = 233;
		return;
	}
	if(strcmp(color, "orchid3") == 0) {
		rgb->red   = 205;
		rgb->green = 105;
		rgb->green = 201;
		return;
	}
	if(strcmp(color, "orchid4") == 0) {
		rgb->red   = 139;
		rgb->green = 71;
		rgb->green = 137;
		return;
	}
	if(strcmp(color, "plum1") == 0) {
		rgb->red   = 255;
		rgb->green = 187;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "plum2") == 0) {
		rgb->red   = 238;
		rgb->green = 174;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "plum3") == 0) {
		rgb->red   = 205;
		rgb->green = 150;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "plum4") == 0) {
		rgb->red   = 139;
		rgb->green = 102;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "MediumOrchid1") == 0) {
		rgb->red   = 224;
		rgb->green = 102;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "MediumOrchid2") == 0) {
		rgb->red   = 209;
		rgb->green = 95;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "MediumOrchid3") == 0) {
		rgb->red   = 180;
		rgb->green = 82;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "MediumOrchid4") == 0) {
		rgb->red   = 122;
		rgb->green = 55;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkOrchid1") == 0) {
		rgb->red   = 191;
		rgb->green = 62;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "DarkOrchid2") == 0) {
		rgb->red   = 178;
		rgb->green = 58;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "DarkOrchid3") == 0) {
		rgb->red   = 154;
		rgb->green = 50;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "DarkOrchid4") == 0) {
		rgb->red   = 104;
		rgb->green = 34;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "purple1") == 0) {
		rgb->red   = 155;
		rgb->green = 48;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "purple2") == 0) {
		rgb->red   = 145;
		rgb->green = 44;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "purple3") == 0) {
		rgb->red   = 125;
		rgb->green = 38;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "purple4") == 0) {
		rgb->red   = 85;
		rgb->green = 26;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "MediumPurple1") == 0) {
		rgb->red   = 171;
		rgb->green = 130;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "MediumPurple2") == 0) {
		rgb->red   = 159;
		rgb->green = 121;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "MediumPurple3") == 0) {
		rgb->red   = 137;
		rgb->green = 104;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "MediumPurple4") == 0) {
		rgb->red   = 93;
		rgb->green = 71;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "thistle1") == 0) {
		rgb->red   = 255;
		rgb->green = 225;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "thistle2") == 0) {
		rgb->red   = 238;
		rgb->green = 210;
		rgb->green = 238;
		return;
	}
	if(strcmp(color, "thistle3") == 0) {
		rgb->red   = 205;
		rgb->green = 181;
		rgb->green = 205;
		return;
	}
	if(strcmp(color, "thistle4") == 0) {
		rgb->red   = 139;
		rgb->green = 123;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "gray0") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "grey0") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "gray1") == 0) {
		rgb->red   = 3;
		rgb->green = 3;
		rgb->green = 3;
		return;
	}
	if(strcmp(color, "grey1") == 0) {
		rgb->red   = 3;
		rgb->green = 3;
		rgb->green = 3;
		return;
	}
	if(strcmp(color, "gray2") == 0) {
		rgb->red   = 5;
		rgb->green = 5;
		rgb->green = 5;
		return;
	}
	if(strcmp(color, "grey2") == 0) {
		rgb->red   = 5;
		rgb->green = 5;
		rgb->green = 5;
		return;
	}
	if(strcmp(color, "gray3") == 0) {
		rgb->red   = 8;
		rgb->green = 8;
		rgb->green = 8;
		return;
	}
	if(strcmp(color, "grey3") == 0) {
		rgb->red   = 8;
		rgb->green = 8;
		rgb->green = 8;
		return;
	}
	if(strcmp(color, "gray4") == 0) {
		rgb->red   = 10;
		rgb->green = 10;
		rgb->green = 10;
		return;
	}
	if(strcmp(color, "grey4") == 0) {
		rgb->red   = 10;
		rgb->green = 10;
		rgb->green = 10;
		return;
	}
	if(strcmp(color, "gray5") == 0) {
		rgb->red   = 13;
		rgb->green = 13;
		rgb->green = 13;
		return;
	}
	if(strcmp(color, "grey5") == 0) {
		rgb->red   = 13;
		rgb->green = 13;
		rgb->green = 13;
		return;
	}
	if(strcmp(color, "gray6") == 0) {
		rgb->red   = 15;
		rgb->green = 15;
		rgb->green = 15;
		return;
	}
	if(strcmp(color, "grey6") == 0) {
		rgb->red   = 15;
		rgb->green = 15;
		rgb->green = 15;
		return;
	}
	if(strcmp(color, "gray7") == 0) {
		rgb->red   = 18;
		rgb->green = 18;
		rgb->green = 18;
		return;
	}
	if(strcmp(color, "grey7") == 0) {
		rgb->red   = 18;
		rgb->green = 18;
		rgb->green = 18;
		return;
	}
	if(strcmp(color, "gray8") == 0) {
		rgb->red   = 20;
		rgb->green = 20;
		rgb->green = 20;
		return;
	}
	if(strcmp(color, "grey8") == 0) {
		rgb->red   = 20;
		rgb->green = 20;
		rgb->green = 20;
		return;
	}
	if(strcmp(color, "gray9") == 0) {
		rgb->red   = 23;
		rgb->green = 23;
		rgb->green = 23;
		return;
	}
	if(strcmp(color, "grey9") == 0) {
		rgb->red   = 23;
		rgb->green = 23;
		rgb->green = 23;
		return;
	}
	if(strcmp(color, "gray10") == 0) {
		rgb->red   = 26;
		rgb->green = 26;
		rgb->green = 26;
		return;
	}
	if(strcmp(color, "grey10") == 0) {
		rgb->red   = 26;
		rgb->green = 26;
		rgb->green = 26;
		return;
	}
	if(strcmp(color, "gray11") == 0) {
		rgb->red   = 28;
		rgb->green = 28;
		rgb->green = 28;
		return;
	}
	if(strcmp(color, "grey11") == 0) {
		rgb->red   = 28;
		rgb->green = 28;
		rgb->green = 28;
		return;
	}
	if(strcmp(color, "gray12") == 0) {
		rgb->red   = 31;
		rgb->green = 31;
		rgb->green = 31;
		return;
	}
	if(strcmp(color, "grey12") == 0) {
		rgb->red   = 31;
		rgb->green = 31;
		rgb->green = 31;
		return;
	}
	if(strcmp(color, "gray13") == 0) {
		rgb->red   = 33;
		rgb->green = 33;
		rgb->green = 33;
		return;
	}
	if(strcmp(color, "grey13") == 0) {
		rgb->red   = 33;
		rgb->green = 33;
		rgb->green = 33;
		return;
	}
	if(strcmp(color, "gray14") == 0) {
		rgb->red   = 36;
		rgb->green = 36;
		rgb->green = 36;
		return;
	}
	if(strcmp(color, "grey14") == 0) {
		rgb->red   = 36;
		rgb->green = 36;
		rgb->green = 36;
		return;
	}
	if(strcmp(color, "gray15") == 0) {
		rgb->red   = 38;
		rgb->green = 38;
		rgb->green = 38;
		return;
	}
	if(strcmp(color, "grey15") == 0) {
		rgb->red   = 38;
		rgb->green = 38;
		rgb->green = 38;
		return;
	}
	if(strcmp(color, "gray16") == 0) {
		rgb->red   = 41;
		rgb->green = 41;
		rgb->green = 41;
		return;
	}
	if(strcmp(color, "grey16") == 0) {
		rgb->red   = 41;
		rgb->green = 41;
		rgb->green = 41;
		return;
	}
	if(strcmp(color, "gray17") == 0) {
		rgb->red   = 43;
		rgb->green = 43;
		rgb->green = 43;
		return;
	}
	if(strcmp(color, "grey17") == 0) {
		rgb->red   = 43;
		rgb->green = 43;
		rgb->green = 43;
		return;
	}
	if(strcmp(color, "gray18") == 0) {
		rgb->red   = 46;
		rgb->green = 46;
		rgb->green = 46;
		return;
	}
	if(strcmp(color, "grey18") == 0) {
		rgb->red   = 46;
		rgb->green = 46;
		rgb->green = 46;
		return;
	}
	if(strcmp(color, "gray19") == 0) {
		rgb->red   = 48;
		rgb->green = 48;
		rgb->green = 48;
		return;
	}
	if(strcmp(color, "grey19") == 0) {
		rgb->red   = 48;
		rgb->green = 48;
		rgb->green = 48;
		return;
	}
	if(strcmp(color, "gray20") == 0) {
		rgb->red   = 51;
		rgb->green = 51;
		rgb->green = 51;
		return;
	}
	if(strcmp(color, "grey20") == 0) {
		rgb->red   = 51;
		rgb->green = 51;
		rgb->green = 51;
		return;
	}
	if(strcmp(color, "gray21") == 0) {
		rgb->red   = 54;
		rgb->green = 54;
		rgb->green = 54;
		return;
	}
	if(strcmp(color, "grey21") == 0) {
		rgb->red   = 54;
		rgb->green = 54;
		rgb->green = 54;
		return;
	}
	if(strcmp(color, "gray22") == 0) {
		rgb->red   = 56;
		rgb->green = 56;
		rgb->green = 56;
		return;
	}
	if(strcmp(color, "grey22") == 0) {
		rgb->red   = 56;
		rgb->green = 56;
		rgb->green = 56;
		return;
	}
	if(strcmp(color, "gray23") == 0) {
		rgb->red   = 59;
		rgb->green = 59;
		rgb->green = 59;
		return;
	}
	if(strcmp(color, "grey23") == 0) {
		rgb->red   = 59;
		rgb->green = 59;
		rgb->green = 59;
		return;
	}
	if(strcmp(color, "gray24") == 0) {
		rgb->red   = 61;
		rgb->green = 61;
		rgb->green = 61;
		return;
	}
	if(strcmp(color, "grey24") == 0) {
		rgb->red   = 61;
		rgb->green = 61;
		rgb->green = 61;
		return;
	}
	if(strcmp(color, "gray25") == 0) {
		rgb->red   = 64;
		rgb->green = 64;
		rgb->green = 64;
		return;
	}
	if(strcmp(color, "grey25") == 0) {
		rgb->red   = 64;
		rgb->green = 64;
		rgb->green = 64;
		return;
	}
	if(strcmp(color, "gray26") == 0) {
		rgb->red   = 66;
		rgb->green = 66;
		rgb->green = 66;
		return;
	}
	if(strcmp(color, "grey26") == 0) {
		rgb->red   = 66;
		rgb->green = 66;
		rgb->green = 66;
		return;
	}
	if(strcmp(color, "gray27") == 0) {
		rgb->red   = 69;
		rgb->green = 69;
		rgb->green = 69;
		return;
	}
	if(strcmp(color, "grey27") == 0) {
		rgb->red   = 69;
		rgb->green = 69;
		rgb->green = 69;
		return;
	}
	if(strcmp(color, "gray28") == 0) {
		rgb->red   = 71;
		rgb->green = 71;
		rgb->green = 71;
		return;
	}
	if(strcmp(color, "grey28") == 0) {
		rgb->red   = 71;
		rgb->green = 71;
		rgb->green = 71;
		return;
	}
	if(strcmp(color, "gray29") == 0) {
		rgb->red   = 74;
		rgb->green = 74;
		rgb->green = 74;
		return;
	}
	if(strcmp(color, "grey29") == 0) {
		rgb->red   = 74;
		rgb->green = 74;
		rgb->green = 74;
		return;
	}
	if(strcmp(color, "gray30") == 0) {
		rgb->red   = 77;
		rgb->green = 77;
		rgb->green = 77;
		return;
	}
	if(strcmp(color, "grey30") == 0) {
		rgb->red   = 77;
		rgb->green = 77;
		rgb->green = 77;
		return;
	}
	if(strcmp(color, "gray31") == 0) {
		rgb->red   = 79;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "grey31") == 0) {
		rgb->red   = 79;
		rgb->green = 79;
		rgb->green = 79;
		return;
	}
	if(strcmp(color, "gray32") == 0) {
		rgb->red   = 82;
		rgb->green = 82;
		rgb->green = 82;
		return;
	}
	if(strcmp(color, "grey32") == 0) {
		rgb->red   = 82;
		rgb->green = 82;
		rgb->green = 82;
		return;
	}
	if(strcmp(color, "gray33") == 0) {
		rgb->red   = 84;
		rgb->green = 84;
		rgb->green = 84;
		return;
	}
	if(strcmp(color, "grey33") == 0) {
		rgb->red   = 84;
		rgb->green = 84;
		rgb->green = 84;
		return;
	}
	if(strcmp(color, "gray34") == 0) {
		rgb->red   = 87;
		rgb->green = 87;
		rgb->green = 87;
		return;
	}
	if(strcmp(color, "grey34") == 0) {
		rgb->red   = 87;
		rgb->green = 87;
		rgb->green = 87;
		return;
	}
	if(strcmp(color, "gray35") == 0) {
		rgb->red   = 89;
		rgb->green = 89;
		rgb->green = 89;
		return;
	}
	if(strcmp(color, "grey35") == 0) {
		rgb->red   = 89;
		rgb->green = 89;
		rgb->green = 89;
		return;
	}
	if(strcmp(color, "gray36") == 0) {
		rgb->red   = 92;
		rgb->green = 92;
		rgb->green = 92;
		return;
	}
	if(strcmp(color, "grey36") == 0) {
		rgb->red   = 92;
		rgb->green = 92;
		rgb->green = 92;
		return;
	}
	if(strcmp(color, "gray37") == 0) {
		rgb->red   = 94;
		rgb->green = 94;
		rgb->green = 94;
		return;
	}
	if(strcmp(color, "grey37") == 0) {
		rgb->red   = 94;
		rgb->green = 94;
		rgb->green = 94;
		return;
	}
	if(strcmp(color, "gray38") == 0) {
		rgb->red   = 97;
		rgb->green = 97;
		rgb->green = 97;
		return;
	}
	if(strcmp(color, "grey38") == 0) {
		rgb->red   = 97;
		rgb->green = 97;
		rgb->green = 97;
		return;
	}
	if(strcmp(color, "gray39") == 0) {
		rgb->red   = 99;
		rgb->green = 99;
		rgb->green = 99;
		return;
	}
	if(strcmp(color, "grey39") == 0) {
		rgb->red   = 99;
		rgb->green = 99;
		rgb->green = 99;
		return;
	}
	if(strcmp(color, "gray40") == 0) {
		rgb->red   = 102;
		rgb->green = 102;
		rgb->green = 102;
		return;
	}
	if(strcmp(color, "grey40") == 0) {
		rgb->red   = 102;
		rgb->green = 102;
		rgb->green = 102;
		return;
	}
	if(strcmp(color, "gray41") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "grey41") == 0) {
		rgb->red   = 105;
		rgb->green = 105;
		rgb->green = 105;
		return;
	}
	if(strcmp(color, "gray42") == 0) {
		rgb->red   = 107;
		rgb->green = 107;
		rgb->green = 107;
		return;
	}
	if(strcmp(color, "grey42") == 0) {
		rgb->red   = 107;
		rgb->green = 107;
		rgb->green = 107;
		return;
	}
	if(strcmp(color, "gray43") == 0) {
		rgb->red   = 110;
		rgb->green = 110;
		rgb->green = 110;
		return;
	}
	if(strcmp(color, "grey43") == 0) {
		rgb->red   = 110;
		rgb->green = 110;
		rgb->green = 110;
		return;
	}
	if(strcmp(color, "gray44") == 0) {
		rgb->red   = 112;
		rgb->green = 112;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "grey44") == 0) {
		rgb->red   = 112;
		rgb->green = 112;
		rgb->green = 112;
		return;
	}
	if(strcmp(color, "gray45") == 0) {
		rgb->red   = 115;
		rgb->green = 115;
		rgb->green = 115;
		return;
	}
	if(strcmp(color, "grey45") == 0) {
		rgb->red   = 115;
		rgb->green = 115;
		rgb->green = 115;
		return;
	}
	if(strcmp(color, "gray46") == 0) {
		rgb->red   = 117;
		rgb->green = 117;
		rgb->green = 117;
		return;
	}
	if(strcmp(color, "grey46") == 0) {
		rgb->red   = 117;
		rgb->green = 117;
		rgb->green = 117;
		return;
	}
	if(strcmp(color, "gray47") == 0) {
		rgb->red   = 120;
		rgb->green = 120;
		rgb->green = 120;
		return;
	}
	if(strcmp(color, "grey47") == 0) {
		rgb->red   = 120;
		rgb->green = 120;
		rgb->green = 120;
		return;
	}
	if(strcmp(color, "gray48") == 0) {
		rgb->red   = 122;
		rgb->green = 122;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "grey48") == 0) {
		rgb->red   = 122;
		rgb->green = 122;
		rgb->green = 122;
		return;
	}
	if(strcmp(color, "gray49") == 0) {
		rgb->red   = 125;
		rgb->green = 125;
		rgb->green = 125;
		return;
	}
	if(strcmp(color, "grey49") == 0) {
		rgb->red   = 125;
		rgb->green = 125;
		rgb->green = 125;
		return;
	}
	if(strcmp(color, "gray50") == 0) {
		rgb->red   = 127;
		rgb->green = 127;
		rgb->green = 127;
		return;
	}
	if(strcmp(color, "grey50") == 0) {
		rgb->red   = 127;
		rgb->green = 127;
		rgb->green = 127;
		return;
	}
	if(strcmp(color, "gray51") == 0) {
		rgb->red   = 130;
		rgb->green = 130;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "grey51") == 0) {
		rgb->red   = 130;
		rgb->green = 130;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "gray52") == 0) {
		rgb->red   = 133;
		rgb->green = 133;
		rgb->green = 133;
		return;
	}
	if(strcmp(color, "grey52") == 0) {
		rgb->red   = 133;
		rgb->green = 133;
		rgb->green = 133;
		return;
	}
	if(strcmp(color, "gray53") == 0) {
		rgb->red   = 135;
		rgb->green = 135;
		rgb->green = 135;
		return;
	}
	if(strcmp(color, "grey53") == 0) {
		rgb->red   = 135;
		rgb->green = 135;
		rgb->green = 135;
		return;
	}
	if(strcmp(color, "gray54") == 0) {
		rgb->red   = 138;
		rgb->green = 138;
		rgb->green = 138;
		return;
	}
	if(strcmp(color, "grey54") == 0) {
		rgb->red   = 138;
		rgb->green = 138;
		rgb->green = 138;
		return;
	}
	if(strcmp(color, "gray55") == 0) {
		rgb->red   = 140;
		rgb->green = 140;
		rgb->green = 140;
		return;
	}
	if(strcmp(color, "grey55") == 0) {
		rgb->red   = 140;
		rgb->green = 140;
		rgb->green = 140;
		return;
	}
	if(strcmp(color, "gray56") == 0) {
		rgb->red   = 143;
		rgb->green = 143;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "grey56") == 0) {
		rgb->red   = 143;
		rgb->green = 143;
		rgb->green = 143;
		return;
	}
	if(strcmp(color, "gray57") == 0) {
		rgb->red   = 145;
		rgb->green = 145;
		rgb->green = 145;
		return;
	}
	if(strcmp(color, "grey57") == 0) {
		rgb->red   = 145;
		rgb->green = 145;
		rgb->green = 145;
		return;
	}
	if(strcmp(color, "gray58") == 0) {
		rgb->red   = 148;
		rgb->green = 148;
		rgb->green = 148;
		return;
	}
	if(strcmp(color, "grey58") == 0) {
		rgb->red   = 148;
		rgb->green = 148;
		rgb->green = 148;
		return;
	}
	if(strcmp(color, "gray59") == 0) {
		rgb->red   = 150;
		rgb->green = 150;
		rgb->green = 150;
		return;
	}
	if(strcmp(color, "grey59") == 0) {
		rgb->red   = 150;
		rgb->green = 150;
		rgb->green = 150;
		return;
	}
	if(strcmp(color, "gray60") == 0) {
		rgb->red   = 153;
		rgb->green = 153;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "grey60") == 0) {
		rgb->red   = 153;
		rgb->green = 153;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "gray61") == 0) {
		rgb->red   = 156;
		rgb->green = 156;
		rgb->green = 156;
		return;
	}
	if(strcmp(color, "grey61") == 0) {
		rgb->red   = 156;
		rgb->green = 156;
		rgb->green = 156;
		return;
	}
	if(strcmp(color, "gray62") == 0) {
		rgb->red   = 158;
		rgb->green = 158;
		rgb->green = 158;
		return;
	}
	if(strcmp(color, "grey62") == 0) {
		rgb->red   = 158;
		rgb->green = 158;
		rgb->green = 158;
		return;
	}
	if(strcmp(color, "gray63") == 0) {
		rgb->red   = 161;
		rgb->green = 161;
		rgb->green = 161;
		return;
	}
	if(strcmp(color, "grey63") == 0) {
		rgb->red   = 161;
		rgb->green = 161;
		rgb->green = 161;
		return;
	}
	if(strcmp(color, "gray64") == 0) {
		rgb->red   = 163;
		rgb->green = 163;
		rgb->green = 163;
		return;
	}
	if(strcmp(color, "grey64") == 0) {
		rgb->red   = 163;
		rgb->green = 163;
		rgb->green = 163;
		return;
	}
	if(strcmp(color, "gray65") == 0) {
		rgb->red   = 166;
		rgb->green = 166;
		rgb->green = 166;
		return;
	}
	if(strcmp(color, "grey65") == 0) {
		rgb->red   = 166;
		rgb->green = 166;
		rgb->green = 166;
		return;
	}
	if(strcmp(color, "gray66") == 0) {
		rgb->red   = 168;
		rgb->green = 168;
		rgb->green = 168;
		return;
	}
	if(strcmp(color, "grey66") == 0) {
		rgb->red   = 168;
		rgb->green = 168;
		rgb->green = 168;
		return;
	}
	if(strcmp(color, "gray67") == 0) {
		rgb->red   = 171;
		rgb->green = 171;
		rgb->green = 171;
		return;
	}
	if(strcmp(color, "grey67") == 0) {
		rgb->red   = 171;
		rgb->green = 171;
		rgb->green = 171;
		return;
	}
	if(strcmp(color, "gray68") == 0) {
		rgb->red   = 173;
		rgb->green = 173;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "grey68") == 0) {
		rgb->red   = 173;
		rgb->green = 173;
		rgb->green = 173;
		return;
	}
	if(strcmp(color, "gray69") == 0) {
		rgb->red   = 176;
		rgb->green = 176;
		rgb->green = 176;
		return;
	}
	if(strcmp(color, "grey69") == 0) {
		rgb->red   = 176;
		rgb->green = 176;
		rgb->green = 176;
		return;
	}
	if(strcmp(color, "gray70") == 0) {
		rgb->red   = 179;
		rgb->green = 179;
		rgb->green = 179;
		return;
	}
	if(strcmp(color, "grey70") == 0) {
		rgb->red   = 179;
		rgb->green = 179;
		rgb->green = 179;
		return;
	}
	if(strcmp(color, "gray71") == 0) {
		rgb->red   = 181;
		rgb->green = 181;
		rgb->green = 181;
		return;
	}
	if(strcmp(color, "grey71") == 0) {
		rgb->red   = 181;
		rgb->green = 181;
		rgb->green = 181;
		return;
	}
	if(strcmp(color, "gray72") == 0) {
		rgb->red   = 184;
		rgb->green = 184;
		rgb->green = 184;
		return;
	}
	if(strcmp(color, "grey72") == 0) {
		rgb->red   = 184;
		rgb->green = 184;
		rgb->green = 184;
		return;
	}
	if(strcmp(color, "gray73") == 0) {
		rgb->red   = 186;
		rgb->green = 186;
		rgb->green = 186;
		return;
	}
	if(strcmp(color, "grey73") == 0) {
		rgb->red   = 186;
		rgb->green = 186;
		rgb->green = 186;
		return;
	}
	if(strcmp(color, "gray74") == 0) {
		rgb->red   = 189;
		rgb->green = 189;
		rgb->green = 189;
		return;
	}
	if(strcmp(color, "grey74") == 0) {
		rgb->red   = 189;
		rgb->green = 189;
		rgb->green = 189;
		return;
	}
	if(strcmp(color, "gray75") == 0) {
		rgb->red   = 191;
		rgb->green = 191;
		rgb->green = 191;
		return;
	}
	if(strcmp(color, "grey75") == 0) {
		rgb->red   = 191;
		rgb->green = 191;
		rgb->green = 191;
		return;
	}
	if(strcmp(color, "gray76") == 0) {
		rgb->red   = 194;
		rgb->green = 194;
		rgb->green = 194;
		return;
	}
	if(strcmp(color, "grey76") == 0) {
		rgb->red   = 194;
		rgb->green = 194;
		rgb->green = 194;
		return;
	}
	if(strcmp(color, "gray77") == 0) {
		rgb->red   = 196;
		rgb->green = 196;
		rgb->green = 196;
		return;
	}
	if(strcmp(color, "grey77") == 0) {
		rgb->red   = 196;
		rgb->green = 196;
		rgb->green = 196;
		return;
	}
	if(strcmp(color, "gray78") == 0) {
		rgb->red   = 199;
		rgb->green = 199;
		rgb->green = 199;
		return;
	}
	if(strcmp(color, "grey78") == 0) {
		rgb->red   = 199;
		rgb->green = 199;
		rgb->green = 199;
		return;
	}
	if(strcmp(color, "gray79") == 0) {
		rgb->red   = 201;
		rgb->green = 201;
		rgb->green = 201;
		return;
	}
	if(strcmp(color, "grey79") == 0) {
		rgb->red   = 201;
		rgb->green = 201;
		rgb->green = 201;
		return;
	}
	if(strcmp(color, "gray80") == 0) {
		rgb->red   = 204;
		rgb->green = 204;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "grey80") == 0) {
		rgb->red   = 204;
		rgb->green = 204;
		rgb->green = 204;
		return;
	}
	if(strcmp(color, "gray81") == 0) {
		rgb->red   = 207;
		rgb->green = 207;
		rgb->green = 207;
		return;
	}
	if(strcmp(color, "grey81") == 0) {
		rgb->red   = 207;
		rgb->green = 207;
		rgb->green = 207;
		return;
	}
	if(strcmp(color, "gray82") == 0) {
		rgb->red   = 209;
		rgb->green = 209;
		rgb->green = 209;
		return;
	}
	if(strcmp(color, "grey82") == 0) {
		rgb->red   = 209;
		rgb->green = 209;
		rgb->green = 209;
		return;
	}
	if(strcmp(color, "gray83") == 0) {
		rgb->red   = 212;
		rgb->green = 212;
		rgb->green = 212;
		return;
	}
	if(strcmp(color, "grey83") == 0) {
		rgb->red   = 212;
		rgb->green = 212;
		rgb->green = 212;
		return;
	}
	if(strcmp(color, "gray84") == 0) {
		rgb->red   = 214;
		rgb->green = 214;
		rgb->green = 214;
		return;
	}
	if(strcmp(color, "grey84") == 0) {
		rgb->red   = 214;
		rgb->green = 214;
		rgb->green = 214;
		return;
	}
	if(strcmp(color, "gray85") == 0) {
		rgb->red   = 217;
		rgb->green = 217;
		rgb->green = 217;
		return;
	}
	if(strcmp(color, "grey85") == 0) {
		rgb->red   = 217;
		rgb->green = 217;
		rgb->green = 217;
		return;
	}
	if(strcmp(color, "gray86") == 0) {
		rgb->red   = 219;
		rgb->green = 219;
		rgb->green = 219;
		return;
	}
	if(strcmp(color, "grey86") == 0) {
		rgb->red   = 219;
		rgb->green = 219;
		rgb->green = 219;
		return;
	}
	if(strcmp(color, "gray87") == 0) {
		rgb->red   = 222;
		rgb->green = 222;
		rgb->green = 222;
		return;
	}
	if(strcmp(color, "grey87") == 0) {
		rgb->red   = 222;
		rgb->green = 222;
		rgb->green = 222;
		return;
	}
	if(strcmp(color, "gray88") == 0) {
		rgb->red   = 224;
		rgb->green = 224;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "grey88") == 0) {
		rgb->red   = 224;
		rgb->green = 224;
		rgb->green = 224;
		return;
	}
	if(strcmp(color, "gray89") == 0) {
		rgb->red   = 227;
		rgb->green = 227;
		rgb->green = 227;
		return;
	}
	if(strcmp(color, "grey89") == 0) {
		rgb->red   = 227;
		rgb->green = 227;
		rgb->green = 227;
		return;
	}
	if(strcmp(color, "gray90") == 0) {
		rgb->red   = 229;
		rgb->green = 229;
		rgb->green = 229;
		return;
	}
	if(strcmp(color, "grey90") == 0) {
		rgb->red   = 229;
		rgb->green = 229;
		rgb->green = 229;
		return;
	}
	if(strcmp(color, "gray91") == 0) {
		rgb->red   = 232;
		rgb->green = 232;
		rgb->green = 232;
		return;
	}
	if(strcmp(color, "grey91") == 0) {
		rgb->red   = 232;
		rgb->green = 232;
		rgb->green = 232;
		return;
	}
	if(strcmp(color, "gray92") == 0) {
		rgb->red   = 235;
		rgb->green = 235;
		rgb->green = 235;
		return;
	}
	if(strcmp(color, "grey92") == 0) {
		rgb->red   = 235;
		rgb->green = 235;
		rgb->green = 235;
		return;
	}
	if(strcmp(color, "gray93") == 0) {
		rgb->red   = 237;
		rgb->green = 237;
		rgb->green = 237;
		return;
	}
	if(strcmp(color, "grey93") == 0) {
		rgb->red   = 237;
		rgb->green = 237;
		rgb->green = 237;
		return;
	}
	if(strcmp(color, "gray94") == 0) {
		rgb->red   = 240;
		rgb->green = 240;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "grey94") == 0) {
		rgb->red   = 240;
		rgb->green = 240;
		rgb->green = 240;
		return;
	}
	if(strcmp(color, "gray95") == 0) {
		rgb->red   = 242;
		rgb->green = 242;
		rgb->green = 242;
		return;
	}
	if(strcmp(color, "grey95") == 0) {
		rgb->red   = 242;
		rgb->green = 242;
		rgb->green = 242;
		return;
	}
	if(strcmp(color, "gray96") == 0) {
		rgb->red   = 245;
		rgb->green = 245;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "grey96") == 0) {
		rgb->red   = 245;
		rgb->green = 245;
		rgb->green = 245;
		return;
	}
	if(strcmp(color, "gray97") == 0) {
		rgb->red   = 247;
		rgb->green = 247;
		rgb->green = 247;
		return;
	}
	if(strcmp(color, "grey97") == 0) {
		rgb->red   = 247;
		rgb->green = 247;
		rgb->green = 247;
		return;
	}
	if(strcmp(color, "gray98") == 0) {
		rgb->red   = 250;
		rgb->green = 250;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "grey98") == 0) {
		rgb->red   = 250;
		rgb->green = 250;
		rgb->green = 250;
		return;
	}
	if(strcmp(color, "gray99") == 0) {
		rgb->red   = 252;
		rgb->green = 252;
		rgb->green = 252;
		return;
	}
	if(strcmp(color, "grey99") == 0) {
		rgb->red   = 252;
		rgb->green = 252;
		rgb->green = 252;
		return;
	}
	if(strcmp(color, "gray100") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "grey100") == 0) {
		rgb->red   = 255;
		rgb->green = 255;
		rgb->green = 255;
		return;
	}
	if(strcmp(color, "dark grey") == 0) {
		rgb->red   = 169;
		rgb->green = 169;
		rgb->green = 169;
		return;
	}
	if(strcmp(color, "DarkGrey") == 0) {
		rgb->red   = 169;
		rgb->green = 169;
		rgb->green = 169;
		return;
	}
	if(strcmp(color, "dark gray") == 0) {
		rgb->red   = 169;
		rgb->green = 169;
		rgb->green = 169;
		return;
	}
	if(strcmp(color, "DarkGray") == 0) {
		rgb->red   = 169;
		rgb->green = 169;
		rgb->green = 169;
		return;
	}
	if(strcmp(color, "dark blue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkBlue") == 0) {
		rgb->red   = 0;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "dark cyan") == 0) {
		rgb->red   = 0;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkCyan") == 0) {
		rgb->red   = 0;
		rgb->green = 139;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "dark magenta") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "DarkMagenta") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 139;
		return;
	}
	if(strcmp(color, "dark red") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "DarkRed") == 0) {
		rgb->red   = 139;
		rgb->green = 0;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "light green") == 0) {
		rgb->red   = 144;
		rgb->green = 238;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "LightGreen") == 0) {
		rgb->red   = 144;
		rgb->green = 238;
		rgb->green = 144;
		return;
	}
	if(strcmp(color, "crimson") == 0) {
		rgb->red   = 220;
		rgb->green = 20;
		rgb->green = 60;
		return;
	}
	if(strcmp(color, "indigo") == 0) {
		rgb->red   = 75;
		rgb->green = 0;
		rgb->green = 130;
		return;
	}
	if(strcmp(color, "olive") == 0) {
		rgb->red   = 128;
		rgb->green = 128;
		rgb->green = 0;
		return;
	}
	if(strcmp(color, "rebecca purple") == 0) {
		rgb->red   = 102;
		rgb->green = 51;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "RebeccaPurple") == 0) {
		rgb->red   = 102;
		rgb->green = 51;
		rgb->green = 153;
		return;
	}
	if(strcmp(color, "silver") == 0) {
		rgb->red   = 192;
		rgb->green = 192;
		rgb->green = 192;
		return;
	}
	if(strcmp(color, "teal") == 0) {
		rgb->red   = 0;
		rgb->green = 128;
		rgb->green = 128;
		return;
	}
	rgb->red   = 0;
	rgb->green = 0;
	rgb->blue  = 0;
}
