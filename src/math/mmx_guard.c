#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
#include "mmx.c"
#else
void mmx_apply(MwLLMathVTable** t) {
}
#endif
