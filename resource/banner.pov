#declare BASE = 1;
#include "resource/logo.pov"

object {
	text {
		ttf "resource/mononoki.ttf",
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
	location <8+6, 2.5, -40>
	look_at <8, 0.75, 0>
	angle 35
	up <0, 1, 0>
	right <5, 0, 0>
}
