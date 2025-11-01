#include <Mw/LowLevelMath.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include "math.h"
#include "Mw/BaseTypes.h"
#include "x86intrin.h"

MwLLVec _MwLLVecCreateGeneric(MwLLVecType ty, ...) {
	MwLLVecUnion un;
	MwLLVec	     vec;
	va_list	     va;

	va_start(va, ty);

	// clang-format off
#define _A_B(ty) un.ty.a = va_arg(va, int); un.ty.b = va_arg(va, int);
#define _C_D(ty) un.ty.c = va_arg(va, int); un.ty.d = va_arg(va, int);
#define _E_F(ty) un.ty.e = va_arg(va, int); un.ty.f = va_arg(va, int);
#define _G_H(ty) un.ty.g = va_arg(va, int); un.ty.h = va_arg(va, int);
switch(ty) {
	case _MwLLVecTypeU8x8:  _A_B(u8);   _C_D(u8);   _E_F(u8);   _G_H(u8);   break;
	case _MwLLVecTypeU16x4: _A_B(u16);  _C_D(u16);                          break;
	case _MwLLVecTypeU32x2: _A_B(u32);                                      break;
	case _MwLLVecTypeI8x8:  _A_B(i8);   _C_D(i8);   _E_F(i8);   _G_H(i8);   break;
	case _MwLLVecTypeI16x4: _A_B(i16);  _C_D(i16);                          break;
	case _MwLLVecTypeI32x2: _A_B(i32);                                      break;
	case _MwLLVecType_Max: break;
}
#undef _A_B
#undef _C_D
#undef _E_F
#undef _G_H
	// clang-format on

	va_end(va);

	vec.ty = ty;
	vec.un = un;

	return vec;
}

static MwBool hasMMX(void) {
	MwU32 eax = 1;
	MwU32 ebx, edx;

	__asm__ __volatile__(
	    "cpuid" : "=a"(eax), "=b"(ebx), "=d"(edx)
	    : "a"(1));

	return (edx & (1 << 23)) == (1 << 23);
}

static MwLLMathVTable** mwLLMultiTable;
static MwLLMathVTable*	multiTableSetupAndGet(MwLLVecType ty);
static MwLLMathVTable*	multiTableGet(MwLLVecType ty);

static MwLLMathVTable* (*mwLLmathFunc)(MwLLVecType ty) = multiTableSetupAndGet;

static MwLLMathVTable* getMultiTable(MwLLVecType ty) {
	return mwLLmathFunc(ty);
}

static MwLLMathVTable* multiTableSetupAndGet(MwLLVecType ty) {
	mwLLMultiTable = default_multi_table();

#if defined(__i386__) || defined(__x86_64__)
	if(hasMMX()) {
		mmx_apply(mwLLMultiTable);
	}
#endif

	mwLLmathFunc = multiTableGet;

	return mwLLMultiTable[ty];
}
static MwLLMathVTable* multiTableGet(MwLLVecType ty) {
	return mwLLMultiTable[ty];
}

void MwLLMathAdd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->Add(a, b, out);
};
void MwLLMathSub(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->Sub(a, b, out);
};
void MwLLMathMultiply(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->Multiply(a, b, out);
};
void MwLLMathReciprocal(MwLLVec* a, MwLLVec* out) {
	assert(a->ty == out->ty);
	return getMultiTable(a->ty)->Reciprocal(a, out);
};
void MwLLMathSquareRoot(MwLLVec* a, MwLLVec* out) {
	assert(a->ty == out->ty);
	return getMultiTable(a->ty)->SquareRoot(a, out);
};

void MwLLMathShiftRight(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->ShiftRight(a, b, out);
};
void MwLLMathShiftLeft(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->ShiftLeft(a, b, out);
};
void MwLLMathEqual(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->Equal(a, b, out);
};
void MwLLMathGreaterThen(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	return getMultiTable(a->ty)->GreaterThen(a, b, out);
};
void MwLLMathAnd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.all = a->un.all & b->un.all;
};
void MwLLMathOr(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.all = a->un.all | b->un.all;
};
