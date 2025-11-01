
/* @(#)e_gamma.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* __fdlibm_gamma(x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __fdlibm_gamma_r
 */

#include "math.h"

extern int signgam;

double __fdlibm_gamma(double x) {
	return __fdlibm_gamma_r(x, &signgam);
}
