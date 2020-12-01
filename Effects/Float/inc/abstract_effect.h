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
#include "floatpoint.h"
#include "nmmintrin.h" // for SSE4.2
#include "immintrin.h" // for AVX 

#define M_PI  3.14159265358979323846
#define M_e    2.71828182845904523536

#define FRAME_COUNT 32

typedef struct stereo_s {
  flt left;
  flt right;
} stereo_t;

typedef enum {
	LP   = 0,
	HP   = 1,
	PEAK = 2,
	LSH  = 3,
	HSH	 = 4,
    OFF  = 5
} filter_types;

#endif