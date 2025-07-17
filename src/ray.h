#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include "scene.h"

typedef struct ray
{
    vec3_t begin;
    vec3_t dir;
} ray_t;

typedef struct ray_hit
{
    float t;
    vec3_t position;
    vec3_t normal;
} ray_hit_t;

bool ray_intersect_scene(const ray_t* self, const scene_t* scene, ray_hit_t* out);

bool ray_intersect_sphere(const ray_t* self, const sphere_t* sphere, ray_hit_t* out);

static inline void ray_hit_copy(const ray_hit_t* src, ray_hit_t* dst)
{
    memcpy(dst, src, sizeof(ray_hit_t));
}

static inline void ray_copy(const ray_t* src, ray_t* dst)
{
    memcpy(dst, src, sizeof(ray_t));
}

#endif