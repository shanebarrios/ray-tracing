#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include "scene.h"

struct material;
typedef struct material material_t;

typedef struct ray
{
    vec3_t begin;
    vec3_t dir;
} ray_t;

typedef struct ray_hit
{
    vec3_t position;
    vec3_t normal;
    const material_t* material;
    float t;
    bool front_face;
} ray_hit_t;

static inline void ray_hit_copy(const ray_hit_t* src, ray_hit_t* dst)
{
    memcpy(dst, src, sizeof(ray_hit_t));
}

static inline void ray_copy(const ray_t* src, ray_t* dst)
{
    memcpy(dst, src, sizeof(ray_t));
}

#endif