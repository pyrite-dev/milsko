/* $Id$ */
#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
#define GUARD
#include "mmx.c"
#else
void mmx_apply(MwLLMathVTable** t) {
}
#endif
