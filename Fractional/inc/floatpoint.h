#ifndef __FLOATPOINT_H__
#define __FLOATPOINT_H__

#include "fractional.h"
#define NCH 4
#define KF1 2.823529
#define KF2 1.882353
#define K0  0.0759776172978545212494579946726
#define INTR 0

typedef struct vector_s {
  #if INTR
  __m128 val;
  #else
  flt val[NCH];
  #endif
} vector_t;

extern flt negf(flt x);
extern flt absf(flt x);
extern flt addf(flt x, flt y);
extern flt subf(flt x, flt y);
extern flt mulf(flt x, flt y);
extern flt macf(flt x, flt y, flt z);
extern flt msubf(flt x, flt y, flt z);
extern flt divf(flt x, flt y);
extern flt divnr(flt x, flt y);
extern flt pow2(flt n);
extern flt flog2(flt x);
extern flt fpow(flt x, flt y);

extern vector_t fma2(vector_t x, vector_t y, vector_t z);
extern vector_t mul2(vector_t x, vector_t y);
extern uint32_t set_vals(vector_t * r, flt x, flt y);
extern uint32_t get_val(flt * x, vector_t y);
extern uint32_t get_vals(flt * x, vector_t y);
extern uint32_t get_vals2(flt * r, vector_t x);
extern uint32_t set_val(vector_t * r, flt x);

#endif
