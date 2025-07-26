#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define PIXEL_WIDTH 1920
#define PIXEL_HEIGHT 1080
#define EPSILON 1e-6f

#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!!(x), 1)

#define BACKFACE_CULL

typedef float vec3_t[3];

#endif
