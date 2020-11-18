#ifndef __ABSTRACT_EFFECT_H__
#define __ABSTRACT_EFFECT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "fractional.h"
#include "nmmintrin.h" // for SSE4.2
#include "immintrin.h" // for AVX 

typedef union {
    __m128 v;
    flt f[4];
} xmm;

typedef struct stereo_s {
    flt left;
    flt right;
} stereo_t;

#endif