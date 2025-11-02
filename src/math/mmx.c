/* $Id$ */
#include <Mw/LowLevelMath.h>

#ifdef MwLLMath_x86
#include "math_internal.h"
#include <mmintrin.h>

#define DO_MMX_INTRINSIC(intrin, _ty, _cty) \
	int i = 0; \
	for(; i < a->size; i += 8) { \
		__m64 m = intrin(*(__m64*)&a->buffer[i], *(__m64*)&b->buffer[i]); \
		memcpy(&((_cty*)out->buffer)[i], &m, sizeof(m)); \
	}

static void mmx_add8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddb, u8, MwU8)
}
static void mmx_sub8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubb, u8, MwU8);
}
static void mmx_equal8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqb, u8, MwU8);
}
static void mmx_add16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddw, u16, MwU16);
}
static void mmx_sub16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubw, u16, MwU16);
}
static void mmx_shiftRight16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrlw, u16, MwU16);
}
static void mmx_shiftLeft16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psllw, u16, MwU16);
}
static void mmx_equal16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u16, MwU16);
}

static void mmx_add32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddd, u32, MwU32);
}
static void mmx_sub32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubd, u32, MwU32);
}
static void mmx_shiftRight32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psrld, u32, MwU32);
}
static void mmx_shiftLeft32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pslld, u32, MwU32);
}
static void mmx_equal32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpeqw, u32, MwU32);
}

static void mmx_addi8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsb, i8, MwI8);
}
static void mmx_subi8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsb, i8, MwI8);
}
static void mmx_addi16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_paddsw, i16, MwI16);
}
static void mmx_subi16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_psubsw, i16, MwI16);
}
static void mmx_greaterTheni8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtb, i8, MwI8);
}
static void mmx_greaterTheni16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtw, i16, MwI16);
}
static void mmx_greaterTheni32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	DO_MMX_INTRINSIC(_m_pcmpgtd, i32, MwI16);
}

void mmx_apply(MwLLVec* vec) {
	switch(vec->ty) {
	case MwLLVecTypeU8:
		vec->vtable.add	  = mmx_add8;
		vec->vtable.sub	  = mmx_sub8;
		vec->vtable.equal = mmx_equal8;
		break;
	case MwLLVecTypeU16:
		vec->vtable.add	       = mmx_add16;
		vec->vtable.sub	       = mmx_sub16;
		vec->vtable.shiftLeft  = mmx_shiftLeft16;
		vec->vtable.shiftRight = mmx_shiftRight16;
		vec->vtable.equal      = mmx_equal16;
		break;
	case MwLLVecTypeU32:
		vec->vtable.add	       = mmx_add32;
		vec->vtable.sub	       = mmx_sub32;
		vec->vtable.shiftLeft  = mmx_shiftLeft32;
		vec->vtable.shiftRight = mmx_shiftRight32;
		vec->vtable.equal      = mmx_equal32;
		break;
	case MwLLVecTypeU64:
		break;
	case MwLLVecTypeI8:
		vec->vtable.add		= mmx_addi8;
		vec->vtable.sub		= mmx_subi8;
		vec->vtable.greaterThen = mmx_greaterTheni8;
		vec->vtable.equal	= mmx_equal8;
		break;
	case MwLLVecTypeI16:
		vec->vtable.add		= mmx_addi16;
		vec->vtable.sub		= mmx_subi16;
		vec->vtable.shiftLeft	= mmx_shiftLeft16;
		vec->vtable.shiftRight	= mmx_shiftRight16;
		vec->vtable.greaterThen = mmx_greaterTheni16;
		vec->vtable.equal	= mmx_equal16;
		break;
	case MwLLVecTypeI32:
		vec->vtable.add		= mmx_add32;
		vec->vtable.sub		= mmx_sub32;
		vec->vtable.shiftLeft	= mmx_shiftLeft32;
		vec->vtable.shiftRight	= mmx_shiftRight32;
		vec->vtable.greaterThen = mmx_greaterTheni32;
		vec->vtable.equal	= mmx_equal32;
		break;
	case MwLLVecTypeI64:
		break;
	case MwLLVecType_Max:
		break;
	}
}
#endif
