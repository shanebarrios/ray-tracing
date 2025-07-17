#ifndef UTILS_H
#define UTILS_H

#include "common.h"

#define PI 3.14159265f

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define CLAMP(val, lower, upper) (MIN((upper), MAX((val), (lower))))

#define TO_RADS(degs) ((degs) * PI / 180.0f) 

#define TO_DEGS(rads) ((rads) * 180.0f / PI)

#define RAND_UNIT_FLOAT() ((float) rand() / RAND_MAX * 2.0f - 1.0f)

bool write_pixels_to_bmp(const vec3_t* pixels, size_t width, size_t height, const char* path);

#endif