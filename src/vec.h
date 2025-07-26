#ifndef VEC_H
#define VEC_H

#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static inline void vec3_set(vec3_t v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

static inline void vec3_fill(vec3_t v, float x)
{
    v[0] = x;
    v[1] = x;
    v[2] = x;
}

static inline void vec3_zero(vec3_t v)
{
    memset(v, 0, sizeof(vec3_t));
}

static inline void vec3_copy(const vec3_t src, vec3_t dst)
{
    memcpy(dst, src, sizeof(vec3_t));
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

static inline void vec3_element_div(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] / v2[0];
    out[1] = v1[1] / v2[1];
    out[2] = v1[2] / v2[2];
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

static inline float vec3_norm_sq(const vec3_t v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

static inline float vec3_norm(const vec3_t v)
{
    return sqrtf(vec3_norm_sq(v));
}

static inline void vec3_normalize(const vec3_t v, vec3_t out)
{
    const float norm = vec3_norm(v);
    out[0] = v[0] / norm;
    out[1] = v[1] / norm;
    out[2] = v[2] / norm;
}

static inline void vec3_sq(const vec3_t v, vec3_t out)
{
    out[0] = v[0] * v[0];
    out[1] = v[1] * v[1];
    out[2] = v[2] * v[2];
}

static inline void vec3_sqrt(const vec3_t v, vec3_t out)
{
    out[0] = sqrtf(v[0]);
    out[1] = sqrtf(v[1]);
    out[2] = sqrtf(v[2]);
}

static inline void vec3_floor(const vec3_t v, vec3_t out)
{
    out[0] = floorf(v[0]);
    out[1] = floorf(v[1]);
    out[2] = floorf(v[2]);
}

static inline void vec3_min(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = fminf(v1[0], v2[0]);
    out[1] = fminf(v1[1], v2[1]);
    out[2] = fminf(v1[2], v2[2]);
}

static inline void vec3_max(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = fmaxf(v1[0], v2[0]);
    out[1] = fmaxf(v1[1], v2[1]);
    out[2] = fmaxf(v1[2], v2[2]);
}

static inline void vec3_reciprocal(const vec3_t v, vec3_t out)
{
    out[0] = 1.0f / v[0];
    out[1] = 1.0f / v[1];
    out[2] = 1.0f / v[2];
}

static inline void vec3_reflect(const vec3_t v, const vec3_t n, vec3_t out)
{
    const float k = -2 * vec3_dot(v, n);
    vec3_mult(n, k, out);
    vec3_add(out, v, out);
}

static inline void vec3_refract(const vec3_t incident, const vec3_t normal, float eta, vec3_t out)
{
    const float cos_theta = -vec3_dot(normal, incident);

    vec3_t tangential;
    vec3_mult(normal, cos_theta, tangential);
    vec3_add(tangential, incident, tangential);
    vec3_mult(tangential, eta, tangential);

    vec3_t parallel;
    const float k = sqrtf(1.0f - eta * eta * (1.0f - cos_theta * cos_theta));
    vec3_mult(normal, -k, parallel);
    vec3_add(tangential, parallel, out);
}

static inline void vec3_random_unit(vec3_t out)
{
    const float theta = rand_float_in_range(0, 2 * PI);
    const float z = rand_unit_float_signed();
    const float sin_phi = sqrtf(1-z*z);
    out[0] = sin_phi * cosf(theta);
    out[1] = sin_phi * sinf(theta);
    out[2] = z;
}

static inline void vec3_random_on_unit_hemisphere(const vec3_t normal, vec3_t out)
{
    vec3_random_unit(out);
    if (vec3_dot(normal, out) < 0.0f)
    {
        vec3_negate(out, out);
    }
}

static inline bool vec3_is_near_zero(const vec3_t v)
{
    return 
        fabsf(v[0]) < EPSILON && 
        fabsf(v[1]) < EPSILON &&
        fabsf(v[2]) < EPSILON;
}

static inline void vec3_print(vec3_t v)
{
    printf("(%f, %f, %f)\n", v[0], v[1], v[2]);
}

#endif
