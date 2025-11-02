/* $Id$ */
/*!
 * @file Mw/LowLevelMath.h
 * @brief A few portable functions for simultaneously supporting SIMD and not supporting it
 * @warning This is mostly used internally. Anything undocumented, and/or anything with an _ prefix (that doesn't have a corresponding typedef) should be avoided.
 */

#ifndef __MW_LOWLEVEL_MATH_H__
#define __MW_LOWLEVEL_MATH_H__

#include <Mw/BaseTypes.h>
#include <Mw/MachDep.h>

#if defined(__i386__) || defined(__x86_64__) || defined(__WATCOMC__)
#define MwLLMath_x86
#endif

/*!
 * @brief SIMD vector
 */
typedef struct _MwLLVec MwLLVec;

/*!
 * @brief SIMD vector type
 * @warning Not exhaustive, enums subject to be added later.
 */
enum MwLLVecType {
	MwLLVecTypeU8 = 0,
	MwLLVecTypeU16,
	MwLLVecTypeU32,
	MwLLVecTypeU64,
	MwLLVecTypeI8,
	MwLLVecTypeI16,
	MwLLVecTypeI32,
	MwLLVecTypeI64,

	MwLLVecType_Max,
};

/*!
 * @brief Create a SIMD Vector (variadic)
 * @warning Prefer using the macro version.
 */
MWDECL MwLLVec* MwLLVaVecCreate(int ty, MwU64 size, ...);

/*!
 * @brief Destroy the given SIMD Vector
 */
MWDECL void MwLLVecDestroy(MwLLVec* vec);

/*!
 * @brief index the given SIMD Vector (u8)
 */
MWDECL MwU8 MwLLVecIndexU8(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (u16)
 */
MWDECL MwU16 MwLLVecIndexU16(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (u32)
 */
MWDECL MwU32 MwLLVecIndexU32(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (u64)
 */
MWDECL MwU64 MwLLVecIndexU64(MwLLVec* vec, MwU64 index);

/*!
 * @brief index the given SIMD Vector (i8)
 */
MWDECL MwI8 MwLLVecIndexI8(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (i16)
 */
MWDECL MwI16 MwLLVecIndexI16(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (i32)
 */
MWDECL MwI32 MwLLVecIndexI32(MwLLVec* vec, MwU64 index);
/*!
 * @brief index the given SIMD Vector (i64)
 */
MWDECL MwI64 MwLLVecIndexI64(MwLLVec* vec, MwU64 index);

/*!
 * @brief SIMD Vector add
 */
MWDECL void MwLLMathAdd(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector multiply
 */
MWDECL void MwLLMathMultiply(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector subtract
 */
MWDECL void MwLLMathSub(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector reciprocal
 */
MWDECL void MwLLMathReciprocal(MwLLVec* a, MwLLVec* out);
/*!
 * @brief SIMD Vector square root
 */
MWDECL void MwLLMathSquareRoot(MwLLVec* a, MwLLVec* out);
/*!
 * @brief SIMD Vector bitwise and
 */
MWDECL void MwLLMathAnd(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector bitwise or
 */
MWDECL void MwLLMathOr(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector bitwise shift right
 */
MWDECL void MwLLMathShiftRight(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector bitwise shift left
 */
MWDECL void MwLLMathShiftLeft(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector bitwise equal
 */
MWDECL void MwLLMathEqual(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector greater then
 */
MWDECL void MwLLMathGreaterThen(MwLLVec* a, MwLLVec* b, MwLLVec* out);
/*!
 * @brief SIMD Vector lesser then
 */
MWDECL void MwLLMathLesserThen(MwLLVec* a, MwLLVec* b, MwLLVec* out);

#endif
