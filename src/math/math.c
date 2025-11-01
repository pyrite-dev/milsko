/* $Id$ */
#include <Mw/LowLevelMath.h>

#include "math_internal.h"

MwLLVec _MwLLVecCreateGeneric(int ty, ...) {
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

#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
static MwU32 getCPUFeatures(void) {
	MwU32	    _eax = 1;
	MwU32 _edx;

#ifdef __WATCOMC__
	__asm {
		cpuid
		mov _eax, eax
		mov _edx, edx
	}
#else
	asm volatile (
	    "cpuid" : "=a"(_eax), "=d"(_edx)
	    : "a"(1) : "ebx", "ecx");
#endif

	return _edx;
}
#endif

static MwLLMathVTable** mwLLMultiTable;
static MwLLMathVTable*	multiTableSetupAndGet(int ty);
static MwLLMathVTable*	multiTableGet(int ty);

static MwLLMathVTable* (*mwLLmathFunc)(int ty) = multiTableSetupAndGet;

static MwLLMathVTable* getMultiTable(int ty) {
	return mwLLmathFunc(ty);
}

static MwLLMathVTable* multiTableSetupAndGet(int ty) {
#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
	MwU32 features;
#endif

	mwLLMultiTable = default_multi_table();

#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
	features = getCPUFeatures();
	printf("Avaliable x86_64 Features:\n");
	printf("\tMMX: %s\n", features & FEATX86_MMX ? "true" : "false");
	printf("\tSSE: %s\n", features & FEATX86_SSE ? "true" : "false");
	printf("\tSSE2: %s\n", features & FEATX86_SSE2 ? "true" : "false");

	if(features & FEATX86_MMX) {
		mmx_apply(mwLLMultiTable);
	}
#endif

	mwLLmathFunc = multiTableGet;

	return mwLLMultiTable[ty];
}
static MwLLMathVTable* multiTableGet(int ty) {
	return mwLLMultiTable[ty];
}

void MwLLMathAdd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->Add(a, b, out);
}
void MwLLMathSub(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->Sub(a, b, out);
}
void MwLLMathMultiply(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->Multiply(a, b, out);
}
void MwLLMathReciprocal(MwLLVec* a, MwLLVec* out) {
	assert(a->ty == out->ty);
	getMultiTable(a->ty)->Reciprocal(a, out);
}
void MwLLMathSquareRoot(MwLLVec* a, MwLLVec* out) {
	assert(a->ty == out->ty);
	getMultiTable(a->ty)->SquareRoot(a, out);
}

void MwLLMathShiftRight(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->ShiftRight(a, b, out);
}
void MwLLMathShiftLeft(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->ShiftLeft(a, b, out);
}
void MwLLMathEqual(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->Equal(a, b, out);
}
void MwLLMathGreaterThen(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	assert(a->ty == b->ty && a->ty == out->ty && b->ty == out->ty);
	getMultiTable(a->ty)->GreaterThen(a, b, out);
}
void MwLLMathAnd(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.all = a->un.all & b->un.all;
}
void MwLLMathOr(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.all = a->un.all | b->un.all;
}
