#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include "pcg_basic.h"
#include <math.h>

#define PI 3.14159265f

#define CLAMP(val, lower, upper) (fminf((upper), fmaxf((val), (lower))))

#define TO_RADS(degrees) ((degrees) * PI / 180.0f)

#define TO_DEGREES(radians) ((radians) * 180.0f / PI)

enum axis
{
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z 
};

static inline bool interval_overlaps(float u1, float u2, float v1, float v2)
{
    return fmaxf(u1, v1) <= fminf(u2, v2);
}

static inline float rand_unit_float()
{
    return (float) pcg32_random() / (float) UINT32_MAX;
}

static inline float rand_unit_float_signed()
{
    return (float) pcg32_random() / (float) UINT32_MAX * 2.0f - 1.0f;
}

static inline float rand_float_in_range(float lower, float upper)
{
    return (float) pcg32_random() / (float) UINT32_MAX * (upper - lower) + lower;
}

static inline int rand_int_in_range(int lower, int upper)
{
    return lower + pcg32_random() % (upper - lower + 1);
}

bool write_pixels_to_bmp(const vec3_t* pixels, size_t width, size_t height, const char* path);

#endif