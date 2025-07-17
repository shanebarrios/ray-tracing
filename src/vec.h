#ifndef VEC_H
#define VEC_H

#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static inline void vec3_init(vec3_t v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

static inline void vec3_zero(vec3_t v)
{
    memset(v, 0, sizeof(vec3_t));
}

static inline void vec3_negate(const vec3_t v, vec3_t out)
{
    out[0] = -v[0];
    out[1] = -v[1];
    out[2] = -v[2];
}

static inline void vec3_add(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
    out[2] = v1[2] + v2[2];
}

static inline void vec3_sub(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
    out[2] = v1[2] - v2[2];
}

static inline void vec3_mult(const vec3_t v1, float k, vec3_t out)
{
    out[0] = v1[0] * k;
    out[1] = v1[1] * k;
    out[2] = v1[2] * k;
}

static inline void vec3_div(const vec3_t v1, float k, vec3_t out)
{
    out[0] = v1[0] / k;
    out[1] = v1[1] / k;
    out[2] = v1[2] / k;
}

static inline void vec3_element_mult(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] * v2[0];
    out[1] = v1[1] * v2[1];
    out[2] = v1[2] * v2[2];
}

static inline float vec3_dot(const vec3_t v1, const vec3_t v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

static inline void vec3_cross(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static inline float vec3_normsq(const vec3_t v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

static inline float vec3_norm(const vec3_t v)
{
    return sqrtf(vec3_normsq(v));
}

static inline void vec3_normalize(const vec3_t v, vec3_t out)
{
    float norm = vec3_norm(v);
    out[0] = v[0] / norm;
    out[1] = v[1] / norm;
    out[2] = v[2] / norm;
}

static inline void vec3_random_unit(vec3_t out)
{
    while (true)
    {
        out[0] = RAND_UNIT_FLOAT();
        out[1] = RAND_UNIT_FLOAT();
        out[2] = RAND_UNIT_FLOAT();
        float norm = vec3_norm(out);
        if (norm > 1.0f || norm < EPSILON) continue;

        vec3_div(out, norm, out);
        break;
    }
}

static inline void vec3_random_on_unit_hemisphere(const vec3_t normal, vec3_t out)
{
    vec3_random_unit(out);
    if (vec3_dot(normal, out) < 0.0f)
    {
        vec3_negate(out, out);
    }
}

static inline void vec3_copy(const vec3_t src, vec3_t dst)
{
    memcpy(dst, src, sizeof(vec3_t));
}

static inline void vec3_print(vec3_t v)
{
    printf("(%f, %f, %f)\n", v[0], v[1], v[2]);
}

#endif
