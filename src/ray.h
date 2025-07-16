#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include "geometry.h"

typedef struct ray
{
    vec3_t begin;
    vec3_t dir;
} ray_t;

typedef struct ray_hit
{
    float t;
} ray_hit_t;

bool ray_intersect_sphere(const ray_t* ray, const sphere_t* sphere, ray_hit_t* hit);

#endif