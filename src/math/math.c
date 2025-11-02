/* $Id$ */
#include <Mw/LowLevelMath.h>

#include "math_internal.h"

#if defined(MwLLMath_x86)
struct x86Features {
	MwBool mmx;
	MwBool sse2;
};

static struct x86Features
getCPUFeatures(void) {
	MwU32		   _eax = 1;
	MwU32		   _edx;
	struct x86Features features;

#ifdef __WATCOMC__
	__asm {
		mov eax, _eax
		cpuid
		mov _eax, eax
		mov _edx, edx
	}
#else
	asm volatile(
	    "cpuid" : "=a"(_eax), "=d"(_edx)
	    : "a"(1) : "ebx", "ecx");
#endif

	if(_edx & FEATX86_MMX) {
		features.mmx = MwTRUE;
	}
	if(_edx & FEATX86_SSE2) {
		features.sse2 = MwTRUE;
	}
	return features;
}
#endif

static int round_multiple(int num, int mul) {
	return ((num + mul - 1) / mul) * mul;
}

MwLLVec* MwLLVaVecCreate(int ty, MwU64 size, ...) {
	struct x86Features features = getCPUFeatures();
	MwLLVec*	   vec	    = malloc(sizeof(MwLLVec));
	va_list		   va;
	int		   i		= 0;
	int		   size_rounded = size;

	memset(vec, 0, sizeof(MwLLVec));

	vec->size = size;

#ifdef MwLLMath_x86
	if(features.mmx) {
		size_rounded = round_multiple(size, 64);
	}
#endif

	va_start(va, size);

	switch(ty) {
	case MwLLVecTypeU8:
	case MwLLVecTypeI8:
		vec->buffer = malloc(size_rounded * sizeof(MwU8));
		memset(vec->buffer, 0, size_rounded);
		for(; i < size; i++) {
			((MwU8*)vec->buffer)[i] = va_arg(va, int);
		}
		break;
	case MwLLVecTypeU16:
	case MwLLVecTypeI16:
		vec->buffer = malloc(size_rounded * sizeof(MwU16));
		memset(vec->buffer, 0, size_rounded);
		for(; i < size; i++) {
			((MwU16*)vec->buffer)[i] = va_arg(va, int);
		}
		break;
	case MwLLVecTypeU32:
	case MwLLVecTypeI32:
		vec->buffer = malloc(size_rounded * sizeof(MwU32));
		memset(vec->buffer, 0, size_rounded);
		for(; i < size; i++) {
			((MwU32*)vec->buffer)[i] = va_arg(va, int);
		}
		break;
	case MwLLVecTypeU64:
	case MwLLVecTypeI64:
		vec->buffer = malloc(size_rounded * sizeof(MwU64));
		memset(vec->buffer, 0, size_rounded);
		for(; i < size; i++) {
			((MwU64*)vec->buffer)[i] = va_arg(va, int);
		}
		break;
	default:
		printf("Unknown MwLLVecType %d\n", ty);
		break;
	}

	va_end(va);

	default_apply(vec);

#ifdef MwLLMath_x86
	if(features.mmx) {
		mmx_apply(vec);
	}
#endif

	return vec;
}

// TODO: Flag for telling debug builds so we can selectively take this out
#define dbg_assert(a) assert(a)

void MwLLMathAdd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.add(a, b, out);
}
void MwLLMathSub(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.sub(a, b, out);
}
void MwLLMathMultiply(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.multiply(a, b, out);
}
void MwLLMathReciprocal(MwLLVec* a, MwLLVec* out) {
	dbg_assert(a->size == out->size);
	a->vtable.reciprocal(a, out);
}
void MwLLMathSquareRoot(MwLLVec* a, MwLLVec* out) {
	dbg_assert(a->size == out->size);
	a->vtable.squareRoot(a, out);
}

void MwLLMathShiftRight(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.shiftRight(a, b, out);
}
void MwLLMathShiftLeft(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	a->vtable.shiftLeft(a, b, out);
}
void MwLLMathEqual(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
}
void MwLLMathGreaterThen(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.shiftRight(a, b, out);
}
void MwLLMathAnd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.and (a, b, out);
}
void MwLLMathOr(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	dbg_assert(a->size == b->size && a->size == out->size && b->size && out->size);
	a->vtable.or (a, b, out);
}

MWDECL MwU8 MwLLVecIndexU8(MwLLVec* vec, MwU64 index) {
	return ((MwU8*)vec->buffer)[index];
};
MWDECL MwU16 MwLLVecIndexU16(MwLLVec* vec, MwU64 index) {
	return ((MwU16*)vec->buffer)[index];
};
MWDECL MwU32 MwLLVecIndexU32(MwLLVec* vec, MwU64 index) {
	return ((MwU32*)vec->buffer)[index];
};
MWDECL MwU64 MwLLVecIndexU64(MwLLVec* vec, MwU64 index) {
	return ((MwU64*)vec->buffer)[index];
};
MWDECL MwI8 MwLLVecIndexI8(MwLLVec* vec, MwU64 index) {
	return ((MwI8*)vec->buffer)[index];
};
MWDECL MwI16 MwLLVecIndexI16(MwLLVec* vec, MwU64 index) {
	return ((MwI16*)vec->buffer)[index];
};
MWDECL MwI32 MwLLVecIndexI32(MwLLVec* vec, MwU64 index) {
	return ((MwI32*)vec->buffer)[index];
};
MWDECL MwI64 MwLLVecIndexI64(MwLLVec* vec, MwU64 index) {
	return ((MwI64*)vec->buffer)[index];
};
