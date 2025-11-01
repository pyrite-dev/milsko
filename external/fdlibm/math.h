
/* @(#)fdlibm.h 1.5 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef _MATH_H_
#define _MATH_H_

/* Sometimes it's necessary to define __LITTLE_ENDIAN explicitly
   but these catch some common cases. */

#if defined(__WATCOMC__) || defined(_MSC_VER) || defined(i386) || defined(i486) || defined(intel) || defined(x86) || defined(i86pc) || defined(__alpha) || defined(__osf__) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN
#endif

#ifdef __LITTLE_ENDIAN
#define __HI(x) *(1 + (int*)&x)
#define __LO(x) *(int*)&x
#define __HIp(x) *(1 + (int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x) *(int*)&x
#define __LO(x) *(1 + (int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1 + (int*)x)
#endif

#define signgam fdlibm_signgam

#define acos fdlibm_acos
#define asin fdlibm_asin
#define atan fdlibm_atan
#define atan2 fdlibm_atan2
#define cos fdlibm_cos
#define sin fdlibm_sin
#define tan fdlibm_tan

#define cosh fdlibm_cosh
#define sinh fdlibm_sinh
#define tanh fdlibm_tanh

#define exp fdlibm_exp
#define frexp fdlibm_frexp
#define ldexp fdlibm_ldexp
#define log fdlibm_log
#define log10 fdlibm_log10
#define modf fdlibm_modf

#define pow fdlibm_pow
#define sqrt fdlibm_sqrt

#define ceil fdlibm_ceil
#define fabs fdlibm_fabs
#define floor fdlibm_floor
#define fmod fdlibm_fmod

#define erf fdlibm_erf
#define erfc fdlibm_erfc
#define gamma fdlibm_gamma
#define hypot fdlibm_hypot
#define isnan fdlibm_isnan
#define finite fdlibm_finite
#define j0 fdlibm_j0
#define j1 fdlibm_j1
#define jn fdlibm_jn
#define lgamma fdlibm_lgamma
#define y0 fdlibm_y0
#define y1 fdlibm_y1
#define yn fdlibm_yn

#define acosh fdlibm_acosh
#define asinh fdlibm_asinh
#define atanh fdlibm_atanh
#define cbrt fdlibm_cbrt
#define logb fdlibm_logb
#define nextafter fdlibm_nextafter
#define remainder fdlibm_remainder
#define scalb fdlibm_scalb

#define significand fdlibm_significand

#define copysign fdlibm_copysign
#define ilogb fdlibm_ilogb
#define rint fdlibm_rint
#define scalbn fdlibm_scalbn

#define expm1 fdlibm_expm1
#define log1p fdlibm_log1p


/*
 * ANSI/POSIX
 */

extern int signgam;

#define MAXFLOAT ((float)3.40282346638528860e+38)

#define HUGE MAXFLOAT

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS 1.41484755040568800000e+16

#define DOMAIN 1
#define SING 2
#define OVERFLOW 3
#define UNDERFLOW 4
#define TLOSS 5
#define PLOSS 6

/*
 * ANSI/POSIX
 */
extern double acos(double);
extern double asin(double);
extern double atan(double);
extern double atan2(double, double);
extern double cos(double);
extern double sin(double);
extern double tan(double);

extern double cosh(double);
extern double sinh(double);
extern double tanh(double);

extern double exp(double);
extern double frexp(double, int*);
extern double ldexp(double, int);
extern double log(double);
extern double log10(double);
extern double modf(double, double*);

extern double pow(double, double);
extern double sqrt(double);

extern double ceil(double);
extern double fabs(double);
extern double floor(double);
extern double fmod(double, double);

extern double erf(double);
extern double erfc(double);
extern double gamma(double);
extern double hypot(double, double);
extern int    isnan(double);
extern int    finite(double);
extern double j0(double);
extern double j1(double);
extern double jn(int, double);
extern double lgamma(double);
extern double y0(double);
extern double y1(double);
extern double yn(int, double);

extern double acosh(double);
extern double asinh(double);
extern double atanh(double);
extern double cbrt(double);
extern double logb(double);
extern double nextafter(double, double);
extern double remainder(double, double);
extern double scalb(double, double);

/*
 * IEEE Test Vector
 */
extern double significand(double);

/*
 * Functions callable from C, intended to support IEEE arithmetic.
 */
extern double copysign(double, double);
extern int    ilogb(double);
extern double rint(double);
extern double scalbn(double, int);

/*
 * BSD math library entry points
 */
extern double expm1(double);
extern double log1p(double);

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#ifdef _REENTRANT
#define gamma_r fdlibm_gamma_r
#define lgamma_r fdlibm_lgamma_r

extern double gamma_r(double, int*);
extern double lgamma_r(double, int*);
#endif /* _REENTRANT */

/* ieee style elementary functions */
extern double __fdlibm_sqrt(double);
extern double __fdlibm_acos(double);
extern double __fdlibm_acosh(double);
extern double __fdlibm_log(double);
extern double __fdlibm_atanh(double);
extern double __fdlibm_asin(double);
extern double __fdlibm_atan2(double, double);
extern double __fdlibm_exp(double);
extern double __fdlibm_cosh(double);
extern double __fdlibm_fmod(double, double);
extern double __fdlibm_pow(double, double);
extern double __fdlibm_lgamma_r(double, int*);
extern double __fdlibm_gamma_r(double, int*);
extern double __fdlibm_lgamma(double);
extern double __fdlibm_gamma(double);
extern double __fdlibm_log10(double);
extern double __fdlibm_sinh(double);
extern double __fdlibm_hypot(double, double);
extern double __fdlibm_j0(double);
extern double __fdlibm_j1(double);
extern double __fdlibm_y0(double);
extern double __fdlibm_y1(double);
extern double __fdlibm_jn(int, double);
extern double __fdlibm_yn(int, double);
extern double __fdlibm_remainder(double, double);
extern int    __fdlibm_rem_pio2(double, double*);
#ifdef _SCALB_INT
extern double __fdlibm_scalb(double, int);
#else
extern double __fdlibm_scalb(double, double);
#endif

/* fdlibm kernel function */
extern double __fdlibm_kernel_sin(double, double, int);
extern double __fdlibm_kernel_cos(double, double);
extern double __fdlibm_kernel_tan(double, double, int);
extern int    __fdlibm_kernel_rem_pio2(double*, double*, int, int, int, const int*);

#endif
