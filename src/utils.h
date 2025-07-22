#ifndef UTILS_H
#define UTILS_H

#include "common.h"

#define PI 3.14159265f

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define CLAMP(val, lower, upper) (MIN((upper), MAX((val), (lower))))

#define TO_RADS(degrees) ((degrees) * PI / 180.0f)

#define TO_DEGREES(radians) ((radians) * 180.0f / PI)

inline float rand_unit_float()
{
    return (float) rand() / (float) RAND_MAX;
}

inline float rand_unit_float_signed()
{
    return (float) rand() / (float) RAND_MAX * 2.0f - 1.0f;
}

inline float rand_float_in_range(float lower, float upper)
{
    return (float) rand() / (float) RAND_MAX * (upper - lower) + lower;
}

inline int rand_int_in_range(int lower, int upper)
{
    return lower + rand() % (upper - lower + 1);
}

bool write_pixels_to_bmp(const vec3_t* pixels, size_t width, size_t height, const char* path);

#endif