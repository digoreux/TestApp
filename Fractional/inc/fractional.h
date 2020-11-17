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

typedef int32_t q31;   // Q1.31
typedef int32_t q32;   // various Q format
typedef int64_t q63;   // Q1.63
typedef int64_t q64;   // various Q format
typedef float flt;

extern const q31 pow2lookup[512];
extern const q31 log2lookup[512];


q63 saturate(q63 r);

q31 neg_q31(q31 x);
q31 abs_q31(q31 x);

q31 left_shift_q31(q31 x, q31 n);
q31 right_shift_q31(q31 x, q31 n);

q63 left_shift_q63(q63 x, q31 n);
q63 right_shift_q63(q63 x, q31 n);

q31 add_q31(q31 x, q31 y);
q63 add_q63(q63 x, q63 y);

q31 sub_q31(q31 x, q31 y);
q63 sub_q63(q63 x, q63 y);


q31 gethigh(q63 x);
q31 getlow(q63 x);

q31 mul_q31(q31 x, q31 y);
q63 mul_q63(q31 x, q31 y);
/*
  input:  Q31
  output: Q63
*/
q63 mac_q63(q31 x, q31 y, q63 z);
/*
  input:  Q31
  output: Q31
*/
q31 mac_q31(q31 x, q31 y, q31 z);
/*
  input:  Q31
  output: Q63
*/
q63 msub_q63(q31 x, q31 y, q63 z);
/*
  input:  Q31
  output: Q31
*/
q31 msub_q31(q31 x, q31 y, q31 z);

/*
  input:  Q31
  output: Q31
*/
q31 div_q31(q31 x, q31 y);
 
/*************************
  pow2
  input:  Q26 [-32; 0]
  output: Q31 [0; 1]
*************************/
q31 pow2_q31(q31 x);
/*
  log2
  input:  Q31 [0; 1]
  output: Q26 [-32; 0]
*/
q31 log2_q31(q31 x);
/*
  pow
  input:  Q31
  output: Q31
*/
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

// log2 = 1;  pow2 = 2
void gen_table(int type);   
// log2 = 1;  pow2 = 2
void test_table(int type);   
void test_fractional(void);
void bin(unsigned n);

q31 float2fixed_q31(flt x);
flt fixed2float_q31(q31 x);

q31 double2fixed(double x);
double fixed2double(q31 x);

q31 double2fixed_q(double x, size_t Q);
double fixed2double_q(q31 x, size_t Q);

q32 float2fixed_q(flt x, size_t Q);
flt fixed2float_q(q31 x, size_t Q);

#endif // !__FRACTIONAL_H__
