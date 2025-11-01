/* @(#)s_scalbn.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/* $Id$ */

#include <Mw/BaseTypes.h>
#include "math_internal.h"

/*
 * scalbn (double x, int n)
 * scalbn(x,n) returns x* 2**n  computed by  exponent
 * manipulation rather than by actually performing an
 * exponentiation or a multiplication.
 */

static const double
    two54  = 1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
    twom54 = 5.55111512312578270212e-17, /* 0x3C900000, 0x00000000 */
    hugev  = 1.0e+300,
    tinyv  = 1.0e-300;

double
nbsd_scalbn(double x, int n) {
	return nbsd_scalbln(x, n);
}

double
nbsd_scalbln(double x, long n) {
	MwI32 k, hx, lx;
	EXTRACT_WORDS(hx, lx, x);
	k = ((MwU32)hx & 0x7ff00000) >> 20;		    /* extract exponent */
	if(k == 0) {					    /* 0 or subnormal x */
		if((lx | (hx & 0x7fffffff)) == 0) return x; /* +-0 */
		x *= two54;
		GET_HIGH_WORD(hx, x);
		k = (((MwU32)hx & 0x7ff00000) >> 20) - 54;
		if(n < -50000) return tinyv * x; /*underflow*/
	}
	if(k == 0x7ff) return x + x; /* NaN or Inf */
	k = k + n;
	if(k > 0x7fe) return hugev * nbsd_copysign(hugev, x); /* overflow  */
	if(k > 0)					      /* normal result */
	{
		SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
		return x;
	}
	if(k <= -54) {
		if(n > 50000)					/* in case integer overflow in n+k */
			return hugev * nbsd_copysign(hugev, x); /*overflow*/
		else
			return tinyv * nbsd_copysign(tinyv, x); /*underflow*/
	}
	k += 54; /* subnormal result */
	SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
	return x * twom54;
}
