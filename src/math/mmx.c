/* $Id$ */
#include <Mw/LowLevelMath.h>

#ifdef MwLLMathMMX
#include "math_internal.h"
#include <mmintrin.h>

#define DO_MMX_INTRINSIC(intrin, _ty, _rty, _tyn) \
	__m64	     m = intrin(*(__m64*)&a->un._ty, *(__m64*)&b->un._ty); \
	struct _tyn* t = (struct _tyn*)&m; \
	out->un._rty   = *t;

static void mmx_add_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddusb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_sub_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubusb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_equal_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqb, u8, u8, _MwLLVecDataU8x8);
}
static void mmx_greaterThen_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtb, u8, u8, _MwLLVecDataU8x8);
}

static void mmx_add_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddusw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_sub_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubusw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_shiftRight_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrlw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_shiftLeft_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psllw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_equal_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u16, u16, _MwLLVecDataU16x4);
}
static void mmx_greaterThen_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtw, u16, u16, _MwLLVecDataU16x4);
}

static void mmx_add_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddd, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_sub_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubd, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_shiftRight_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrld, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_shiftLeft_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pslld, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_equal_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u32, u32, _MwLLVecDataU32x2);
}
static void mmx_greaterThen_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtw, u32, u32, _MwLLVecDataU32x2);
}

static void mmx_add_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsb, i8, i8, _MwLLVecDataI8x8);
};
static void mmx_sub_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsb, i8, i8, _MwLLVecDataI8x8);
}

static void mmx_add_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsw, i16, i16, _MwLLVecDataI16x4);
}
static void mmx_sub_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsw, i16, i16, _MwLLVecDataI16x4);
}

void mmx_apply(MwLLMathVTable** t) {
	t[_MwLLVecTypeU8x8]->Add	  = mmx_add_u8;
	t[_MwLLVecTypeU8x8]->Sub	  = mmx_sub_u8;
	t[_MwLLVecTypeU8x8]->GreaterThen  = mmx_greaterThen_u8;
	t[_MwLLVecTypeU8x8]->Equal	  = mmx_equal_u8;
	t[_MwLLVecTypeU16x4]->Add	  = mmx_add_u16;
	t[_MwLLVecTypeU16x4]->Sub	  = mmx_sub_u16;
	t[_MwLLVecTypeU16x4]->ShiftLeft	  = mmx_shiftLeft_u16;
	t[_MwLLVecTypeU16x4]->ShiftRight  = mmx_shiftRight_u16;
	t[_MwLLVecTypeU16x4]->GreaterThen = mmx_greaterThen_u16;
	t[_MwLLVecTypeU16x4]->Equal	  = mmx_equal_u16;
	t[_MwLLVecTypeU32x2]->Add	  = mmx_add_u32;
	t[_MwLLVecTypeU32x2]->Sub	  = mmx_sub_u32;
	t[_MwLLVecTypeU32x2]->ShiftLeft	  = mmx_shiftLeft_u32;
	t[_MwLLVecTypeU32x2]->ShiftRight  = mmx_shiftRight_u32;
	t[_MwLLVecTypeU32x2]->GreaterThen = mmx_greaterThen_u32;
	t[_MwLLVecTypeU32x2]->Equal	  = mmx_equal_u32;

	t[_MwLLVecTypeI8x8]->Add	 = mmx_add_i8;
	t[_MwLLVecTypeI8x8]->Sub	 = mmx_sub_i8;
	t[_MwLLVecTypeI16x4]->Add	 = mmx_add_i16;
	t[_MwLLVecTypeI16x4]->Sub	 = mmx_sub_i16;
	t[_MwLLVecTypeI16x4]->ShiftLeft	 = mmx_shiftLeft_u16;
	t[_MwLLVecTypeI16x4]->ShiftRight = mmx_shiftRight_u16;
	t[_MwLLVecTypeI32x2]->ShiftLeft	 = mmx_shiftLeft_u32;
	t[_MwLLVecTypeI32x2]->ShiftRight = mmx_shiftRight_u32;
}
#endif
