/* $Id$ */
#declare BASE = 1;
#include "resource/logo/logo.pov"

object {
	text {
		ttf "resource/font/IBMPlexSerif.ttf",
		"Milsko Toolkit",
		1, 0
	}
	scale 2
	translate <2.5, -1, -1>
	pigment {
		color White
	}
	finish {
		phong 1
		reflection 0.3
	}
}

camera {
	location <8+5, 2.5, -37.5>
	look_at <7, 0.75, 0>
	angle 35
	up <0, 1, 0>
	right image_width / image_height * x
}
