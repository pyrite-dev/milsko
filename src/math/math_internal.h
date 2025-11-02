/* $Id$ */

#ifndef __MW_LOWLEVEL_INTERNAL_MATH_H__
#define __MW_LOWLEVEL_INTERNAL_MATH_H__

#include <Mw/BaseTypes.h>
#include <Mw/LowLevelMath.h>

typedef struct _MwLLMathVTable MwLLMathVTable;

struct _MwLLMathVTable {
	void (*add)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*multiply)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*sub)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*reciprocal)(MwLLVec* a, MwLLVec* out);
	void (*squareRoot)(MwLLVec* a, MwLLVec* out);
	void (*and)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*or)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*shiftRight)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*shiftLeft)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*equal)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*greaterThen)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*lesserThen)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
};

struct _MwLLVec {
	void*	       buffer;
	int	       ty;
	MwU64	       size;
	MwLLMathVTable vtable;
};

void default_apply(MwLLVec*);

/* Bitfield of cpu features we get from x86's CPUID */
#if defined(MwLLMath_x86)
#define FEATX86_MMX (1 << 23)
// #define FEATX86_SSE (1 << 25)
#define FEATX86_SSE2 (1 << 26)
void mmx_apply(MwLLVec*);
#endif

#endif
