/* $Id$ */
#include <Mw/Milsko.h>

MwLLColor MwParseColorName(MwWidget handle, const char* color) {
	int r = 0, g = 0, b = 0;

	if(strcmp(color, "snow") == 0) {
		r = 255;
		g = 250;
		b = 250;
	}
	if(strcmp(color, "ghost white") == 0) {
		r = 248;
		g = 248;
		b = 255;
	}
	if(strcmp(color, "GhostWhite") == 0) {
		r = 248;
		g = 248;
		b = 255;
	}
	if(strcmp(color, "white smoke") == 0) {
		r = 245;
		g = 245;
		b = 245;
	}
	if(strcmp(color, "WhiteSmoke") == 0) {
		r = 245;
		g = 245;
		b = 245;
	}
	if(strcmp(color, "gainsboro") == 0) {
		r = 220;
		g = 220;
		b = 220;
	}
	if(strcmp(color, "floral white") == 0) {
		r = 255;
		g = 250;
		b = 240;
	}
	if(strcmp(color, "FloralWhite") == 0) {
		r = 255;
		g = 250;
		b = 240;
	}
	if(strcmp(color, "old lace") == 0) {
		r = 253;
		g = 245;
		b = 230;
	}
	if(strcmp(color, "OldLace") == 0) {
		r = 253;
		g = 245;
		b = 230;
	}
	if(strcmp(color, "linen") == 0) {
		r = 250;
		g = 240;
		b = 230;
	}
	if(strcmp(color, "antique white") == 0) {
		r = 250;
		g = 235;
		b = 215;
	}
	if(strcmp(color, "AntiqueWhite") == 0) {
		r = 250;
		g = 235;
		b = 215;
	}
	if(strcmp(color, "papaya whip") == 0) {
		r = 255;
		g = 239;
		b = 213;
	}
	if(strcmp(color, "PapayaWhip") == 0) {
		r = 255;
		g = 239;
		b = 213;
	}
	if(strcmp(color, "blanched almond") == 0) {
		r = 255;
		g = 235;
		b = 205;
	}
	if(strcmp(color, "BlanchedAlmond") == 0) {
		r = 255;
		g = 235;
		b = 205;
	}
	if(strcmp(color, "bisque") == 0) {
		r = 255;
		g = 228;
		b = 196;
	}
	if(strcmp(color, "peach puff") == 0) {
		r = 255;
		g = 218;
		b = 185;
	}
	if(strcmp(color, "PeachPuff") == 0) {
		r = 255;
		g = 218;
		b = 185;
	}
	if(strcmp(color, "navajo white") == 0) {
		r = 255;
		g = 222;
		b = 173;
	}
	if(strcmp(color, "NavajoWhite") == 0) {
		r = 255;
		g = 222;
		b = 173;
	}
	if(strcmp(color, "moccasin") == 0) {
		r = 255;
		g = 228;
		b = 181;
	}
	if(strcmp(color, "cornsilk") == 0) {
		r = 255;
		g = 248;
		b = 220;
	}
	if(strcmp(color, "ivory") == 0) {
		r = 255;
		g = 255;
		b = 240;
	}
	if(strcmp(color, "lemon chiffon") == 0) {
		r = 255;
		g = 250;
		b = 205;
	}
	if(strcmp(color, "LemonChiffon") == 0) {
		r = 255;
		g = 250;
		b = 205;
	}
	if(strcmp(color, "seashell") == 0) {
		r = 255;
		g = 245;
		b = 238;
	}
	if(strcmp(color, "honeydew") == 0) {
		r = 240;
		g = 255;
		b = 240;
	}
	if(strcmp(color, "mint cream") == 0) {
		r = 245;
		g = 255;
		b = 250;
	}
	if(strcmp(color, "MintCream") == 0) {
		r = 245;
		g = 255;
		b = 250;
	}
	if(strcmp(color, "azure") == 0) {
		r = 240;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "alice blue") == 0) {
		r = 240;
		g = 248;
		b = 255;
	}
	if(strcmp(color, "AliceBlue") == 0) {
		r = 240;
		g = 248;
		b = 255;
	}
	if(strcmp(color, "lavender") == 0) {
		r = 230;
		g = 230;
		b = 250;
	}
	if(strcmp(color, "lavender blush") == 0) {
		r = 255;
		g = 240;
		b = 245;
	}
	if(strcmp(color, "LavenderBlush") == 0) {
		r = 255;
		g = 240;
		b = 245;
	}
	if(strcmp(color, "misty rose") == 0) {
		r = 255;
		g = 228;
		b = 225;
	}
	if(strcmp(color, "MistyRose") == 0) {
		r = 255;
		g = 228;
		b = 225;
	}
	if(strcmp(color, "white") == 0) {
		r = 255;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "black") == 0) {
		r = 0;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "dark slate gray") == 0) {
		r = 47;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "DarkSlateGray") == 0) {
		r = 47;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "dark slate grey") == 0) {
		r = 47;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "DarkSlateGrey") == 0) {
		r = 47;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "dim gray") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "DimGray") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "dim grey") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "DimGrey") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "slate gray") == 0) {
		r = 112;
		g = 128;
		b = 144;
	}
	if(strcmp(color, "SlateGray") == 0) {
		r = 112;
		g = 128;
		b = 144;
	}
	if(strcmp(color, "slate grey") == 0) {
		r = 112;
		g = 128;
		b = 144;
	}
	if(strcmp(color, "SlateGrey") == 0) {
		r = 112;
		g = 128;
		b = 144;
	}
	if(strcmp(color, "light slate gray") == 0) {
		r = 119;
		g = 136;
		b = 153;
	}
	if(strcmp(color, "LightSlateGray") == 0) {
		r = 119;
		g = 136;
		b = 153;
	}
	if(strcmp(color, "light slate grey") == 0) {
		r = 119;
		g = 136;
		b = 153;
	}
	if(strcmp(color, "LightSlateGrey") == 0) {
		r = 119;
		g = 136;
		b = 153;
	}
	if(strcmp(color, "gray") == 0) {
		r = 190;
		g = 190;
		b = 190;
	}
	if(strcmp(color, "grey") == 0) {
		r = 190;
		g = 190;
		b = 190;
	}
	if(strcmp(color, "light grey") == 0) {
		r = 211;
		g = 211;
		b = 211;
	}
	if(strcmp(color, "LightGrey") == 0) {
		r = 211;
		g = 211;
		b = 211;
	}
	if(strcmp(color, "light gray") == 0) {
		r = 211;
		g = 211;
		b = 211;
	}
	if(strcmp(color, "LightGray") == 0) {
		r = 211;
		g = 211;
		b = 211;
	}
	if(strcmp(color, "midnight blue") == 0) {
		r = 25;
		g = 25;
		b = 112;
	}
	if(strcmp(color, "MidnightBlue") == 0) {
		r = 25;
		g = 25;
		b = 112;
	}
	if(strcmp(color, "navy") == 0) {
		r = 0;
		g = 0;
		b = 128;
	}
	if(strcmp(color, "navy blue") == 0) {
		r = 0;
		g = 0;
		b = 128;
	}
	if(strcmp(color, "NavyBlue") == 0) {
		r = 0;
		g = 0;
		b = 128;
	}
	if(strcmp(color, "cornflower blue") == 0) {
		r = 100;
		g = 149;
		b = 237;
	}
	if(strcmp(color, "CornflowerBlue") == 0) {
		r = 100;
		g = 149;
		b = 237;
	}
	if(strcmp(color, "dark slate blue") == 0) {
		r = 72;
		g = 61;
		b = 139;
	}
	if(strcmp(color, "DarkSlateBlue") == 0) {
		r = 72;
		g = 61;
		b = 139;
	}
	if(strcmp(color, "slate blue") == 0) {
		r = 106;
		g = 90;
		b = 205;
	}
	if(strcmp(color, "SlateBlue") == 0) {
		r = 106;
		g = 90;
		b = 205;
	}
	if(strcmp(color, "medium slate blue") == 0) {
		r = 123;
		g = 104;
		b = 238;
	}
	if(strcmp(color, "MediumSlateBlue") == 0) {
		r = 123;
		g = 104;
		b = 238;
	}
	if(strcmp(color, "light slate blue") == 0) {
		r = 132;
		g = 112;
		b = 255;
	}
	if(strcmp(color, "LightSlateBlue") == 0) {
		r = 132;
		g = 112;
		b = 255;
	}
	if(strcmp(color, "medium blue") == 0) {
		r = 0;
		g = 0;
		b = 205;
	}
	if(strcmp(color, "MediumBlue") == 0) {
		r = 0;
		g = 0;
		b = 205;
	}
	if(strcmp(color, "royal blue") == 0) {
		r = 65;
		g = 105;
		b = 225;
	}
	if(strcmp(color, "RoyalBlue") == 0) {
		r = 65;
		g = 105;
		b = 225;
	}
	if(strcmp(color, "blue") == 0) {
		r = 0;
		g = 0;
		b = 255;
	}
	if(strcmp(color, "dodger blue") == 0) {
		r = 30;
		g = 144;
		b = 255;
	}
	if(strcmp(color, "DodgerBlue") == 0) {
		r = 30;
		g = 144;
		b = 255;
	}
	if(strcmp(color, "deep sky blue") == 0) {
		r = 0;
		g = 191;
		b = 255;
	}
	if(strcmp(color, "DeepSkyBlue") == 0) {
		r = 0;
		g = 191;
		b = 255;
	}
	if(strcmp(color, "sky blue") == 0) {
		r = 135;
		g = 206;
		b = 235;
	}
	if(strcmp(color, "SkyBlue") == 0) {
		r = 135;
		g = 206;
		b = 235;
	}
	if(strcmp(color, "light sky blue") == 0) {
		r = 135;
		g = 206;
		b = 250;
	}
	if(strcmp(color, "LightSkyBlue") == 0) {
		r = 135;
		g = 206;
		b = 250;
	}
	if(strcmp(color, "steel blue") == 0) {
		r = 70;
		g = 130;
		b = 180;
	}
	if(strcmp(color, "SteelBlue") == 0) {
		r = 70;
		g = 130;
		b = 180;
	}
	if(strcmp(color, "light steel blue") == 0) {
		r = 176;
		g = 196;
		b = 222;
	}
	if(strcmp(color, "LightSteelBlue") == 0) {
		r = 176;
		g = 196;
		b = 222;
	}
	if(strcmp(color, "light blue") == 0) {
		r = 173;
		g = 216;
		b = 230;
	}
	if(strcmp(color, "LightBlue") == 0) {
		r = 173;
		g = 216;
		b = 230;
	}
	if(strcmp(color, "powder blue") == 0) {
		r = 176;
		g = 224;
		b = 230;
	}
	if(strcmp(color, "PowderBlue") == 0) {
		r = 176;
		g = 224;
		b = 230;
	}
	if(strcmp(color, "pale turquoise") == 0) {
		r = 175;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "PaleTurquoise") == 0) {
		r = 175;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "dark turquoise") == 0) {
		r = 0;
		g = 206;
		b = 209;
	}
	if(strcmp(color, "DarkTurquoise") == 0) {
		r = 0;
		g = 206;
		b = 209;
	}
	if(strcmp(color, "medium turquoise") == 0) {
		r = 72;
		g = 209;
		b = 204;
	}
	if(strcmp(color, "MediumTurquoise") == 0) {
		r = 72;
		g = 209;
		b = 204;
	}
	if(strcmp(color, "turquoise") == 0) {
		r = 64;
		g = 224;
		b = 208;
	}
	if(strcmp(color, "cyan") == 0) {
		r = 0;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "light cyan") == 0) {
		r = 224;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "LightCyan") == 0) {
		r = 224;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "cadet blue") == 0) {
		r = 95;
		g = 158;
		b = 160;
	}
	if(strcmp(color, "CadetBlue") == 0) {
		r = 95;
		g = 158;
		b = 160;
	}
	if(strcmp(color, "medium aquamarine") == 0) {
		r = 102;
		g = 205;
		b = 170;
	}
	if(strcmp(color, "MediumAquamarine") == 0) {
		r = 102;
		g = 205;
		b = 170;
	}
	if(strcmp(color, "aquamarine") == 0) {
		r = 127;
		g = 255;
		b = 212;
	}
	if(strcmp(color, "dark green") == 0) {
		r = 0;
		g = 100;
		b = 0;
	}
	if(strcmp(color, "DarkGreen") == 0) {
		r = 0;
		g = 100;
		b = 0;
	}
	if(strcmp(color, "dark olive green") == 0) {
		r = 85;
		g = 107;
		b = 47;
	}
	if(strcmp(color, "DarkOliveGreen") == 0) {
		r = 85;
		g = 107;
		b = 47;
	}
	if(strcmp(color, "dark sea green") == 0) {
		r = 143;
		g = 188;
		b = 143;
	}
	if(strcmp(color, "DarkSeaGreen") == 0) {
		r = 143;
		g = 188;
		b = 143;
	}
	if(strcmp(color, "sea green") == 0) {
		r = 46;
		g = 139;
		b = 87;
	}
	if(strcmp(color, "SeaGreen") == 0) {
		r = 46;
		g = 139;
		b = 87;
	}
	if(strcmp(color, "medium sea green") == 0) {
		r = 60;
		g = 179;
		b = 113;
	}
	if(strcmp(color, "MediumSeaGreen") == 0) {
		r = 60;
		g = 179;
		b = 113;
	}
	if(strcmp(color, "light sea green") == 0) {
		r = 32;
		g = 178;
		b = 170;
	}
	if(strcmp(color, "LightSeaGreen") == 0) {
		r = 32;
		g = 178;
		b = 170;
	}
	if(strcmp(color, "pale green") == 0) {
		r = 152;
		g = 251;
		b = 152;
	}
	if(strcmp(color, "PaleGreen") == 0) {
		r = 152;
		g = 251;
		b = 152;
	}
	if(strcmp(color, "spring green") == 0) {
		r = 0;
		g = 255;
		b = 127;
	}
	if(strcmp(color, "SpringGreen") == 0) {
		r = 0;
		g = 255;
		b = 127;
	}
	if(strcmp(color, "lawn green") == 0) {
		r = 124;
		g = 252;
		b = 0;
	}
	if(strcmp(color, "LawnGreen") == 0) {
		r = 124;
		g = 252;
		b = 0;
	}
	if(strcmp(color, "green") == 0) {
		r = 0;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "chartreuse") == 0) {
		r = 127;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "medium spring green") == 0) {
		r = 0;
		g = 250;
		b = 154;
	}
	if(strcmp(color, "MediumSpringGreen") == 0) {
		r = 0;
		g = 250;
		b = 154;
	}
	if(strcmp(color, "green yellow") == 0) {
		r = 173;
		g = 255;
		b = 47;
	}
	if(strcmp(color, "GreenYellow") == 0) {
		r = 173;
		g = 255;
		b = 47;
	}
	if(strcmp(color, "lime green") == 0) {
		r = 50;
		g = 205;
		b = 50;
	}
	if(strcmp(color, "LimeGreen") == 0) {
		r = 50;
		g = 205;
		b = 50;
	}
	if(strcmp(color, "yellow green") == 0) {
		r = 154;
		g = 205;
		b = 50;
	}
	if(strcmp(color, "YellowGreen") == 0) {
		r = 154;
		g = 205;
		b = 50;
	}
	if(strcmp(color, "forest green") == 0) {
		r = 34;
		g = 139;
		b = 34;
	}
	if(strcmp(color, "ForestGreen") == 0) {
		r = 34;
		g = 139;
		b = 34;
	}
	if(strcmp(color, "olive drab") == 0) {
		r = 107;
		g = 142;
		b = 35;
	}
	if(strcmp(color, "OliveDrab") == 0) {
		r = 107;
		g = 142;
		b = 35;
	}
	if(strcmp(color, "dark khaki") == 0) {
		r = 189;
		g = 183;
		b = 107;
	}
	if(strcmp(color, "DarkKhaki") == 0) {
		r = 189;
		g = 183;
		b = 107;
	}
	if(strcmp(color, "khaki") == 0) {
		r = 240;
		g = 230;
		b = 140;
	}
	if(strcmp(color, "pale goldenrod") == 0) {
		r = 238;
		g = 232;
		b = 170;
	}
	if(strcmp(color, "PaleGoldenrod") == 0) {
		r = 238;
		g = 232;
		b = 170;
	}
	if(strcmp(color, "light goldenrod yellow") == 0) {
		r = 250;
		g = 250;
		b = 210;
	}
	if(strcmp(color, "LightGoldenrodYellow") == 0) {
		r = 250;
		g = 250;
		b = 210;
	}
	if(strcmp(color, "light yellow") == 0) {
		r = 255;
		g = 255;
		b = 224;
	}
	if(strcmp(color, "LightYellow") == 0) {
		r = 255;
		g = 255;
		b = 224;
	}
	if(strcmp(color, "yellow") == 0) {
		r = 255;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "gold") == 0) {
		r = 255;
		g = 215;
		b = 0;
	}
	if(strcmp(color, "light goldenrod") == 0) {
		r = 238;
		g = 221;
		b = 130;
	}
	if(strcmp(color, "LightGoldenrod") == 0) {
		r = 238;
		g = 221;
		b = 130;
	}
	if(strcmp(color, "goldenrod") == 0) {
		r = 218;
		g = 165;
		b = 32;
	}
	if(strcmp(color, "dark goldenrod") == 0) {
		r = 184;
		g = 134;
		b = 11;
	}
	if(strcmp(color, "DarkGoldenrod") == 0) {
		r = 184;
		g = 134;
		b = 11;
	}
	if(strcmp(color, "rosy brown") == 0) {
		r = 188;
		g = 143;
		b = 143;
	}
	if(strcmp(color, "RosyBrown") == 0) {
		r = 188;
		g = 143;
		b = 143;
	}
	if(strcmp(color, "indian red") == 0) {
		r = 205;
		g = 92;
		b = 92;
	}
	if(strcmp(color, "IndianRed") == 0) {
		r = 205;
		g = 92;
		b = 92;
	}
	if(strcmp(color, "saddle brown") == 0) {
		r = 139;
		g = 69;
		b = 19;
	}
	if(strcmp(color, "SaddleBrown") == 0) {
		r = 139;
		g = 69;
		b = 19;
	}
	if(strcmp(color, "sienna") == 0) {
		r = 160;
		g = 82;
		b = 45;
	}
	if(strcmp(color, "peru") == 0) {
		r = 205;
		g = 133;
		b = 63;
	}
	if(strcmp(color, "burlywood") == 0) {
		r = 222;
		g = 184;
		b = 135;
	}
	if(strcmp(color, "beige") == 0) {
		r = 245;
		g = 245;
		b = 220;
	}
	if(strcmp(color, "wheat") == 0) {
		r = 245;
		g = 222;
		b = 179;
	}
	if(strcmp(color, "sandy brown") == 0) {
		r = 244;
		g = 164;
		b = 96;
	}
	if(strcmp(color, "SandyBrown") == 0) {
		r = 244;
		g = 164;
		b = 96;
	}
	if(strcmp(color, "tan") == 0) {
		r = 210;
		g = 180;
		b = 140;
	}
	if(strcmp(color, "chocolate") == 0) {
		r = 210;
		g = 105;
		b = 30;
	}
	if(strcmp(color, "firebrick") == 0) {
		r = 178;
		g = 34;
		b = 34;
	}
	if(strcmp(color, "brown") == 0) {
		r = 165;
		g = 42;
		b = 42;
	}
	if(strcmp(color, "dark salmon") == 0) {
		r = 233;
		g = 150;
		b = 122;
	}
	if(strcmp(color, "DarkSalmon") == 0) {
		r = 233;
		g = 150;
		b = 122;
	}
	if(strcmp(color, "salmon") == 0) {
		r = 250;
		g = 128;
		b = 114;
	}
	if(strcmp(color, "light salmon") == 0) {
		r = 255;
		g = 160;
		b = 122;
	}
	if(strcmp(color, "LightSalmon") == 0) {
		r = 255;
		g = 160;
		b = 122;
	}
	if(strcmp(color, "orange") == 0) {
		r = 255;
		g = 165;
		b = 0;
	}
	if(strcmp(color, "dark orange") == 0) {
		r = 255;
		g = 140;
		b = 0;
	}
	if(strcmp(color, "DarkOrange") == 0) {
		r = 255;
		g = 140;
		b = 0;
	}
	if(strcmp(color, "coral") == 0) {
		r = 255;
		g = 127;
		b = 80;
	}
	if(strcmp(color, "light coral") == 0) {
		r = 240;
		g = 128;
		b = 128;
	}
	if(strcmp(color, "LightCoral") == 0) {
		r = 240;
		g = 128;
		b = 128;
	}
	if(strcmp(color, "tomato") == 0) {
		r = 255;
		g = 99;
		b = 71;
	}
	if(strcmp(color, "orange red") == 0) {
		r = 255;
		g = 69;
		b = 0;
	}
	if(strcmp(color, "OrangeRed") == 0) {
		r = 255;
		g = 69;
		b = 0;
	}
	if(strcmp(color, "red") == 0) {
		r = 255;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "hot pink") == 0) {
		r = 255;
		g = 105;
		b = 180;
	}
	if(strcmp(color, "HotPink") == 0) {
		r = 255;
		g = 105;
		b = 180;
	}
	if(strcmp(color, "deep pink") == 0) {
		r = 255;
		g = 20;
		b = 147;
	}
	if(strcmp(color, "DeepPink") == 0) {
		r = 255;
		g = 20;
		b = 147;
	}
	if(strcmp(color, "pink") == 0) {
		r = 255;
		g = 192;
		b = 203;
	}
	if(strcmp(color, "light pink") == 0) {
		r = 255;
		g = 182;
		b = 193;
	}
	if(strcmp(color, "LightPink") == 0) {
		r = 255;
		g = 182;
		b = 193;
	}
	if(strcmp(color, "pale violet red") == 0) {
		r = 219;
		g = 112;
		b = 147;
	}
	if(strcmp(color, "PaleVioletRed") == 0) {
		r = 219;
		g = 112;
		b = 147;
	}
	if(strcmp(color, "maroon") == 0) {
		r = 176;
		g = 48;
		b = 96;
	}
	if(strcmp(color, "medium violet red") == 0) {
		r = 199;
		g = 21;
		b = 133;
	}
	if(strcmp(color, "MediumVioletRed") == 0) {
		r = 199;
		g = 21;
		b = 133;
	}
	if(strcmp(color, "violet red") == 0) {
		r = 208;
		g = 32;
		b = 144;
	}
	if(strcmp(color, "VioletRed") == 0) {
		r = 208;
		g = 32;
		b = 144;
	}
	if(strcmp(color, "magenta") == 0) {
		r = 255;
		g = 0;
		b = 255;
	}
	if(strcmp(color, "violet") == 0) {
		r = 238;
		g = 130;
		b = 238;
	}
	if(strcmp(color, "plum") == 0) {
		r = 221;
		g = 160;
		b = 221;
	}
	if(strcmp(color, "orchid") == 0) {
		r = 218;
		g = 112;
		b = 214;
	}
	if(strcmp(color, "medium orchid") == 0) {
		r = 186;
		g = 85;
		b = 211;
	}
	if(strcmp(color, "MediumOrchid") == 0) {
		r = 186;
		g = 85;
		b = 211;
	}
	if(strcmp(color, "dark orchid") == 0) {
		r = 153;
		g = 50;
		b = 204;
	}
	if(strcmp(color, "DarkOrchid") == 0) {
		r = 153;
		g = 50;
		b = 204;
	}
	if(strcmp(color, "dark violet") == 0) {
		r = 148;
		g = 0;
		b = 211;
	}
	if(strcmp(color, "DarkViolet") == 0) {
		r = 148;
		g = 0;
		b = 211;
	}
	if(strcmp(color, "blue violet") == 0) {
		r = 138;
		g = 43;
		b = 226;
	}
	if(strcmp(color, "BlueViolet") == 0) {
		r = 138;
		g = 43;
		b = 226;
	}
	if(strcmp(color, "purple") == 0) {
		r = 160;
		g = 32;
		b = 240;
	}
	if(strcmp(color, "medium purple") == 0) {
		r = 147;
		g = 112;
		b = 219;
	}
	if(strcmp(color, "MediumPurple") == 0) {
		r = 147;
		g = 112;
		b = 219;
	}
	if(strcmp(color, "thistle") == 0) {
		r = 216;
		g = 191;
		b = 216;
	}
	if(strcmp(color, "snow1") == 0) {
		r = 255;
		g = 250;
		b = 250;
	}
	if(strcmp(color, "snow2") == 0) {
		r = 238;
		g = 233;
		b = 233;
	}
	if(strcmp(color, "snow3") == 0) {
		r = 205;
		g = 201;
		b = 201;
	}
	if(strcmp(color, "snow4") == 0) {
		r = 139;
		g = 137;
		b = 137;
	}
	if(strcmp(color, "seashell1") == 0) {
		r = 255;
		g = 245;
		b = 238;
	}
	if(strcmp(color, "seashell2") == 0) {
		r = 238;
		g = 229;
		b = 222;
	}
	if(strcmp(color, "seashell3") == 0) {
		r = 205;
		g = 197;
		b = 191;
	}
	if(strcmp(color, "seashell4") == 0) {
		r = 139;
		g = 134;
		b = 130;
	}
	if(strcmp(color, "AntiqueWhite1") == 0) {
		r = 255;
		g = 239;
		b = 219;
	}
	if(strcmp(color, "AntiqueWhite2") == 0) {
		r = 238;
		g = 223;
		b = 204;
	}
	if(strcmp(color, "AntiqueWhite3") == 0) {
		r = 205;
		g = 192;
		b = 176;
	}
	if(strcmp(color, "AntiqueWhite4") == 0) {
		r = 139;
		g = 131;
		b = 120;
	}
	if(strcmp(color, "bisque1") == 0) {
		r = 255;
		g = 228;
		b = 196;
	}
	if(strcmp(color, "bisque2") == 0) {
		r = 238;
		g = 213;
		b = 183;
	}
	if(strcmp(color, "bisque3") == 0) {
		r = 205;
		g = 183;
		b = 158;
	}
	if(strcmp(color, "bisque4") == 0) {
		r = 139;
		g = 125;
		b = 107;
	}
	if(strcmp(color, "PeachPuff1") == 0) {
		r = 255;
		g = 218;
		b = 185;
	}
	if(strcmp(color, "PeachPuff2") == 0) {
		r = 238;
		g = 203;
		b = 173;
	}
	if(strcmp(color, "PeachPuff3") == 0) {
		r = 205;
		g = 175;
		b = 149;
	}
	if(strcmp(color, "PeachPuff4") == 0) {
		r = 139;
		g = 119;
		b = 101;
	}
	if(strcmp(color, "NavajoWhite1") == 0) {
		r = 255;
		g = 222;
		b = 173;
	}
	if(strcmp(color, "NavajoWhite2") == 0) {
		r = 238;
		g = 207;
		b = 161;
	}
	if(strcmp(color, "NavajoWhite3") == 0) {
		r = 205;
		g = 179;
		b = 139;
	}
	if(strcmp(color, "NavajoWhite4") == 0) {
		r = 139;
		g = 121;
		b = 94;
	}
	if(strcmp(color, "LemonChiffon1") == 0) {
		r = 255;
		g = 250;
		b = 205;
	}
	if(strcmp(color, "LemonChiffon2") == 0) {
		r = 238;
		g = 233;
		b = 191;
	}
	if(strcmp(color, "LemonChiffon3") == 0) {
		r = 205;
		g = 201;
		b = 165;
	}
	if(strcmp(color, "LemonChiffon4") == 0) {
		r = 139;
		g = 137;
		b = 112;
	}
	if(strcmp(color, "cornsilk1") == 0) {
		r = 255;
		g = 248;
		b = 220;
	}
	if(strcmp(color, "cornsilk2") == 0) {
		r = 238;
		g = 232;
		b = 205;
	}
	if(strcmp(color, "cornsilk3") == 0) {
		r = 205;
		g = 200;
		b = 177;
	}
	if(strcmp(color, "cornsilk4") == 0) {
		r = 139;
		g = 136;
		b = 120;
	}
	if(strcmp(color, "ivory1") == 0) {
		r = 255;
		g = 255;
		b = 240;
	}
	if(strcmp(color, "ivory2") == 0) {
		r = 238;
		g = 238;
		b = 224;
	}
	if(strcmp(color, "ivory3") == 0) {
		r = 205;
		g = 205;
		b = 193;
	}
	if(strcmp(color, "ivory4") == 0) {
		r = 139;
		g = 139;
		b = 131;
	}
	if(strcmp(color, "honeydew1") == 0) {
		r = 240;
		g = 255;
		b = 240;
	}
	if(strcmp(color, "honeydew2") == 0) {
		r = 224;
		g = 238;
		b = 224;
	}
	if(strcmp(color, "honeydew3") == 0) {
		r = 193;
		g = 205;
		b = 193;
	}
	if(strcmp(color, "honeydew4") == 0) {
		r = 131;
		g = 139;
		b = 131;
	}
	if(strcmp(color, "LavenderBlush1") == 0) {
		r = 255;
		g = 240;
		b = 245;
	}
	if(strcmp(color, "LavenderBlush2") == 0) {
		r = 238;
		g = 224;
		b = 229;
	}
	if(strcmp(color, "LavenderBlush3") == 0) {
		r = 205;
		g = 193;
		b = 197;
	}
	if(strcmp(color, "LavenderBlush4") == 0) {
		r = 139;
		g = 131;
		b = 134;
	}
	if(strcmp(color, "MistyRose1") == 0) {
		r = 255;
		g = 228;
		b = 225;
	}
	if(strcmp(color, "MistyRose2") == 0) {
		r = 238;
		g = 213;
		b = 210;
	}
	if(strcmp(color, "MistyRose3") == 0) {
		r = 205;
		g = 183;
		b = 181;
	}
	if(strcmp(color, "MistyRose4") == 0) {
		r = 139;
		g = 125;
		b = 123;
	}
	if(strcmp(color, "azure1") == 0) {
		r = 240;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "azure2") == 0) {
		r = 224;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "azure3") == 0) {
		r = 193;
		g = 205;
		b = 205;
	}
	if(strcmp(color, "azure4") == 0) {
		r = 131;
		g = 139;
		b = 139;
	}
	if(strcmp(color, "SlateBlue1") == 0) {
		r = 131;
		g = 111;
		b = 255;
	}
	if(strcmp(color, "SlateBlue2") == 0) {
		r = 122;
		g = 103;
		b = 238;
	}
	if(strcmp(color, "SlateBlue3") == 0) {
		r = 105;
		g = 89;
		b = 205;
	}
	if(strcmp(color, "SlateBlue4") == 0) {
		r = 71;
		g = 60;
		b = 139;
	}
	if(strcmp(color, "RoyalBlue1") == 0) {
		r = 72;
		g = 118;
		b = 255;
	}
	if(strcmp(color, "RoyalBlue2") == 0) {
		r = 67;
		g = 110;
		b = 238;
	}
	if(strcmp(color, "RoyalBlue3") == 0) {
		r = 58;
		g = 95;
		b = 205;
	}
	if(strcmp(color, "RoyalBlue4") == 0) {
		r = 39;
		g = 64;
		b = 139;
	}
	if(strcmp(color, "blue1") == 0) {
		r = 0;
		g = 0;
		b = 255;
	}
	if(strcmp(color, "blue2") == 0) {
		r = 0;
		g = 0;
		b = 238;
	}
	if(strcmp(color, "blue3") == 0) {
		r = 0;
		g = 0;
		b = 205;
	}
	if(strcmp(color, "blue4") == 0) {
		r = 0;
		g = 0;
		b = 139;
	}
	if(strcmp(color, "DodgerBlue1") == 0) {
		r = 30;
		g = 144;
		b = 255;
	}
	if(strcmp(color, "DodgerBlue2") == 0) {
		r = 28;
		g = 134;
		b = 238;
	}
	if(strcmp(color, "DodgerBlue3") == 0) {
		r = 24;
		g = 116;
		b = 205;
	}
	if(strcmp(color, "DodgerBlue4") == 0) {
		r = 16;
		g = 78;
		b = 139;
	}
	if(strcmp(color, "SteelBlue1") == 0) {
		r = 99;
		g = 184;
		b = 255;
	}
	if(strcmp(color, "SteelBlue2") == 0) {
		r = 92;
		g = 172;
		b = 238;
	}
	if(strcmp(color, "SteelBlue3") == 0) {
		r = 79;
		g = 148;
		b = 205;
	}
	if(strcmp(color, "SteelBlue4") == 0) {
		r = 54;
		g = 100;
		b = 139;
	}
	if(strcmp(color, "DeepSkyBlue1") == 0) {
		r = 0;
		g = 191;
		b = 255;
	}
	if(strcmp(color, "DeepSkyBlue2") == 0) {
		r = 0;
		g = 178;
		b = 238;
	}
	if(strcmp(color, "DeepSkyBlue3") == 0) {
		r = 0;
		g = 154;
		b = 205;
	}
	if(strcmp(color, "DeepSkyBlue4") == 0) {
		r = 0;
		g = 104;
		b = 139;
	}
	if(strcmp(color, "SkyBlue1") == 0) {
		r = 135;
		g = 206;
		b = 255;
	}
	if(strcmp(color, "SkyBlue2") == 0) {
		r = 126;
		g = 192;
		b = 238;
	}
	if(strcmp(color, "SkyBlue3") == 0) {
		r = 108;
		g = 166;
		b = 205;
	}
	if(strcmp(color, "SkyBlue4") == 0) {
		r = 74;
		g = 112;
		b = 139;
	}
	if(strcmp(color, "LightSkyBlue1") == 0) {
		r = 176;
		g = 226;
		b = 255;
	}
	if(strcmp(color, "LightSkyBlue2") == 0) {
		r = 164;
		g = 211;
		b = 238;
	}
	if(strcmp(color, "LightSkyBlue3") == 0) {
		r = 141;
		g = 182;
		b = 205;
	}
	if(strcmp(color, "LightSkyBlue4") == 0) {
		r = 96;
		g = 123;
		b = 139;
	}
	if(strcmp(color, "SlateGray1") == 0) {
		r = 198;
		g = 226;
		b = 255;
	}
	if(strcmp(color, "SlateGray2") == 0) {
		r = 185;
		g = 211;
		b = 238;
	}
	if(strcmp(color, "SlateGray3") == 0) {
		r = 159;
		g = 182;
		b = 205;
	}
	if(strcmp(color, "SlateGray4") == 0) {
		r = 108;
		g = 123;
		b = 139;
	}
	if(strcmp(color, "LightSteelBlue1") == 0) {
		r = 202;
		g = 225;
		b = 255;
	}
	if(strcmp(color, "LightSteelBlue2") == 0) {
		r = 188;
		g = 210;
		b = 238;
	}
	if(strcmp(color, "LightSteelBlue3") == 0) {
		r = 162;
		g = 181;
		b = 205;
	}
	if(strcmp(color, "LightSteelBlue4") == 0) {
		r = 110;
		g = 123;
		b = 139;
	}
	if(strcmp(color, "LightBlue1") == 0) {
		r = 191;
		g = 239;
		b = 255;
	}
	if(strcmp(color, "LightBlue2") == 0) {
		r = 178;
		g = 223;
		b = 238;
	}
	if(strcmp(color, "LightBlue3") == 0) {
		r = 154;
		g = 192;
		b = 205;
	}
	if(strcmp(color, "LightBlue4") == 0) {
		r = 104;
		g = 131;
		b = 139;
	}
	if(strcmp(color, "LightCyan1") == 0) {
		r = 224;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "LightCyan2") == 0) {
		r = 209;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "LightCyan3") == 0) {
		r = 180;
		g = 205;
		b = 205;
	}
	if(strcmp(color, "LightCyan4") == 0) {
		r = 122;
		g = 139;
		b = 139;
	}
	if(strcmp(color, "PaleTurquoise1") == 0) {
		r = 187;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "PaleTurquoise2") == 0) {
		r = 174;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "PaleTurquoise3") == 0) {
		r = 150;
		g = 205;
		b = 205;
	}
	if(strcmp(color, "PaleTurquoise4") == 0) {
		r = 102;
		g = 139;
		b = 139;
	}
	if(strcmp(color, "CadetBlue1") == 0) {
		r = 152;
		g = 245;
		b = 255;
	}
	if(strcmp(color, "CadetBlue2") == 0) {
		r = 142;
		g = 229;
		b = 238;
	}
	if(strcmp(color, "CadetBlue3") == 0) {
		r = 122;
		g = 197;
		b = 205;
	}
	if(strcmp(color, "CadetBlue4") == 0) {
		r = 83;
		g = 134;
		b = 139;
	}
	if(strcmp(color, "turquoise1") == 0) {
		r = 0;
		g = 245;
		b = 255;
	}
	if(strcmp(color, "turquoise2") == 0) {
		r = 0;
		g = 229;
		b = 238;
	}
	if(strcmp(color, "turquoise3") == 0) {
		r = 0;
		g = 197;
		b = 205;
	}
	if(strcmp(color, "turquoise4") == 0) {
		r = 0;
		g = 134;
		b = 139;
	}
	if(strcmp(color, "cyan1") == 0) {
		r = 0;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "cyan2") == 0) {
		r = 0;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "cyan3") == 0) {
		r = 0;
		g = 205;
		b = 205;
	}
	if(strcmp(color, "cyan4") == 0) {
		r = 0;
		g = 139;
		b = 139;
	}
	if(strcmp(color, "DarkSlateGray1") == 0) {
		r = 151;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "DarkSlateGray2") == 0) {
		r = 141;
		g = 238;
		b = 238;
	}
	if(strcmp(color, "DarkSlateGray3") == 0) {
		r = 121;
		g = 205;
		b = 205;
	}
	if(strcmp(color, "DarkSlateGray4") == 0) {
		r = 82;
		g = 139;
		b = 139;
	}
	if(strcmp(color, "aquamarine1") == 0) {
		r = 127;
		g = 255;
		b = 212;
	}
	if(strcmp(color, "aquamarine2") == 0) {
		r = 118;
		g = 238;
		b = 198;
	}
	if(strcmp(color, "aquamarine3") == 0) {
		r = 102;
		g = 205;
		b = 170;
	}
	if(strcmp(color, "aquamarine4") == 0) {
		r = 69;
		g = 139;
		b = 116;
	}
	if(strcmp(color, "DarkSeaGreen1") == 0) {
		r = 193;
		g = 255;
		b = 193;
	}
	if(strcmp(color, "DarkSeaGreen2") == 0) {
		r = 180;
		g = 238;
		b = 180;
	}
	if(strcmp(color, "DarkSeaGreen3") == 0) {
		r = 155;
		g = 205;
		b = 155;
	}
	if(strcmp(color, "DarkSeaGreen4") == 0) {
		r = 105;
		g = 139;
		b = 105;
	}
	if(strcmp(color, "SeaGreen1") == 0) {
		r = 84;
		g = 255;
		b = 159;
	}
	if(strcmp(color, "SeaGreen2") == 0) {
		r = 78;
		g = 238;
		b = 148;
	}
	if(strcmp(color, "SeaGreen3") == 0) {
		r = 67;
		g = 205;
		b = 128;
	}
	if(strcmp(color, "SeaGreen4") == 0) {
		r = 46;
		g = 139;
		b = 87;
	}
	if(strcmp(color, "PaleGreen1") == 0) {
		r = 154;
		g = 255;
		b = 154;
	}
	if(strcmp(color, "PaleGreen2") == 0) {
		r = 144;
		g = 238;
		b = 144;
	}
	if(strcmp(color, "PaleGreen3") == 0) {
		r = 124;
		g = 205;
		b = 124;
	}
	if(strcmp(color, "PaleGreen4") == 0) {
		r = 84;
		g = 139;
		b = 84;
	}
	if(strcmp(color, "SpringGreen1") == 0) {
		r = 0;
		g = 255;
		b = 127;
	}
	if(strcmp(color, "SpringGreen2") == 0) {
		r = 0;
		g = 238;
		b = 118;
	}
	if(strcmp(color, "SpringGreen3") == 0) {
		r = 0;
		g = 205;
		b = 102;
	}
	if(strcmp(color, "SpringGreen4") == 0) {
		r = 0;
		g = 139;
		b = 69;
	}
	if(strcmp(color, "green1") == 0) {
		r = 0;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "green2") == 0) {
		r = 0;
		g = 238;
		b = 0;
	}
	if(strcmp(color, "green3") == 0) {
		r = 0;
		g = 205;
		b = 0;
	}
	if(strcmp(color, "green4") == 0) {
		r = 0;
		g = 139;
		b = 0;
	}
	if(strcmp(color, "chartreuse1") == 0) {
		r = 127;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "chartreuse2") == 0) {
		r = 118;
		g = 238;
		b = 0;
	}
	if(strcmp(color, "chartreuse3") == 0) {
		r = 102;
		g = 205;
		b = 0;
	}
	if(strcmp(color, "chartreuse4") == 0) {
		r = 69;
		g = 139;
		b = 0;
	}
	if(strcmp(color, "OliveDrab1") == 0) {
		r = 192;
		g = 255;
		b = 62;
	}
	if(strcmp(color, "OliveDrab2") == 0) {
		r = 179;
		g = 238;
		b = 58;
	}
	if(strcmp(color, "OliveDrab3") == 0) {
		r = 154;
		g = 205;
		b = 50;
	}
	if(strcmp(color, "OliveDrab4") == 0) {
		r = 105;
		g = 139;
		b = 34;
	}
	if(strcmp(color, "DarkOliveGreen1") == 0) {
		r = 202;
		g = 255;
		b = 112;
	}
	if(strcmp(color, "DarkOliveGreen2") == 0) {
		r = 188;
		g = 238;
		b = 104;
	}
	if(strcmp(color, "DarkOliveGreen3") == 0) {
		r = 162;
		g = 205;
		b = 90;
	}
	if(strcmp(color, "DarkOliveGreen4") == 0) {
		r = 110;
		g = 139;
		b = 61;
	}
	if(strcmp(color, "khaki1") == 0) {
		r = 255;
		g = 246;
		b = 143;
	}
	if(strcmp(color, "khaki2") == 0) {
		r = 238;
		g = 230;
		b = 133;
	}
	if(strcmp(color, "khaki3") == 0) {
		r = 205;
		g = 198;
		b = 115;
	}
	if(strcmp(color, "khaki4") == 0) {
		r = 139;
		g = 134;
		b = 78;
	}
	if(strcmp(color, "LightGoldenrod1") == 0) {
		r = 255;
		g = 236;
		b = 139;
	}
	if(strcmp(color, "LightGoldenrod2") == 0) {
		r = 238;
		g = 220;
		b = 130;
	}
	if(strcmp(color, "LightGoldenrod3") == 0) {
		r = 205;
		g = 190;
		b = 112;
	}
	if(strcmp(color, "LightGoldenrod4") == 0) {
		r = 139;
		g = 129;
		b = 76;
	}
	if(strcmp(color, "LightYellow1") == 0) {
		r = 255;
		g = 255;
		b = 224;
	}
	if(strcmp(color, "LightYellow2") == 0) {
		r = 238;
		g = 238;
		b = 209;
	}
	if(strcmp(color, "LightYellow3") == 0) {
		r = 205;
		g = 205;
		b = 180;
	}
	if(strcmp(color, "LightYellow4") == 0) {
		r = 139;
		g = 139;
		b = 122;
	}
	if(strcmp(color, "yellow1") == 0) {
		r = 255;
		g = 255;
		b = 0;
	}
	if(strcmp(color, "yellow2") == 0) {
		r = 238;
		g = 238;
		b = 0;
	}
	if(strcmp(color, "yellow3") == 0) {
		r = 205;
		g = 205;
		b = 0;
	}
	if(strcmp(color, "yellow4") == 0) {
		r = 139;
		g = 139;
		b = 0;
	}
	if(strcmp(color, "gold1") == 0) {
		r = 255;
		g = 215;
		b = 0;
	}
	if(strcmp(color, "gold2") == 0) {
		r = 238;
		g = 201;
		b = 0;
	}
	if(strcmp(color, "gold3") == 0) {
		r = 205;
		g = 173;
		b = 0;
	}
	if(strcmp(color, "gold4") == 0) {
		r = 139;
		g = 117;
		b = 0;
	}
	if(strcmp(color, "goldenrod1") == 0) {
		r = 255;
		g = 193;
		b = 37;
	}
	if(strcmp(color, "goldenrod2") == 0) {
		r = 238;
		g = 180;
		b = 34;
	}
	if(strcmp(color, "goldenrod3") == 0) {
		r = 205;
		g = 155;
		b = 29;
	}
	if(strcmp(color, "goldenrod4") == 0) {
		r = 139;
		g = 105;
		b = 20;
	}
	if(strcmp(color, "DarkGoldenrod1") == 0) {
		r = 255;
		g = 185;
		b = 15;
	}
	if(strcmp(color, "DarkGoldenrod2") == 0) {
		r = 238;
		g = 173;
		b = 14;
	}
	if(strcmp(color, "DarkGoldenrod3") == 0) {
		r = 205;
		g = 149;
		b = 12;
	}
	if(strcmp(color, "DarkGoldenrod4") == 0) {
		r = 139;
		g = 101;
		b = 8;
	}
	if(strcmp(color, "RosyBrown1") == 0) {
		r = 255;
		g = 193;
		b = 193;
	}
	if(strcmp(color, "RosyBrown2") == 0) {
		r = 238;
		g = 180;
		b = 180;
	}
	if(strcmp(color, "RosyBrown3") == 0) {
		r = 205;
		g = 155;
		b = 155;
	}
	if(strcmp(color, "RosyBrown4") == 0) {
		r = 139;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "IndianRed1") == 0) {
		r = 255;
		g = 106;
		b = 106;
	}
	if(strcmp(color, "IndianRed2") == 0) {
		r = 238;
		g = 99;
		b = 99;
	}
	if(strcmp(color, "IndianRed3") == 0) {
		r = 205;
		g = 85;
		b = 85;
	}
	if(strcmp(color, "IndianRed4") == 0) {
		r = 139;
		g = 58;
		b = 58;
	}
	if(strcmp(color, "sienna1") == 0) {
		r = 255;
		g = 130;
		b = 71;
	}
	if(strcmp(color, "sienna2") == 0) {
		r = 238;
		g = 121;
		b = 66;
	}
	if(strcmp(color, "sienna3") == 0) {
		r = 205;
		g = 104;
		b = 57;
	}
	if(strcmp(color, "sienna4") == 0) {
		r = 139;
		g = 71;
		b = 38;
	}
	if(strcmp(color, "burlywood1") == 0) {
		r = 255;
		g = 211;
		b = 155;
	}
	if(strcmp(color, "burlywood2") == 0) {
		r = 238;
		g = 197;
		b = 145;
	}
	if(strcmp(color, "burlywood3") == 0) {
		r = 205;
		g = 170;
		b = 125;
	}
	if(strcmp(color, "burlywood4") == 0) {
		r = 139;
		g = 115;
		b = 85;
	}
	if(strcmp(color, "wheat1") == 0) {
		r = 255;
		g = 231;
		b = 186;
	}
	if(strcmp(color, "wheat2") == 0) {
		r = 238;
		g = 216;
		b = 174;
	}
	if(strcmp(color, "wheat3") == 0) {
		r = 205;
		g = 186;
		b = 150;
	}
	if(strcmp(color, "wheat4") == 0) {
		r = 139;
		g = 126;
		b = 102;
	}
	if(strcmp(color, "tan1") == 0) {
		r = 255;
		g = 165;
		b = 79;
	}
	if(strcmp(color, "tan2") == 0) {
		r = 238;
		g = 154;
		b = 73;
	}
	if(strcmp(color, "tan3") == 0) {
		r = 205;
		g = 133;
		b = 63;
	}
	if(strcmp(color, "tan4") == 0) {
		r = 139;
		g = 90;
		b = 43;
	}
	if(strcmp(color, "chocolate1") == 0) {
		r = 255;
		g = 127;
		b = 36;
	}
	if(strcmp(color, "chocolate2") == 0) {
		r = 238;
		g = 118;
		b = 33;
	}
	if(strcmp(color, "chocolate3") == 0) {
		r = 205;
		g = 102;
		b = 29;
	}
	if(strcmp(color, "chocolate4") == 0) {
		r = 139;
		g = 69;
		b = 19;
	}
	if(strcmp(color, "firebrick1") == 0) {
		r = 255;
		g = 48;
		b = 48;
	}
	if(strcmp(color, "firebrick2") == 0) {
		r = 238;
		g = 44;
		b = 44;
	}
	if(strcmp(color, "firebrick3") == 0) {
		r = 205;
		g = 38;
		b = 38;
	}
	if(strcmp(color, "firebrick4") == 0) {
		r = 139;
		g = 26;
		b = 26;
	}
	if(strcmp(color, "brown1") == 0) {
		r = 255;
		g = 64;
		b = 64;
	}
	if(strcmp(color, "brown2") == 0) {
		r = 238;
		g = 59;
		b = 59;
	}
	if(strcmp(color, "brown3") == 0) {
		r = 205;
		g = 51;
		b = 51;
	}
	if(strcmp(color, "brown4") == 0) {
		r = 139;
		g = 35;
		b = 35;
	}
	if(strcmp(color, "salmon1") == 0) {
		r = 255;
		g = 140;
		b = 105;
	}
	if(strcmp(color, "salmon2") == 0) {
		r = 238;
		g = 130;
		b = 98;
	}
	if(strcmp(color, "salmon3") == 0) {
		r = 205;
		g = 112;
		b = 84;
	}
	if(strcmp(color, "salmon4") == 0) {
		r = 139;
		g = 76;
		b = 57;
	}
	if(strcmp(color, "LightSalmon1") == 0) {
		r = 255;
		g = 160;
		b = 122;
	}
	if(strcmp(color, "LightSalmon2") == 0) {
		r = 238;
		g = 149;
		b = 114;
	}
	if(strcmp(color, "LightSalmon3") == 0) {
		r = 205;
		g = 129;
		b = 98;
	}
	if(strcmp(color, "LightSalmon4") == 0) {
		r = 139;
		g = 87;
		b = 66;
	}
	if(strcmp(color, "orange1") == 0) {
		r = 255;
		g = 165;
		b = 0;
	}
	if(strcmp(color, "orange2") == 0) {
		r = 238;
		g = 154;
		b = 0;
	}
	if(strcmp(color, "orange3") == 0) {
		r = 205;
		g = 133;
		b = 0;
	}
	if(strcmp(color, "orange4") == 0) {
		r = 139;
		g = 90;
		b = 0;
	}
	if(strcmp(color, "DarkOrange1") == 0) {
		r = 255;
		g = 127;
		b = 0;
	}
	if(strcmp(color, "DarkOrange2") == 0) {
		r = 238;
		g = 118;
		b = 0;
	}
	if(strcmp(color, "DarkOrange3") == 0) {
		r = 205;
		g = 102;
		b = 0;
	}
	if(strcmp(color, "DarkOrange4") == 0) {
		r = 139;
		g = 69;
		b = 0;
	}
	if(strcmp(color, "coral1") == 0) {
		r = 255;
		g = 114;
		b = 86;
	}
	if(strcmp(color, "coral2") == 0) {
		r = 238;
		g = 106;
		b = 80;
	}
	if(strcmp(color, "coral3") == 0) {
		r = 205;
		g = 91;
		b = 69;
	}
	if(strcmp(color, "coral4") == 0) {
		r = 139;
		g = 62;
		b = 47;
	}
	if(strcmp(color, "tomato1") == 0) {
		r = 255;
		g = 99;
		b = 71;
	}
	if(strcmp(color, "tomato2") == 0) {
		r = 238;
		g = 92;
		b = 66;
	}
	if(strcmp(color, "tomato3") == 0) {
		r = 205;
		g = 79;
		b = 57;
	}
	if(strcmp(color, "tomato4") == 0) {
		r = 139;
		g = 54;
		b = 38;
	}
	if(strcmp(color, "OrangeRed1") == 0) {
		r = 255;
		g = 69;
		b = 0;
	}
	if(strcmp(color, "OrangeRed2") == 0) {
		r = 238;
		g = 64;
		b = 0;
	}
	if(strcmp(color, "OrangeRed3") == 0) {
		r = 205;
		g = 55;
		b = 0;
	}
	if(strcmp(color, "OrangeRed4") == 0) {
		r = 139;
		g = 37;
		b = 0;
	}
	if(strcmp(color, "red1") == 0) {
		r = 255;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "red2") == 0) {
		r = 238;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "red3") == 0) {
		r = 205;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "red4") == 0) {
		r = 139;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "DeepPink1") == 0) {
		r = 255;
		g = 20;
		b = 147;
	}
	if(strcmp(color, "DeepPink2") == 0) {
		r = 238;
		g = 18;
		b = 137;
	}
	if(strcmp(color, "DeepPink3") == 0) {
		r = 205;
		g = 16;
		b = 118;
	}
	if(strcmp(color, "DeepPink4") == 0) {
		r = 139;
		g = 10;
		b = 80;
	}
	if(strcmp(color, "HotPink1") == 0) {
		r = 255;
		g = 110;
		b = 180;
	}
	if(strcmp(color, "HotPink2") == 0) {
		r = 238;
		g = 106;
		b = 167;
	}
	if(strcmp(color, "HotPink3") == 0) {
		r = 205;
		g = 96;
		b = 144;
	}
	if(strcmp(color, "HotPink4") == 0) {
		r = 139;
		g = 58;
		b = 98;
	}
	if(strcmp(color, "pink1") == 0) {
		r = 255;
		g = 181;
		b = 197;
	}
	if(strcmp(color, "pink2") == 0) {
		r = 238;
		g = 169;
		b = 184;
	}
	if(strcmp(color, "pink3") == 0) {
		r = 205;
		g = 145;
		b = 158;
	}
	if(strcmp(color, "pink4") == 0) {
		r = 139;
		g = 99;
		b = 108;
	}
	if(strcmp(color, "LightPink1") == 0) {
		r = 255;
		g = 174;
		b = 185;
	}
	if(strcmp(color, "LightPink2") == 0) {
		r = 238;
		g = 162;
		b = 173;
	}
	if(strcmp(color, "LightPink3") == 0) {
		r = 205;
		g = 140;
		b = 149;
	}
	if(strcmp(color, "LightPink4") == 0) {
		r = 139;
		g = 95;
		b = 101;
	}
	if(strcmp(color, "PaleVioletRed1") == 0) {
		r = 255;
		g = 130;
		b = 171;
	}
	if(strcmp(color, "PaleVioletRed2") == 0) {
		r = 238;
		g = 121;
		b = 159;
	}
	if(strcmp(color, "PaleVioletRed3") == 0) {
		r = 205;
		g = 104;
		b = 137;
	}
	if(strcmp(color, "PaleVioletRed4") == 0) {
		r = 139;
		g = 71;
		b = 93;
	}
	if(strcmp(color, "maroon1") == 0) {
		r = 255;
		g = 52;
		b = 179;
	}
	if(strcmp(color, "maroon2") == 0) {
		r = 238;
		g = 48;
		b = 167;
	}
	if(strcmp(color, "maroon3") == 0) {
		r = 205;
		g = 41;
		b = 144;
	}
	if(strcmp(color, "maroon4") == 0) {
		r = 139;
		g = 28;
		b = 98;
	}
	if(strcmp(color, "VioletRed1") == 0) {
		r = 255;
		g = 62;
		b = 150;
	}
	if(strcmp(color, "VioletRed2") == 0) {
		r = 238;
		g = 58;
		b = 140;
	}
	if(strcmp(color, "VioletRed3") == 0) {
		r = 205;
		g = 50;
		b = 120;
	}
	if(strcmp(color, "VioletRed4") == 0) {
		r = 139;
		g = 34;
		b = 82;
	}
	if(strcmp(color, "magenta1") == 0) {
		r = 255;
		g = 0;
		b = 255;
	}
	if(strcmp(color, "magenta2") == 0) {
		r = 238;
		g = 0;
		b = 238;
	}
	if(strcmp(color, "magenta3") == 0) {
		r = 205;
		g = 0;
		b = 205;
	}
	if(strcmp(color, "magenta4") == 0) {
		r = 139;
		g = 0;
		b = 139;
	}
	if(strcmp(color, "orchid1") == 0) {
		r = 255;
		g = 131;
		b = 250;
	}
	if(strcmp(color, "orchid2") == 0) {
		r = 238;
		g = 122;
		b = 233;
	}
	if(strcmp(color, "orchid3") == 0) {
		r = 205;
		g = 105;
		b = 201;
	}
	if(strcmp(color, "orchid4") == 0) {
		r = 139;
		g = 71;
		b = 137;
	}
	if(strcmp(color, "plum1") == 0) {
		r = 255;
		g = 187;
		b = 255;
	}
	if(strcmp(color, "plum2") == 0) {
		r = 238;
		g = 174;
		b = 238;
	}
	if(strcmp(color, "plum3") == 0) {
		r = 205;
		g = 150;
		b = 205;
	}
	if(strcmp(color, "plum4") == 0) {
		r = 139;
		g = 102;
		b = 139;
	}
	if(strcmp(color, "MediumOrchid1") == 0) {
		r = 224;
		g = 102;
		b = 255;
	}
	if(strcmp(color, "MediumOrchid2") == 0) {
		r = 209;
		g = 95;
		b = 238;
	}
	if(strcmp(color, "MediumOrchid3") == 0) {
		r = 180;
		g = 82;
		b = 205;
	}
	if(strcmp(color, "MediumOrchid4") == 0) {
		r = 122;
		g = 55;
		b = 139;
	}
	if(strcmp(color, "DarkOrchid1") == 0) {
		r = 191;
		g = 62;
		b = 255;
	}
	if(strcmp(color, "DarkOrchid2") == 0) {
		r = 178;
		g = 58;
		b = 238;
	}
	if(strcmp(color, "DarkOrchid3") == 0) {
		r = 154;
		g = 50;
		b = 205;
	}
	if(strcmp(color, "DarkOrchid4") == 0) {
		r = 104;
		g = 34;
		b = 139;
	}
	if(strcmp(color, "purple1") == 0) {
		r = 155;
		g = 48;
		b = 255;
	}
	if(strcmp(color, "purple2") == 0) {
		r = 145;
		g = 44;
		b = 238;
	}
	if(strcmp(color, "purple3") == 0) {
		r = 125;
		g = 38;
		b = 205;
	}
	if(strcmp(color, "purple4") == 0) {
		r = 85;
		g = 26;
		b = 139;
	}
	if(strcmp(color, "MediumPurple1") == 0) {
		r = 171;
		g = 130;
		b = 255;
	}
	if(strcmp(color, "MediumPurple2") == 0) {
		r = 159;
		g = 121;
		b = 238;
	}
	if(strcmp(color, "MediumPurple3") == 0) {
		r = 137;
		g = 104;
		b = 205;
	}
	if(strcmp(color, "MediumPurple4") == 0) {
		r = 93;
		g = 71;
		b = 139;
	}
	if(strcmp(color, "thistle1") == 0) {
		r = 255;
		g = 225;
		b = 255;
	}
	if(strcmp(color, "thistle2") == 0) {
		r = 238;
		g = 210;
		b = 238;
	}
	if(strcmp(color, "thistle3") == 0) {
		r = 205;
		g = 181;
		b = 205;
	}
	if(strcmp(color, "thistle4") == 0) {
		r = 139;
		g = 123;
		b = 139;
	}
	if(strcmp(color, "gray0") == 0) {
		r = 0;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "grey0") == 0) {
		r = 0;
		g = 0;
		b = 0;
	}
	if(strcmp(color, "gray1") == 0) {
		r = 3;
		g = 3;
		b = 3;
	}
	if(strcmp(color, "grey1") == 0) {
		r = 3;
		g = 3;
		b = 3;
	}
	if(strcmp(color, "gray2") == 0) {
		r = 5;
		g = 5;
		b = 5;
	}
	if(strcmp(color, "grey2") == 0) {
		r = 5;
		g = 5;
		b = 5;
	}
	if(strcmp(color, "gray3") == 0) {
		r = 8;
		g = 8;
		b = 8;
	}
	if(strcmp(color, "grey3") == 0) {
		r = 8;
		g = 8;
		b = 8;
	}
	if(strcmp(color, "gray4") == 0) {
		r = 10;
		g = 10;
		b = 10;
	}
	if(strcmp(color, "grey4") == 0) {
		r = 10;
		g = 10;
		b = 10;
	}
	if(strcmp(color, "gray5") == 0) {
		r = 13;
		g = 13;
		b = 13;
	}
	if(strcmp(color, "grey5") == 0) {
		r = 13;
		g = 13;
		b = 13;
	}
	if(strcmp(color, "gray6") == 0) {
		r = 15;
		g = 15;
		b = 15;
	}
	if(strcmp(color, "grey6") == 0) {
		r = 15;
		g = 15;
		b = 15;
	}
	if(strcmp(color, "gray7") == 0) {
		r = 18;
		g = 18;
		b = 18;
	}
	if(strcmp(color, "grey7") == 0) {
		r = 18;
		g = 18;
		b = 18;
	}
	if(strcmp(color, "gray8") == 0) {
		r = 20;
		g = 20;
		b = 20;
	}
	if(strcmp(color, "grey8") == 0) {
		r = 20;
		g = 20;
		b = 20;
	}
	if(strcmp(color, "gray9") == 0) {
		r = 23;
		g = 23;
		b = 23;
	}
	if(strcmp(color, "grey9") == 0) {
		r = 23;
		g = 23;
		b = 23;
	}
	if(strcmp(color, "gray10") == 0) {
		r = 26;
		g = 26;
		b = 26;
	}
	if(strcmp(color, "grey10") == 0) {
		r = 26;
		g = 26;
		b = 26;
	}
	if(strcmp(color, "gray11") == 0) {
		r = 28;
		g = 28;
		b = 28;
	}
	if(strcmp(color, "grey11") == 0) {
		r = 28;
		g = 28;
		b = 28;
	}
	if(strcmp(color, "gray12") == 0) {
		r = 31;
		g = 31;
		b = 31;
	}
	if(strcmp(color, "grey12") == 0) {
		r = 31;
		g = 31;
		b = 31;
	}
	if(strcmp(color, "gray13") == 0) {
		r = 33;
		g = 33;
		b = 33;
	}
	if(strcmp(color, "grey13") == 0) {
		r = 33;
		g = 33;
		b = 33;
	}
	if(strcmp(color, "gray14") == 0) {
		r = 36;
		g = 36;
		b = 36;
	}
	if(strcmp(color, "grey14") == 0) {
		r = 36;
		g = 36;
		b = 36;
	}
	if(strcmp(color, "gray15") == 0) {
		r = 38;
		g = 38;
		b = 38;
	}
	if(strcmp(color, "grey15") == 0) {
		r = 38;
		g = 38;
		b = 38;
	}
	if(strcmp(color, "gray16") == 0) {
		r = 41;
		g = 41;
		b = 41;
	}
	if(strcmp(color, "grey16") == 0) {
		r = 41;
		g = 41;
		b = 41;
	}
	if(strcmp(color, "gray17") == 0) {
		r = 43;
		g = 43;
		b = 43;
	}
	if(strcmp(color, "grey17") == 0) {
		r = 43;
		g = 43;
		b = 43;
	}
	if(strcmp(color, "gray18") == 0) {
		r = 46;
		g = 46;
		b = 46;
	}
	if(strcmp(color, "grey18") == 0) {
		r = 46;
		g = 46;
		b = 46;
	}
	if(strcmp(color, "gray19") == 0) {
		r = 48;
		g = 48;
		b = 48;
	}
	if(strcmp(color, "grey19") == 0) {
		r = 48;
		g = 48;
		b = 48;
	}
	if(strcmp(color, "gray20") == 0) {
		r = 51;
		g = 51;
		b = 51;
	}
	if(strcmp(color, "grey20") == 0) {
		r = 51;
		g = 51;
		b = 51;
	}
	if(strcmp(color, "gray21") == 0) {
		r = 54;
		g = 54;
		b = 54;
	}
	if(strcmp(color, "grey21") == 0) {
		r = 54;
		g = 54;
		b = 54;
	}
	if(strcmp(color, "gray22") == 0) {
		r = 56;
		g = 56;
		b = 56;
	}
	if(strcmp(color, "grey22") == 0) {
		r = 56;
		g = 56;
		b = 56;
	}
	if(strcmp(color, "gray23") == 0) {
		r = 59;
		g = 59;
		b = 59;
	}
	if(strcmp(color, "grey23") == 0) {
		r = 59;
		g = 59;
		b = 59;
	}
	if(strcmp(color, "gray24") == 0) {
		r = 61;
		g = 61;
		b = 61;
	}
	if(strcmp(color, "grey24") == 0) {
		r = 61;
		g = 61;
		b = 61;
	}
	if(strcmp(color, "gray25") == 0) {
		r = 64;
		g = 64;
		b = 64;
	}
	if(strcmp(color, "grey25") == 0) {
		r = 64;
		g = 64;
		b = 64;
	}
	if(strcmp(color, "gray26") == 0) {
		r = 66;
		g = 66;
		b = 66;
	}
	if(strcmp(color, "grey26") == 0) {
		r = 66;
		g = 66;
		b = 66;
	}
	if(strcmp(color, "gray27") == 0) {
		r = 69;
		g = 69;
		b = 69;
	}
	if(strcmp(color, "grey27") == 0) {
		r = 69;
		g = 69;
		b = 69;
	}
	if(strcmp(color, "gray28") == 0) {
		r = 71;
		g = 71;
		b = 71;
	}
	if(strcmp(color, "grey28") == 0) {
		r = 71;
		g = 71;
		b = 71;
	}
	if(strcmp(color, "gray29") == 0) {
		r = 74;
		g = 74;
		b = 74;
	}
	if(strcmp(color, "grey29") == 0) {
		r = 74;
		g = 74;
		b = 74;
	}
	if(strcmp(color, "gray30") == 0) {
		r = 77;
		g = 77;
		b = 77;
	}
	if(strcmp(color, "grey30") == 0) {
		r = 77;
		g = 77;
		b = 77;
	}
	if(strcmp(color, "gray31") == 0) {
		r = 79;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "grey31") == 0) {
		r = 79;
		g = 79;
		b = 79;
	}
	if(strcmp(color, "gray32") == 0) {
		r = 82;
		g = 82;
		b = 82;
	}
	if(strcmp(color, "grey32") == 0) {
		r = 82;
		g = 82;
		b = 82;
	}
	if(strcmp(color, "gray33") == 0) {
		r = 84;
		g = 84;
		b = 84;
	}
	if(strcmp(color, "grey33") == 0) {
		r = 84;
		g = 84;
		b = 84;
	}
	if(strcmp(color, "gray34") == 0) {
		r = 87;
		g = 87;
		b = 87;
	}
	if(strcmp(color, "grey34") == 0) {
		r = 87;
		g = 87;
		b = 87;
	}
	if(strcmp(color, "gray35") == 0) {
		r = 89;
		g = 89;
		b = 89;
	}
	if(strcmp(color, "grey35") == 0) {
		r = 89;
		g = 89;
		b = 89;
	}
	if(strcmp(color, "gray36") == 0) {
		r = 92;
		g = 92;
		b = 92;
	}
	if(strcmp(color, "grey36") == 0) {
		r = 92;
		g = 92;
		b = 92;
	}
	if(strcmp(color, "gray37") == 0) {
		r = 94;
		g = 94;
		b = 94;
	}
	if(strcmp(color, "grey37") == 0) {
		r = 94;
		g = 94;
		b = 94;
	}
	if(strcmp(color, "gray38") == 0) {
		r = 97;
		g = 97;
		b = 97;
	}
	if(strcmp(color, "grey38") == 0) {
		r = 97;
		g = 97;
		b = 97;
	}
	if(strcmp(color, "gray39") == 0) {
		r = 99;
		g = 99;
		b = 99;
	}
	if(strcmp(color, "grey39") == 0) {
		r = 99;
		g = 99;
		b = 99;
	}
	if(strcmp(color, "gray40") == 0) {
		r = 102;
		g = 102;
		b = 102;
	}
	if(strcmp(color, "grey40") == 0) {
		r = 102;
		g = 102;
		b = 102;
	}
	if(strcmp(color, "gray41") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "grey41") == 0) {
		r = 105;
		g = 105;
		b = 105;
	}
	if(strcmp(color, "gray42") == 0) {
		r = 107;
		g = 107;
		b = 107;
	}
	if(strcmp(color, "grey42") == 0) {
		r = 107;
		g = 107;
		b = 107;
	}
	if(strcmp(color, "gray43") == 0) {
		r = 110;
		g = 110;
		b = 110;
	}
	if(strcmp(color, "grey43") == 0) {
		r = 110;
		g = 110;
		b = 110;
	}
	if(strcmp(color, "gray44") == 0) {
		r = 112;
		g = 112;
		b = 112;
	}
	if(strcmp(color, "grey44") == 0) {
		r = 112;
		g = 112;
		b = 112;
	}
	if(strcmp(color, "gray45") == 0) {
		r = 115;
		g = 115;
		b = 115;
	}
	if(strcmp(color, "grey45") == 0) {
		r = 115;
		g = 115;
		b = 115;
	}
	if(strcmp(color, "gray46") == 0) {
		r = 117;
		g = 117;
		b = 117;
	}
	if(strcmp(color, "grey46") == 0) {
		r = 117;
		g = 117;
		b = 117;
	}
	if(strcmp(color, "gray47") == 0) {
		r = 120;
		g = 120;
		b = 120;
	}
	if(strcmp(color, "grey47") == 0) {
		r = 120;
		g = 120;
		b = 120;
	}
	if(strcmp(color, "gray48") == 0) {
		r = 122;
		g = 122;
		b = 122;
	}
	if(strcmp(color, "grey48") == 0) {
		r = 122;
		g = 122;
		b = 122;
	}
	if(strcmp(color, "gray49") == 0) {
		r = 125;
		g = 125;
		b = 125;
	}
	if(strcmp(color, "grey49") == 0) {
		r = 125;
		g = 125;
		b = 125;
	}
	if(strcmp(color, "gray50") == 0) {
		r = 127;
		g = 127;
		b = 127;
	}
	if(strcmp(color, "grey50") == 0) {
		r = 127;
		g = 127;
		b = 127;
	}
	if(strcmp(color, "gray51") == 0) {
		r = 130;
		g = 130;
		b = 130;
	}
	if(strcmp(color, "grey51") == 0) {
		r = 130;
		g = 130;
		b = 130;
	}
	if(strcmp(color, "gray52") == 0) {
		r = 133;
		g = 133;
		b = 133;
	}
	if(strcmp(color, "grey52") == 0) {
		r = 133;
		g = 133;
		b = 133;
	}
	if(strcmp(color, "gray53") == 0) {
		r = 135;
		g = 135;
		b = 135;
	}
	if(strcmp(color, "grey53") == 0) {
		r = 135;
		g = 135;
		b = 135;
	}
	if(strcmp(color, "gray54") == 0) {
		r = 138;
		g = 138;
		b = 138;
	}
	if(strcmp(color, "grey54") == 0) {
		r = 138;
		g = 138;
		b = 138;
	}
	if(strcmp(color, "gray55") == 0) {
		r = 140;
		g = 140;
		b = 140;
	}
	if(strcmp(color, "grey55") == 0) {
		r = 140;
		g = 140;
		b = 140;
	}
	if(strcmp(color, "gray56") == 0) {
		r = 143;
		g = 143;
		b = 143;
	}
	if(strcmp(color, "grey56") == 0) {
		r = 143;
		g = 143;
		b = 143;
	}
	if(strcmp(color, "gray57") == 0) {
		r = 145;
		g = 145;
		b = 145;
	}
	if(strcmp(color, "grey57") == 0) {
		r = 145;
		g = 145;
		b = 145;
	}
	if(strcmp(color, "gray58") == 0) {
		r = 148;
		g = 148;
		b = 148;
	}
	if(strcmp(color, "grey58") == 0) {
		r = 148;
		g = 148;
		b = 148;
	}
	if(strcmp(color, "gray59") == 0) {
		r = 150;
		g = 150;
		b = 150;
	}
	if(strcmp(color, "grey59") == 0) {
		r = 150;
		g = 150;
		b = 150;
	}
	if(strcmp(color, "gray60") == 0) {
		r = 153;
		g = 153;
		b = 153;
	}
	if(strcmp(color, "grey60") == 0) {
		r = 153;
		g = 153;
		b = 153;
	}
	if(strcmp(color, "gray61") == 0) {
		r = 156;
		g = 156;
		b = 156;
	}
	if(strcmp(color, "grey61") == 0) {
		r = 156;
		g = 156;
		b = 156;
	}
	if(strcmp(color, "gray62") == 0) {
		r = 158;
		g = 158;
		b = 158;
	}
	if(strcmp(color, "grey62") == 0) {
		r = 158;
		g = 158;
		b = 158;
	}
	if(strcmp(color, "gray63") == 0) {
		r = 161;
		g = 161;
		b = 161;
	}
	if(strcmp(color, "grey63") == 0) {
		r = 161;
		g = 161;
		b = 161;
	}
	if(strcmp(color, "gray64") == 0) {
		r = 163;
		g = 163;
		b = 163;
	}
	if(strcmp(color, "grey64") == 0) {
		r = 163;
		g = 163;
		b = 163;
	}
	if(strcmp(color, "gray65") == 0) {
		r = 166;
		g = 166;
		b = 166;
	}
	if(strcmp(color, "grey65") == 0) {
		r = 166;
		g = 166;
		b = 166;
	}
	if(strcmp(color, "gray66") == 0) {
		r = 168;
		g = 168;
		b = 168;
	}
	if(strcmp(color, "grey66") == 0) {
		r = 168;
		g = 168;
		b = 168;
	}
	if(strcmp(color, "gray67") == 0) {
		r = 171;
		g = 171;
		b = 171;
	}
	if(strcmp(color, "grey67") == 0) {
		r = 171;
		g = 171;
		b = 171;
	}
	if(strcmp(color, "gray68") == 0) {
		r = 173;
		g = 173;
		b = 173;
	}
	if(strcmp(color, "grey68") == 0) {
		r = 173;
		g = 173;
		b = 173;
	}
	if(strcmp(color, "gray69") == 0) {
		r = 176;
		g = 176;
		b = 176;
	}
	if(strcmp(color, "grey69") == 0) {
		r = 176;
		g = 176;
		b = 176;
	}
	if(strcmp(color, "gray70") == 0) {
		r = 179;
		g = 179;
		b = 179;
	}
	if(strcmp(color, "grey70") == 0) {
		r = 179;
		g = 179;
		b = 179;
	}
	if(strcmp(color, "gray71") == 0) {
		r = 181;
		g = 181;
		b = 181;
	}
	if(strcmp(color, "grey71") == 0) {
		r = 181;
		g = 181;
		b = 181;
	}
	if(strcmp(color, "gray72") == 0) {
		r = 184;
		g = 184;
		b = 184;
	}
	if(strcmp(color, "grey72") == 0) {
		r = 184;
		g = 184;
		b = 184;
	}
	if(strcmp(color, "gray73") == 0) {
		r = 186;
		g = 186;
		b = 186;
	}
	if(strcmp(color, "grey73") == 0) {
		r = 186;
		g = 186;
		b = 186;
	}
	if(strcmp(color, "gray74") == 0) {
		r = 189;
		g = 189;
		b = 189;
	}
	if(strcmp(color, "grey74") == 0) {
		r = 189;
		g = 189;
		b = 189;
	}
	if(strcmp(color, "gray75") == 0) {
		r = 191;
		g = 191;
		b = 191;
	}
	if(strcmp(color, "grey75") == 0) {
		r = 191;
		g = 191;
		b = 191;
	}
	if(strcmp(color, "gray76") == 0) {
		r = 194;
		g = 194;
		b = 194;
	}
	if(strcmp(color, "grey76") == 0) {
		r = 194;
		g = 194;
		b = 194;
	}
	if(strcmp(color, "gray77") == 0) {
		r = 196;
		g = 196;
		b = 196;
	}
	if(strcmp(color, "grey77") == 0) {
		r = 196;
		g = 196;
		b = 196;
	}
	if(strcmp(color, "gray78") == 0) {
		r = 199;
		g = 199;
		b = 199;
	}
	if(strcmp(color, "grey78") == 0) {
		r = 199;
		g = 199;
		b = 199;
	}
	if(strcmp(color, "gray79") == 0) {
		r = 201;
		g = 201;
		b = 201;
	}
	if(strcmp(color, "grey79") == 0) {
		r = 201;
		g = 201;
		b = 201;
	}
	if(strcmp(color, "gray80") == 0) {
		r = 204;
		g = 204;
		b = 204;
	}
	if(strcmp(color, "grey80") == 0) {
		r = 204;
		g = 204;
		b = 204;
	}
	if(strcmp(color, "gray81") == 0) {
		r = 207;
		g = 207;
		b = 207;
	}
	if(strcmp(color, "grey81") == 0) {
		r = 207;
		g = 207;
		b = 207;
	}
	if(strcmp(color, "gray82") == 0) {
		r = 209;
		g = 209;
		b = 209;
	}
	if(strcmp(color, "grey82") == 0) {
		r = 209;
		g = 209;
		b = 209;
	}
	if(strcmp(color, "gray83") == 0) {
		r = 212;
		g = 212;
		b = 212;
	}
	if(strcmp(color, "grey83") == 0) {
		r = 212;
		g = 212;
		b = 212;
	}
	if(strcmp(color, "gray84") == 0) {
		r = 214;
		g = 214;
		b = 214;
	}
	if(strcmp(color, "grey84") == 0) {
		r = 214;
		g = 214;
		b = 214;
	}
	if(strcmp(color, "gray85") == 0) {
		r = 217;
		g = 217;
		b = 217;
	}
	if(strcmp(color, "grey85") == 0) {
		r = 217;
		g = 217;
		b = 217;
	}
	if(strcmp(color, "gray86") == 0) {
		r = 219;
		g = 219;
		b = 219;
	}
	if(strcmp(color, "grey86") == 0) {
		r = 219;
		g = 219;
		b = 219;
	}
	if(strcmp(color, "gray87") == 0) {
		r = 222;
		g = 222;
		b = 222;
	}
	if(strcmp(color, "grey87") == 0) {
		r = 222;
		g = 222;
		b = 222;
	}
	if(strcmp(color, "gray88") == 0) {
		r = 224;
		g = 224;
		b = 224;
	}
	if(strcmp(color, "grey88") == 0) {
		r = 224;
		g = 224;
		b = 224;
	}
	if(strcmp(color, "gray89") == 0) {
		r = 227;
		g = 227;
		b = 227;
	}
	if(strcmp(color, "grey89") == 0) {
		r = 227;
		g = 227;
		b = 227;
	}
	if(strcmp(color, "gray90") == 0) {
		r = 229;
		g = 229;
		b = 229;
	}
	if(strcmp(color, "grey90") == 0) {
		r = 229;
		g = 229;
		b = 229;
	}
	if(strcmp(color, "gray91") == 0) {
		r = 232;
		g = 232;
		b = 232;
	}
	if(strcmp(color, "grey91") == 0) {
		r = 232;
		g = 232;
		b = 232;
	}
	if(strcmp(color, "gray92") == 0) {
		r = 235;
		g = 235;
		b = 235;
	}
	if(strcmp(color, "grey92") == 0) {
		r = 235;
		g = 235;
		b = 235;
	}
	if(strcmp(color, "gray93") == 0) {
		r = 237;
		g = 237;
		b = 237;
	}
	if(strcmp(color, "grey93") == 0) {
		r = 237;
		g = 237;
		b = 237;
	}
	if(strcmp(color, "gray94") == 0) {
		r = 240;
		g = 240;
		b = 240;
	}
	if(strcmp(color, "grey94") == 0) {
		r = 240;
		g = 240;
		b = 240;
	}
	if(strcmp(color, "gray95") == 0) {
		r = 242;
		g = 242;
		b = 242;
	}
	if(strcmp(color, "grey95") == 0) {
		r = 242;
		g = 242;
		b = 242;
	}
	if(strcmp(color, "gray96") == 0) {
		r = 245;
		g = 245;
		b = 245;
	}
	if(strcmp(color, "grey96") == 0) {
		r = 245;
		g = 245;
		b = 245;
	}
	if(strcmp(color, "gray97") == 0) {
		r = 247;
		g = 247;
		b = 247;
	}
	if(strcmp(color, "grey97") == 0) {
		r = 247;
		g = 247;
		b = 247;
	}
	if(strcmp(color, "gray98") == 0) {
		r = 250;
		g = 250;
		b = 250;
	}
	if(strcmp(color, "grey98") == 0) {
		r = 250;
		g = 250;
		b = 250;
	}
	if(strcmp(color, "gray99") == 0) {
		r = 252;
		g = 252;
		b = 252;
	}
	if(strcmp(color, "grey99") == 0) {
		r = 252;
		g = 252;
		b = 252;
	}
	if(strcmp(color, "gray100") == 0) {
		r = 255;
		g = 255;
		b = 255;
	}
	if(strcmp(color, "grey100") == 0) {
		r = 255;
		g = 255;
		b = 255;
	}
	return MwLLAllocColor(handle->lowlevel, r, g, b);
}
