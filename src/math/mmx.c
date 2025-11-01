/* $Id$ */
#include <Mw/LowLevelMath.h>

#ifdef MwLLMathMMX
#include "math_internal.h"
#include <mmintrin.h>

#define DO_MMX_INTRINSIC(intrin, _ty, _rty, _tyn) \
	__m64	     m = intrin(*(__m64*)&a->un._ty, *(__m64*)&b->un._ty); \
	struct _tyn* t = (struct _tyn*)&m; \
	out->un._rty   = *t;

static void mmx_add8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_sub8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_equal8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_add16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_sub16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_shiftRight16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrlw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_shiftLeft16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psllw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_equal16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u16, u16, _MwLLVecDataU16x4);
}

static void mmx_add32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddd, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_sub32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubd, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_shiftRight32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrld, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_shiftLeft32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pslld, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_equal32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u32, u32, _MwLLVecDataU32x2);
}

static void mmx_addi8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_subi8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_addi16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_subi16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_greaterTheni8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_greaterTheni16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_greaterTheni32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtd, u32, u32, _MwLLVecDataU32x2);
}

void mmx_apply(MwLLMathVTable** t) {
	t[_MwLLVecTypeU8x8]->Add = mmx_add8;
	t[_MwLLVecTypeU8x8]->Sub = mmx_sub8;
	// t[_MwLLVecTypeU8x8]->GreaterThen  = mmx_greaterThen8;
	t[_MwLLVecTypeU8x8]->Equal	 = mmx_equal8;
	t[_MwLLVecTypeU16x4]->Add	 = mmx_add16;
	t[_MwLLVecTypeU16x4]->Sub	 = mmx_sub16;
	t[_MwLLVecTypeU16x4]->ShiftLeft	 = mmx_shiftLeft16;
	t[_MwLLVecTypeU16x4]->ShiftRight = mmx_shiftRight16;
	// t[_MwLLVecTypeU16x4]->GreaterThen = mmx_greaterThen16;
	t[_MwLLVecTypeU16x4]->Equal	 = mmx_equal16;
	t[_MwLLVecTypeU32x2]->Add	 = mmx_add32;
	t[_MwLLVecTypeU32x2]->Sub	 = mmx_sub32;
	t[_MwLLVecTypeU32x2]->ShiftLeft	 = mmx_shiftLeft32;
	t[_MwLLVecTypeU32x2]->ShiftRight = mmx_shiftRight32;
	// t[_MwLLVecTypeU32x2]->GreaterThen = mmx_greaterThen32;
	t[_MwLLVecTypeU32x2]->Equal = mmx_equal32;

	t[_MwLLVecTypeU8x8]->Add	  = mmx_addi8;
	t[_MwLLVecTypeU8x8]->Sub	  = mmx_subi8;
	t[_MwLLVecTypeI8x8]->GreaterThen  = mmx_greaterTheni8;
	t[_MwLLVecTypeI8x8]->Equal	  = mmx_equal8;
	t[_MwLLVecTypeU16x4]->Add	  = mmx_addi16;
	t[_MwLLVecTypeU16x4]->Sub	  = mmx_subi16;
	t[_MwLLVecTypeI16x4]->ShiftLeft	  = mmx_shiftLeft16;
	t[_MwLLVecTypeI16x4]->ShiftRight  = mmx_shiftRight16;
	t[_MwLLVecTypeI16x4]->GreaterThen = mmx_greaterTheni16;
	t[_MwLLVecTypeI16x4]->Equal	  = mmx_equal16;
	t[_MwLLVecTypeI32x2]->Add	  = mmx_add32;
	t[_MwLLVecTypeI32x2]->Sub	  = mmx_sub32;
	t[_MwLLVecTypeI32x2]->ShiftLeft	  = mmx_shiftLeft32;
	t[_MwLLVecTypeI32x2]->ShiftRight  = mmx_shiftRight32;
	t[_MwLLVecTypeI32x2]->GreaterThen = mmx_greaterTheni32;
	t[_MwLLVecTypeI32x2]->Equal	  = mmx_equal32;
}
#endif
