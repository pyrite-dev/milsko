/* $Id$ */
#include <Mw/LowLevelMath.h>
#include "math_internal.h"

#define MAKE_DEFAULT_TABLE(suffix, ty) \
	static void add_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] + ((ty*)b->buffer)[i]; \
		} \
	}; \
	static void sub_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] - ((ty*)b->buffer)[i]; \
		} \
	}; \
	static void multiply_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] * ((ty*)b->buffer)[i]; \
		} \
	}; \
	static void reciprocal_##suffix(MwLLVec* a, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = pow(((ty*)a->buffer)[i], -1); \
		} \
	}; \
	static void squareRoot_##suffix(MwLLVec* a, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = sqrt(((ty*)a->buffer)[i]); \
		} \
	} \
	static void shiftRight_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] >> ((ty*)b->buffer)[i]; \
		} \
	}; \
	static void shiftLeft_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] << ((ty*)b->buffer)[i]; \
		} \
	} \
	static void equal_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] == ((ty*)b->buffer)[i]; \
		} \
	}; \
	static void greaterThen_##suffix(MwLLVec* a, MwLLVec* b, MwLLVec* out) { \
		int i = 0; \
		for(; i < a->size; i++) { \
			((ty*)out->buffer)[i] = ((ty*)a->buffer)[i] >= ((ty*)b->buffer)[i]; \
		} \
	}; \
	static MwLLMathVTable table_##suffix = {add_##suffix, multiply_##suffix, sub_##suffix, reciprocal_##suffix, squareRoot_##suffix, NULL, NULL, shiftRight_##suffix, shiftLeft_##suffix, equal_##suffix, greaterThen_##suffix, NULL};

MAKE_DEFAULT_TABLE(u8, MwU8);
MAKE_DEFAULT_TABLE(u16, MwU16);
MAKE_DEFAULT_TABLE(u32, MwU32);
MAKE_DEFAULT_TABLE(u64, MwU64);
MAKE_DEFAULT_TABLE(i8, MwI8);
MAKE_DEFAULT_TABLE(i16, MwI16);
MAKE_DEFAULT_TABLE(i32, MwI32);
MAKE_DEFAULT_TABLE(i64, MwI64);

#if 0
static MwLLMathVTable* defaultMultiTable[MwLLVecType_Max] = {
    &table_u8,	/*MwLLVecTypeU8*/
    &table_u16, /*MwLLVecTypeU16*/
    &table_u32, /*MwLLVecTypeU32*/
    &table_u64, /*MwLLVecTypeU64*/
    &table_i8,	/*MwLLVecTypeI8*/
    &table_i16, /*MwLLVecTypeI16*/
    &table_i32, /*MwLLVecTypeI32*/
    &table_i64, /*MwLLVecTypeI64*/
};
#endif
void default_apply(MwLLVec* v) {
	switch(v->ty) {
	case MwLLVecTypeU8:
		v->vtable = table_u8;
		break;
	case MwLLVecTypeU16:
		v->vtable = table_u16;
		break;
	case MwLLVecTypeU32:
		v->vtable = table_u32;
		break;
	case MwLLVecTypeU64:
		v->vtable = table_u64;
		break;
	case MwLLVecTypeI8:
		v->vtable = table_i8;
		break;
	case MwLLVecTypeI16:
		v->vtable = table_i16;
		break;
	case MwLLVecTypeI32:
		v->vtable = table_i32;
		break;
	case MwLLVecTypeI64:
		v->vtable = table_i64;
		break;
	case MwLLVecType_Max:
		break;
	}
}
