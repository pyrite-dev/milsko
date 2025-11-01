#include <Mw/LowLevelMath.h>
#include "math.h"
#include <assert.h>
#include <math.h>

static void default_add_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a + b->un.u8.a;
	out->un.u8.b = a->un.u8.b + b->un.u8.b;
	out->un.u8.c = a->un.u8.c + b->un.u8.c;
	out->un.u8.d = a->un.u8.d + b->un.u8.d;
	out->un.u8.e = a->un.u8.e + b->un.u8.e;
	out->un.u8.f = a->un.u8.f + b->un.u8.f;
	out->un.u8.g = a->un.u8.g + b->un.u8.g;
	out->un.u8.h = a->un.u8.h + b->un.u8.h;
};
static void default_sub_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a - b->un.u8.a;
	out->un.u8.b = a->un.u8.b - b->un.u8.b;
	out->un.u8.c = a->un.u8.c - b->un.u8.c;
	out->un.u8.d = a->un.u8.d - b->un.u8.d;
	out->un.u8.e = a->un.u8.e - b->un.u8.e;
	out->un.u8.f = a->un.u8.f - b->un.u8.f;
	out->un.u8.g = a->un.u8.g - b->un.u8.g;
	out->un.u8.h = a->un.u8.h - b->un.u8.h;
};
static void default_multiply_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a * b->un.u8.a;
	out->un.u8.b = a->un.u8.b * b->un.u8.b;
	out->un.u8.c = a->un.u8.c * b->un.u8.c;
	out->un.u8.d = a->un.u8.d * b->un.u8.d;
	out->un.u8.e = a->un.u8.e * b->un.u8.e;
	out->un.u8.f = a->un.u8.f * b->un.u8.f;
	out->un.u8.g = a->un.u8.g * b->un.u8.g;
	out->un.u8.h = a->un.u8.h * b->un.u8.h;
};
static void default_reciprocal_u8(MwLLVec* a, MwLLVec* out) {
	out->un.u8.a = powf(a->un.u8.a, -1);
	out->un.u8.b = powf(a->un.u8.b, -1);
	out->un.u8.c = powf(a->un.u8.c, -1);
	out->un.u8.d = powf(a->un.u8.d, -1);
	out->un.u8.e = powf(a->un.u8.e, -1);
	out->un.u8.f = powf(a->un.u8.f, -1);
	out->un.u8.g = powf(a->un.u8.g, -1);
	out->un.u8.h = powf(a->un.u8.h, -1);
};
static void default_squareRoot_u8(MwLLVec* a, MwLLVec* out) {
	out->un.u8.a = sqrt(a->un.u8.a);
	out->un.u8.b = sqrt(a->un.u8.b);
	out->un.u8.c = sqrt(a->un.u8.c);
	out->un.u8.d = sqrt(a->un.u8.d);
	out->un.u8.e = sqrt(a->un.u8.e);
	out->un.u8.f = sqrt(a->un.u8.f);
	out->un.u8.g = sqrt(a->un.u8.g);
	out->un.u8.h = sqrt(a->un.u8.h);
}
static void default_shiftRight_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a >> b->un.u8.a;
	out->un.u8.b = a->un.u8.b >> b->un.u8.b;
	out->un.u8.c = a->un.u8.c >> b->un.u8.c;
	out->un.u8.d = a->un.u8.d >> b->un.u8.d;
	out->un.u8.e = a->un.u8.e >> b->un.u8.e;
	out->un.u8.f = a->un.u8.f >> b->un.u8.f;
	out->un.u8.g = a->un.u8.g >> b->un.u8.g;
	out->un.u8.h = a->un.u8.h >> b->un.u8.h;
};
static void default_shiftLeft_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a << b->un.u8.a;
	out->un.u8.b = a->un.u8.b << b->un.u8.b;
	out->un.u8.c = a->un.u8.c << b->un.u8.c;
	out->un.u8.d = a->un.u8.d << b->un.u8.d;
	out->un.u8.e = a->un.u8.e << b->un.u8.e;
	out->un.u8.f = a->un.u8.f << b->un.u8.f;
	out->un.u8.g = a->un.u8.g << b->un.u8.g;
	out->un.u8.h = a->un.u8.h << b->un.u8.h;
}
static void default_equal_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a == b->un.u8.a;
	out->un.u8.b = a->un.u8.b == b->un.u8.b;
	out->un.u8.c = a->un.u8.c == b->un.u8.c;
	out->un.u8.d = a->un.u8.d == b->un.u8.d;
	out->un.u8.e = a->un.u8.e == b->un.u8.e;
	out->un.u8.f = a->un.u8.f == b->un.u8.f;
	out->un.u8.g = a->un.u8.g == b->un.u8.g;
	out->un.u8.h = a->un.u8.h == b->un.u8.h;
};
static void default_greaterThen_u8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u8.a = a->un.u8.a >= b->un.u8.a;
	out->un.u8.b = a->un.u8.b >= b->un.u8.b;
	out->un.u8.c = a->un.u8.c >= b->un.u8.c;
	out->un.u8.d = a->un.u8.d >= b->un.u8.d;
	out->un.u8.e = a->un.u8.e >= b->un.u8.e;
	out->un.u8.f = a->un.u8.f >= b->un.u8.f;
	out->un.u8.g = a->un.u8.g >= b->un.u8.g;
	out->un.u8.h = a->un.u8.h >= b->un.u8.h;
};
static MwLLMathVTable table_u8 = {
    .Add	 = default_add_u8,
    .Sub	 = default_sub_u8,
    .Multiply	 = default_multiply_u8,
    .Reciprocal	 = default_reciprocal_u8,
    .SquareRoot	 = default_squareRoot_u8,
    .ShiftRight	 = default_shiftRight_u8,
    .ShiftLeft	 = default_shiftLeft_u8,
    .Equal	 = default_equal_u8,
    .GreaterThen = default_greaterThen_u8,
};
static void default_add_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a + b->un.u16.a;
	out->un.u16.b = a->un.u16.b + b->un.u16.b;
	out->un.u16.c = a->un.u16.c + b->un.u16.c;
	out->un.u16.d = a->un.u16.d + b->un.u16.d;
}
static void default_sub_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a - b->un.u16.a;
	out->un.u16.b = a->un.u16.b - b->un.u16.b;
	out->un.u16.c = a->un.u16.c - b->un.u16.c;
	out->un.u16.d = a->un.u16.d - b->un.u16.d;
}
static void default_multiply_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a * b->un.u16.a;
	out->un.u16.b = a->un.u16.b * b->un.u16.b;
	out->un.u16.c = a->un.u16.c * b->un.u16.c;
	out->un.u16.d = a->un.u16.d * b->un.u16.d;
}
static void default_reciprocal_u16(MwLLVec* a, MwLLVec* out) {
	out->un.u16.a = powf(a->un.u16.a, -1);
	out->un.u16.b = powf(a->un.u16.b, -1);
	out->un.u16.c = powf(a->un.u16.c, -1);
	out->un.u16.d = powf(a->un.u16.d, -1);
};
static void default_squareRoot_u16(MwLLVec* a, MwLLVec* out) {
	out->un.u16.a = sqrt(a->un.u16.a);
	out->un.u16.b = sqrt(a->un.u16.b);
	out->un.u16.c = sqrt(a->un.u16.c);
	out->un.u16.d = sqrt(a->un.u16.d);
};

static void default_shiftRight_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a >> b->un.u16.a;
	out->un.u16.b = a->un.u16.b >> b->un.u16.b;
	out->un.u16.c = a->un.u16.c >> b->un.u16.c;
	out->un.u16.d = a->un.u16.d >> b->un.u16.d;
};
static void default_shiftLeft_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a << b->un.u16.a;
	out->un.u16.b = a->un.u16.b << b->un.u16.b;
	out->un.u16.c = a->un.u16.c << b->un.u16.c;
	out->un.u16.d = a->un.u16.d << b->un.u16.d;
}
static void default_equal_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a == b->un.u16.a;
	out->un.u16.b = a->un.u16.b == b->un.u16.b;
	out->un.u16.c = a->un.u16.c == b->un.u16.c;
	out->un.u16.d = a->un.u16.d == b->un.u16.d;
}
static void default_greaterThen_u16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u16.a = a->un.u16.a >= b->un.u16.a;
	out->un.u16.b = a->un.u16.b >= b->un.u16.b;
	out->un.u16.c = a->un.u16.c >= b->un.u16.c;
	out->un.u16.d = a->un.u16.d >= b->un.u16.d;
}
static MwLLMathVTable table_u16 = {
    .Add	 = default_add_u16,
    .Sub	 = default_sub_u16,
    .Multiply	 = default_multiply_u16,
    .Reciprocal	 = default_reciprocal_u16,
    .SquareRoot	 = default_squareRoot_u16,
    .ShiftRight	 = default_shiftRight_u16,
    .ShiftLeft	 = default_shiftLeft_u16,
    .Equal	 = default_equal_u16,
    .GreaterThen = default_greaterThen_u16,
};

static void default_add_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a + b->un.u32.a;
	out->un.u32.b = a->un.u32.b + b->un.u32.b;
}
static void default_sub_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a - b->un.u32.a;
	out->un.u32.b = a->un.u32.b - b->un.u32.b;
}
static void default_multiply_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a * b->un.u32.a;
	out->un.u32.b = a->un.u32.b * b->un.u32.b;
}
static void default_reciprocal_u32(MwLLVec* a, MwLLVec* out) {
	out->un.u32.a = powf(a->un.u32.a, -1);
	out->un.u32.b = powf(a->un.u32.b, -1);
};
static void default_squareRoot_u32(MwLLVec* a, MwLLVec* out) {
	out->un.u32.a = sqrt(a->un.u32.a);
	out->un.u32.b = sqrt(a->un.u32.b);
};

static void default_shiftRight_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a >> b->un.u32.a;
	out->un.u32.b = a->un.u32.b >> b->un.u32.b;
};
static void default_shiftLeft_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a << b->un.u32.a;
	out->un.u32.b = a->un.u32.b << b->un.u32.b;
}
static void default_equal_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a == b->un.u32.a;
	out->un.u32.b = a->un.u32.b == b->un.u32.b;
}
static void default_greaterThen_u32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.u32.a = a->un.u32.a >= b->un.u32.a;
	out->un.u32.b = a->un.u32.b >= b->un.u32.b;
}
static MwLLMathVTable table_u32 = {
    .Add	 = default_add_u32,
    .Sub	 = default_sub_u32,
    .Multiply	 = default_multiply_u32,
    .Reciprocal	 = default_reciprocal_u32,
    .SquareRoot	 = default_squareRoot_u32,
    .ShiftRight	 = default_shiftRight_u32,
    .ShiftLeft	 = default_shiftLeft_u32,
    .Equal	 = default_equal_u32,
    .GreaterThen = default_greaterThen_u32,
};

static void default_add_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i8.a = a->un.i8.a + b->un.i8.a;
	out->un.i8.b = a->un.i8.b + b->un.i8.b;
	out->un.i8.c = a->un.i8.c + b->un.i8.c;
	out->un.i8.d = a->un.i8.d + b->un.i8.d;
	out->un.i8.e = a->un.i8.e + b->un.i8.e;
	out->un.i8.f = a->un.i8.f + b->un.i8.f;
	out->un.i8.g = a->un.i8.g + b->un.i8.g;
	out->un.i8.h = a->un.i8.h + b->un.i8.h;
};
static void default_sub_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i8.a = a->un.i8.a - b->un.i8.a;
	out->un.i8.b = a->un.i8.b - b->un.i8.b;
	out->un.i8.c = a->un.i8.c - b->un.i8.c;
	out->un.i8.d = a->un.i8.d - b->un.i8.d;
	out->un.i8.e = a->un.i8.e - b->un.i8.e;
	out->un.i8.f = a->un.i8.f - b->un.i8.f;
	out->un.i8.g = a->un.i8.g - b->un.i8.g;
	out->un.i8.h = a->un.i8.h - b->un.i8.h;
};
static void default_multiply_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i8.a = a->un.i8.a * b->un.i8.a;
	out->un.i8.b = a->un.i8.b * b->un.i8.b;
	out->un.i8.c = a->un.i8.c * b->un.i8.c;
	out->un.i8.d = a->un.i8.d * b->un.i8.d;
	out->un.i8.e = a->un.i8.e * b->un.i8.e;
	out->un.i8.f = a->un.i8.f * b->un.i8.f;
	out->un.i8.g = a->un.i8.g * b->un.i8.g;
	out->un.i8.h = a->un.i8.h * b->un.i8.h;
};
static void default_reciprocal_i8(MwLLVec* a, MwLLVec* out) {
	out->un.i8.a = powf(a->un.i8.a, -1);
	out->un.i8.b = powf(a->un.i8.b, -1);
	out->un.i8.c = powf(a->un.i8.c, -1);
	out->un.i8.d = powf(a->un.i8.d, -1);
	out->un.i8.e = powf(a->un.i8.e, -1);
	out->un.i8.f = powf(a->un.i8.f, -1);
	out->un.i8.g = powf(a->un.i8.g, -1);
	out->un.i8.h = powf(a->un.i8.h, -1);
};
static void default_squareRoot_i8(MwLLVec* a, MwLLVec* out) {
	out->un.i8.a = sqrt(a->un.i8.a);
	out->un.i8.b = sqrt(a->un.i8.b);
	out->un.i8.c = sqrt(a->un.i8.c);
	out->un.i8.d = sqrt(a->un.i8.d);
	out->un.i8.e = sqrt(a->un.i8.e);
	out->un.i8.f = sqrt(a->un.i8.f);
	out->un.i8.g = sqrt(a->un.i8.g);
	out->un.i8.h = sqrt(a->un.i8.h);
}

static void default_equal_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i8.a = a->un.i8.a == b->un.i8.a;
	out->un.i8.b = a->un.i8.b == b->un.i8.b;
	out->un.i8.c = a->un.i8.c == b->un.i8.c;
	out->un.i8.d = a->un.i8.d == b->un.i8.d;
	out->un.i8.e = a->un.i8.e == b->un.i8.e;
	out->un.i8.f = a->un.i8.f == b->un.i8.f;
	out->un.i8.g = a->un.i8.g == b->un.i8.g;
	out->un.i8.h = a->un.i8.h == b->un.i8.h;
};
static void default_greaterThen_i8(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i8.a = a->un.i8.a >= b->un.i8.a;
	out->un.i8.b = a->un.i8.b >= b->un.i8.b;
	out->un.i8.c = a->un.i8.c >= b->un.i8.c;
	out->un.i8.d = a->un.i8.d >= b->un.i8.d;
	out->un.i8.e = a->un.i8.e >= b->un.i8.e;
	out->un.i8.f = a->un.i8.f >= b->un.i8.f;
	out->un.i8.g = a->un.i8.g >= b->un.i8.g;
	out->un.i8.h = a->un.i8.h >= b->un.i8.h;
};
static MwLLMathVTable table_i8 = {
    .Add	 = default_add_i8,
    .Sub	 = default_sub_i8,
    .Multiply	 = default_multiply_i8,
    .Reciprocal	 = default_reciprocal_i8,
    .SquareRoot	 = default_squareRoot_i8,
    .ShiftRight	 = default_shiftRight_u8,
    .ShiftLeft	 = default_shiftLeft_u8,
    .Equal	 = default_equal_i8,
    .GreaterThen = default_greaterThen_i8,
};
static void default_add_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i16.a = a->un.i16.a + b->un.i16.a;
	out->un.i16.b = a->un.i16.b + b->un.i16.b;
	out->un.i16.c = a->un.i16.c + b->un.i16.c;
	out->un.i16.d = a->un.i16.d + b->un.i16.d;
}
static void default_sub_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i16.a = a->un.i16.a - b->un.i16.a;
	out->un.i16.b = a->un.i16.b - b->un.i16.b;
	out->un.i16.c = a->un.i16.c - b->un.i16.c;
	out->un.i16.d = a->un.i16.d - b->un.i16.d;
}
static void default_multiply_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i16.a = a->un.i16.a * b->un.i16.a;
	out->un.i16.b = a->un.i16.b * b->un.i16.b;
	out->un.i16.c = a->un.i16.c * b->un.i16.c;
	out->un.i16.d = a->un.i16.d * b->un.i16.d;
}
static void default_reciprocal_i16(MwLLVec* a, MwLLVec* out) {
	out->un.i16.a = powf(a->un.i16.a, -1);
	out->un.i16.b = powf(a->un.i16.b, -1);
	out->un.i16.c = powf(a->un.i16.c, -1);
	out->un.i16.d = powf(a->un.i16.d, -1);
};
static void default_squareRoot_i16(MwLLVec* a, MwLLVec* out) {
	out->un.i16.a = sqrt(a->un.i16.a);
	out->un.i16.b = sqrt(a->un.i16.b);
	out->un.i16.c = sqrt(a->un.i16.c);
	out->un.i16.d = sqrt(a->un.i16.d);
};

static void default_equal_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i16.a = a->un.i16.a == b->un.i16.a;
	out->un.i16.b = a->un.i16.b == b->un.i16.b;
	out->un.i16.c = a->un.i16.c == b->un.i16.c;
	out->un.i16.d = a->un.i16.d == b->un.i16.d;
}
static void default_greaterThen_i16(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i16.a = a->un.i16.a >= b->un.i16.a;
	out->un.i16.b = a->un.i16.b >= b->un.i16.b;
	out->un.i16.c = a->un.i16.c >= b->un.i16.c;
	out->un.i16.d = a->un.i16.d >= b->un.i16.d;
}
static MwLLMathVTable table_i16 = {
    .Add	 = default_add_i16,
    .Sub	 = default_sub_i16,
    .Multiply	 = default_multiply_i16,
    .Reciprocal	 = default_reciprocal_i16,
    .SquareRoot	 = default_squareRoot_i16,
    .ShiftRight	 = default_shiftRight_u16,
    .ShiftLeft	 = default_shiftLeft_u16,
    .Equal	 = default_equal_i16,
    .GreaterThen = default_greaterThen_i16,
};

static void default_add_i32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i32.a = a->un.i32.a + b->un.i32.a;
	out->un.i32.b = a->un.i32.b + b->un.i32.b;
}
static void default_sub_i32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i32.a = a->un.i32.a - b->un.i32.a;
	out->un.i32.b = a->un.i32.b - b->un.i32.b;
}
static void default_multiply_i32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i32.a = a->un.i32.a * b->un.i32.a;
	out->un.i32.b = a->un.i32.b * b->un.i32.b;
}
static void default_reciprocal_i32(MwLLVec* a, MwLLVec* out) {
	out->un.i32.a = powf(a->un.i32.a, -1);
	out->un.i32.b = powf(a->un.i32.b, -1);
};
static void default_squareRoot_i32(MwLLVec* a, MwLLVec* out) {
	out->un.i32.a = sqrt(a->un.i32.a);
	out->un.i32.b = sqrt(a->un.i32.b);
};

static void default_equal_i32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i32.a = a->un.i32.a == b->un.i32.a;
	out->un.i32.b = a->un.i32.b == b->un.i32.b;
}
static void default_greaterThen_i32(MwLLVec* a, MwLLVec* b, MwLLVec* out) {
	out->un.i32.a = a->un.i32.a >= b->un.i32.a;
	out->un.i32.b = a->un.i32.b >= b->un.i32.b;
}
static MwLLMathVTable table_i32 = {
    .Add	 = default_add_i32,
    .Sub	 = default_sub_i32,
    .Multiply	 = default_multiply_i32,
    .Reciprocal	 = default_reciprocal_i32,
    .SquareRoot	 = default_squareRoot_i32,
    .ShiftRight	 = default_shiftRight_u32,
    .ShiftLeft	 = default_shiftLeft_u32,
    .Equal	 = default_equal_i32,
    .GreaterThen = default_greaterThen_i32,
};

static MwLLMathVTable* defaultMultiTable[_MwLLVecType_Max] = {
    &table_u8,	// _MwLLVecTypeU8x8
    &table_u16, // _MwLLVecTypeU16x4
    &table_u32, // _MwLLVecTypeU32x2
    &table_i8,	// _MwLLVecTypeI8x8
    &table_i16, // _MwLLVecTypeI16x4
    &table_i32, // _MwLLVecTypeI32x2
};

MwLLMathVTable** default_multi_table() {
	return defaultMultiTable;
}
