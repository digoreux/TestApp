#ifndef __FRACTIONAL_H__
#define __FRACTIONAL_H__

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef int32_t q31;
typedef int64_t q63;   // intermediate results
typedef float flt;

extern const q31 pow2lookup[512];
extern const q31 log2lookup[512];

q31 gethigh(q63 x);
q63 saturate(q63 r);
q31 saturate32(q31 x, q31 y);
q63 saturate64(q63 x, q63 y);

q31 neg_q31(q31 x);
q31 abs_q31(q31 x);
q31 add_q31(q31 x, q31 y);
q63 add_q63(q63 x, q63 y);
q31 sub_q31(q31 x, q31 y);
q63 sub_q63(q63 x, q63 y);
q63 mac_q31(q31 x, q31 y, q63 z);
q63 msub_q31(q31 x, q31 y, q63 z);
q31 mul_q31(q31 x, q31 y);
q63 mul_q63(q31 x, q31 y);
q31 div_q31(q31 x, q31 y);
q31 left_shift(q31 x, q31 n);
q31 right_shift(q31 x, q31 n);
q31 pow2_q31(q31 x);
q31 log2_q31(q31 x);
q31 pow_q31(q31 x, q31 y);


flt negf(flt x);
flt absf(flt x);
flt addf(flt x, flt y);
flt subf(flt x, flt y);
flt mulf(flt x, flt y);
flt macf(flt x, flt y, flt z);
flt msubf(flt x, flt y, flt z);
flt divf(flt x, flt y);
flt divnr(flt x, flt y);
flt pow2(flt n);
flt flog2(flt x);
flt fpow(flt x, flt y);


void gentable(int type);
void test_fractional(void);
void bin(unsigned n);

q31 float2fixed_q31(flt x);
flt fixed2float_q31(q31 x);

q31 double2fixed(double x);
double fixed2double(q31 x);

q31 double2fixed_q(double x, size_t Q);
double fixed2double_q(q31 x, size_t Q);

#endif // !__FRACTIONAL_H__
