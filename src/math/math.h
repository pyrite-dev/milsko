/* $Id$ */

#ifndef __MW_LOWLEVEL_INTERNAL_MATH_H__
#define __MW_LOWLEVEL_INTERNAL_MATH_H__

#include <Mw/LowLevelMath.h>

struct _MwLLMathVTable {
	void (*Add)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*Multiply)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*Sub)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*Reciprocal)(MwLLVec* a, MwLLVec* out);
	void (*SquareRoot)(MwLLVec* a, MwLLVec* out);
	void (*And)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*Or)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*ShiftRight)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*ShiftLeft)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*Equal)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*GreaterThen)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
	void (*LesserThen)(MwLLVec* a, MwLLVec* b, MwLLVec* out);
};

typedef struct _MwLLMathVTable MwLLMathVTable;

MwLLMathVTable** default_multi_table();
void		 mmx_apply(MwLLMathVTable**);

#endif
