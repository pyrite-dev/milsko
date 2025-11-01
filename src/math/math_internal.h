/* $Id$ */

#ifndef __MW_LOWLEVEL_INTERNAL_MATH_H__
#define __MW_LOWLEVEL_INTERNAL_MATH_H__

#include <Mw/BaseTypes.h>
#include <Mw/LowLevelMath.h>

/* Bitfield of cpu features we get from x86's CPUID */
#if defined(__WATCOMC__) || defined(__i386__) || defined(__amd64__)
typedef struct _cpuFeatures cpuFeatures;
struct _cpuFeatures {
	MwBool fpu : 1;	    /* x87 FPU on chip */
	MwBool vme : 1;	    /* Virtual-8086 Mode Enhancement */
	MwBool de : 1;	    /* Debugging Extensions */
	MwBool pse : 1;	    /* Page Size Extensions */
	MwBool tsc : 1;	    /* Time Stamp Counter */
	MwBool msr : 1;	    /* RDMSR and WRMSR Support */
	MwBool pae : 1;	    /* Physical Address Extensions */
	MwBool mce : 1;	    /* Machine Check Exception */
	MwBool cx8 : 1;	    /* CMPXCHG8B instr */
	MwBool apic : 1;    /* APIC on Chip */
	MwBool sep : 1;	    /* SYSENTER and SYSEXIT instrs */
	MwBool mtrr : 1;    /* Memory Type Range Registers */
	MwBool pge : 1;	    /* Page Global Bit */
	MwBool mca : 1;	    /* Machine Check Architecture */
	MwBool cmov : 1;    /* Conditional Move Instrs */
	MwBool pat : 1;	    /* Page Attribute Table */
	MwBool pse36 : 1;   /* 36-Bit Page Size Extension */
	MwBool psn : 1;	    /* Processor Serial Number */
	MwBool clflush : 1; /* CLFLUSH instr */
	MwBool ds : 1;	    /* Debug Store */
	MwBool acpi : 1;    /* Thermal Monitor and Software Controlled Clock Facilities */
	MwBool mmx : 1;	    /* Intel MMX Technology */
	MwBool fxsr : 1;    /* XSAVE and FXRSTOR Instrs */
	MwBool sse : 1;	    /* SSE */
	MwBool sse2 : 1;    /* SSE2 */
	MwBool ss : 1;	    /* Self Snoop */
	MwBool hit : 1;	    /* Max APIC IDs */
	MwBool tm : 1;	    /* Thermal Monitor */
	MwBool pbe : 1;	    /* Pending Break Enable */
};
#endif

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

MwLLMathVTable** default_multi_table(void);
void		 mmx_apply(MwLLMathVTable**);

#endif
