/* $Id$ */

#ifndef __MW_LOWLEVEL_INTERNAL_MATH_H__
#define __MW_LOWLEVEL_INTERNAL_MATH_H__

#include <Mw/BaseTypes.h>
#include <Mw/LowLevelMath.h>

struct _cpuFeatures {
	MwBool fpu : 1;
	MwBool vme : 1;
	MwBool de : 1;
	MwBool pse : 1;
	MwBool tsc : 1;
	MwBool msr : 1;
	MwBool pae : 1;
	MwBool mce : 1;
	MwBool cx8 : 1;
	MwBool apic : 1;
	MwBool sep : 1;
	MwBool mtrr : 1;
	MwBool pge : 1;
	MwBool mca : 1;
	MwBool cmov : 1;
	MwBool pat : 1;
	MwBool pse36 : 1;
	MwBool psn : 1;
	MwBool clflush : 1;
	MwBool ds : 1;
	MwBool acpi : 1;
	MwBool mmx : 1;
	MwBool fxsr : 1;
	MwBool sse : 1;
	MwBool sse2 : 1;
	MwBool ss : 1;
	MwBool hit : 1;
	MwBool tm : 1;
	MwBool pbe : 1;
};

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
typedef struct _cpuFeatures    cpuFeatures;

MwLLMathVTable** default_multi_table(void);
void		 mmx_apply(MwLLMathVTable**);

#endif
