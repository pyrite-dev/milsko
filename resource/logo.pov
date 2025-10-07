// $Id$
#include "shapes.inc"
#include "colors.inc"

object {
	Cube
	translate <-1, 0, 1>
	pigment {
		color Red
	}
}

object {
	Cube
	translate <1, 0, 1>
	pigment {
		color Cyan
	}
}

object {
	Cube
	translate <-1, 0, -1>
	pigment {
		color Blue
	}
}

object {
	Cube
	translate <1, 0, -1>
	pigment {
		color Green
	}
}

object {
	Cube
	translate <0, 2, 0>
	pigment {
		color Red
	}
}

plane {
	y, -1
	pigment {
		checker Red Gray
	}
	finish {
		phong 1
		reflection 0.5
	}
}

light_source {
	<-250, 250, -500>
	color White*2
	parallel point_at <0, 0, 0>
}

#ifndef(BASE)
camera {
	location <1.5, 2.5, -10>
	look_at <0, 0.75, 0>
	angle 35
	up <0, 1, 0>
	right <1, 0, 0>
}
#end
