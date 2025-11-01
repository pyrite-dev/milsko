
/* @(#)w_j1.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * wrapper of j1,y1
 */

#include "math.h"

double j1(double x) /* wrapper j1 */
{
	return __fdlibm_j1(x);
}

double y1(double x) /* wrapper y1 */
{
	return __fdlibm_y1(x);
}
