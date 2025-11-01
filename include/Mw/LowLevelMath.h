/* $Id$ */
/*!
 * @file Mw/LowLevelMath.h
 * @brief A few portable functions for supporting simultaneously supporting SIMD and not supporting it
 * @warning This is mostly used internally. Anything undocumented, and/or anything with an _ prefix (that doesn't have a corresponding typedef) should be avoided.
 */

#ifndef __MW_LOWLEVEL_MATH_H__
#define __MW_LOWLEVEL_MATH_H__

#include <Mw/BaseTypes.h>
#include <Mw/MachDep.h>

#if !defined(__i386__) && !defined(__x86_64__) && !defined(__WATCOMC__)
#warning LowLevelMath.h does not yet support non-X86 platforms
#endif

/*!
 * @brief Generic vector type
 * @warning Do not try to instantiate this yourself, use the appropriate functions instead.
 */
typedef struct _MwLLVec MwLLVec;

typedef enum _MwLLVecType   MwLLVecType;
typedef union _MwLLVecUnion MwLLVecUnion;

// clang-format off
struct _MwLLVecDataU8x8  { MwU8 a; MwU8 b; MwU8 c; MwU8 d; MwU8 e; MwU8 f; MwU8 g; MwU8 h;};
struct _MwLLVecDataU16x4 { MwU16 a; MwU16 b; MwU16 c; MwU16 d;};
struct _MwLLVecDataU32x2 { MwU32 a; MwU32 b;};
struct _MwLLVecDataI8x8  { MwI8 a; MwI8 b; MwI8 c; MwI8 d; MwI8 e; MwI8 f; MwI8 g; MwI8 h;};
struct _MwLLVecDataI16x4 { MwI16 a; MwI16 b; MwI16 c; MwI16 d;};
struct _MwLLVecDataI32x2 { MwI32 a; MwI32 b;};
union _MwLLVecUnion {
	struct _MwLLVecDataU8x8 u8; struct _MwLLVecDataU16x4 u16; struct _MwLLVecDataU32x2 u32;
	struct _MwLLVecDataI8x8 i8; struct _MwLLVecDataI16x4 i16; struct _MwLLVecDataI32x2 i32;
	MwU64 all;
};
// clang-format on
enum _MwLLVecType {
	_MwLLVecTypeU8x8  = 0,
	_MwLLVecTypeU16x4 = 1,
	_MwLLVecTypeU32x2 = 2,
	_MwLLVecTypeI8x8  = 3,
	_MwLLVecTypeI16x4 = 4,
	_MwLLVecTypeI32x2 = 5,

	_MwLLVecType_Max,
};
struct _MwLLVec {
	MwLLVecType  ty;
	MwLLVecUnion un;
};

MwLLVec _MwLLVecCreateGeneric(MwLLVecType ty, ...);

#define MwLLVecU8x8(a, b, c, d, e, f, g, h) _MwLLVecCreateGeneric(_MwLLVecTypeU8x8, a, b, c, d, e, f, g, h)
#define MwLLVecU16x4(a, b, c, d) _MwLLVecCreateGeneric(_MwLLVecTypeU16x4, a, b, c, d)
#define MwLLVecU32x2(a, b) _MwLLVecCreateGeneric(_MwLLVecTypeU32x2, a, b)
#define MwLLVecI8x8(a, b, c, d, e, f, g, h) _MwLLVecCreateGeneric(_MwLLVecTypeI8x8, a, b, c, d, e, f, g, h)
#define MwLLVecI16x4(a, b, c, d) _MwLLVecCreateGeneric(_MwLLVecTypeI16x4, a, b, c, d)
#define MwLLVecI32x2(a, b) _MwLLVecCreateGeneric(_MwLLVecTypeI32x2, a, b)

void MwLLMathAdd(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathMultiply(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathSub(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathReciprocal(MwLLVec* a, MwLLVec* out);
void MwLLMathSquareRoot(MwLLVec* a, MwLLVec* out);
void MwLLMathAnd(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathOr(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathShiftRight(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathShiftLeft(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathEqual(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathGreaterThen(MwLLVec* a, MwLLVec* b, MwLLVec* out);
void MwLLMathLesserThen(MwLLVec* a, MwLLVec* b, MwLLVec* out);

#endif
