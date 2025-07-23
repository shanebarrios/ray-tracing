#ifndef VEC_H
#define VEC_H

#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

inline void vec3_set(vec3_t v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

inline void vec3_fill(vec3_t v, float x)
{
    v[0] = x;
    v[1] = x;
    v[2] = x;
}

inline void vec3_zero(vec3_t v)
{
    memset(v, 0, sizeof(vec3_t));
}

inline void vec3_copy(const vec3_t src, vec3_t dst)
{
    memcpy(dst, src, sizeof(vec3_t));
}

inline void vec3_negate(const vec3_t v, vec3_t out)
{
    out[0] = -v[0];
    out[1] = -v[1];
    out[2] = -v[2];
}

inline void vec3_add(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
    out[2] = v1[2] + v2[2];
}

inline void vec3_sub(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
    out[2] = v1[2] - v2[2];
}

inline void vec3_mult(const vec3_t v1, float k, vec3_t out)
{
    out[0] = v1[0] * k;
    out[1] = v1[1] * k;
    out[2] = v1[2] * k;
}

inline void vec3_div(const vec3_t v1, float k, vec3_t out)
{
    out[0] = v1[0] / k;
    out[1] = v1[1] / k;
    out[2] = v1[2] / k;
}

inline void vec3_element_mult(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] * v2[0];
    out[1] = v1[1] * v2[1];
    out[2] = v1[2] * v2[2];
}

inline void vec3_element_div(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[0] / v2[0];
    out[1] = v1[1] / v2[1];
    out[2] = v1[2] / v2[2];
}

inline float vec3_dot(const vec3_t v1, const vec3_t v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline void vec3_cross(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

inline float vec3_norm_sq(const vec3_t v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

inline float vec3_norm(const vec3_t v)
{
    return sqrtf(vec3_norm_sq(v));
}

inline void vec3_normalize(const vec3_t v, vec3_t out)
{
    const float norm = vec3_norm(v);
    out[0] = v[0] / norm;
    out[1] = v[1] / norm;
    out[2] = v[2] / norm;
}

inline void vec3_sq(const vec3_t v, vec3_t out)
{
    out[0] = v[0] * v[0];
    out[1] = v[1] * v[1];
    out[2] = v[2] * v[2];
}

inline void vec3_sqrt(const vec3_t v, vec3_t out)
{
    out[0] = sqrtf(v[0]);
    out[1] = sqrtf(v[1]);
    out[2] = sqrtf(v[2]);
}

inline void vec3_min(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = fminf(v1[0], v2[0]);
    out[1] = fminf(v1[1], v2[1]);
    out[2] = fminf(v1[2], v2[2]);
}

inline void vec3_max(const vec3_t v1, const vec3_t v2, vec3_t out)
{
    out[0] = fmaxf(v1[0], v2[0]);
    out[1] = fmaxf(v1[1], v2[1]);
    out[2] = fmaxf(v1[2], v2[2]);
}

inline void vec3_reciprocal(const vec3_t v, vec3_t out)
{
    out[0] = 1.0f / v[0];
    out[1] = 1.0f / v[1];
    out[2] = 1.0f / v[2];
}

inline void vec3_reflect(const vec3_t v, const vec3_t n, vec3_t out)
{
    const float k = -2 * vec3_dot(v, n);
    vec3_mult(n, k, out);
    vec3_add(out, v, out);
}

inline void vec3_refract(const vec3_t incident, const vec3_t normal, float eta, vec3_t out)
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

inline void vec3_random_unit(vec3_t out)
{
    while (true)
    {
        out[0] = rand_unit_float_signed();
        out[1] = rand_unit_float_signed();
        out[2] = rand_unit_float_signed();
        const float norm = vec3_norm(out);
        if (norm > 1.0f || norm < EPSILON) continue;

        vec3_div(out, norm, out);
        break;
    }
}

inline void vec3_random_on_unit_hemisphere(const vec3_t normal, vec3_t out)
{
    vec3_random_unit(out);
    if (vec3_dot(normal, out) < 0.0f)
    {
        vec3_negate(out, out);
    }
}

inline bool vec3_is_near_zero(const vec3_t v)
{
    return 
        fabsf(v[0]) < EPSILON && 
        fabsf(v[1]) < EPSILON &&
        fabsf(v[2]) < EPSILON;
}

inline void vec3_print(vec3_t v)
{
    printf("(%f, %f, %f)\n", v[0], v[1], v[2]);
}

#endif
