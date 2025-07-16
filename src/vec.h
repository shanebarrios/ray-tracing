#ifndef VEC_H
#define VEC_H

#include "common.h"
#include <stdio.h>

typedef struct
{
    union
    {
        struct {float x, y, z; };
        struct {float r, g, b; };
    };
} vec3_t;

static inline void vec3_add(const vec3_t* v1, const vec3_t* v2, vec3_t* out)
{
    *out = (vec3_t) {v1->x + v2->x, v1->y + v2->y, v1->z + v2->z};
}

static inline void vec3_sub(const vec3_t* v1, const vec3_t* v2, vec3_t* out)
{
    *out = (vec3_t) {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z};
}

static inline void vec3_mult(const vec3_t* v1, float k, vec3_t* out)
{
    *out = (vec3_t) {v1->x * k, v1->y * k, v1->z * k};
}

static inline void vec3_div(const vec3_t* v1, float k, vec3_t* out)
{
    *out = (vec3_t) {v1->x / k, v1->y / k, v1->z / k};
}

static inline void vec3_element_mult(const vec3_t* v1, const vec3_t* v2, vec3_t* out)
{
    *out = (vec3_t) {v1->x * v2->x, v1->y * v2->y, v1->z * v2->z};
}

static inline float vec3_dot(const vec3_t* v1, const vec3_t* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

static inline void vec3_cross(const vec3_t* v1, const vec3_t* v2, vec3_t* out)
{
    *out = (vec3_t)
    {
        v1->y * v2->z - v1->z * v2->y,
        v1->z * v2->x - v1->x * v2->z,
        v1->x * v2->y - v1->y * v2->x
    };
}

static inline float vec3_norm(const vec3_t* v1)
{
    return sqrtf(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
}

static inline float vec3_normsq(const vec3_t* v1)
{
    return v1->x * v1->x + v1->y * v1->y + v1->z * v1->z;
}

static inline void vec3_normalize(const vec3_t* v1, vec3_t* out)
{
    const float norm = vec3_norm(v1);
    *out = (vec3_t) {v1->x / norm, v1->y / norm, v1->z / norm};
}

static inline void vec3_print(vec3_t v1)
{
    printf("(%f, %f, %f)\n", v1.x, v1.y, v1.z);
}

#endif
