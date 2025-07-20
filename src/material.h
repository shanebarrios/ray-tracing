#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

struct ray;
struct ray_hit;
typedef struct ray ray_t;
typedef struct ray_hit ray_hit_t;

typedef struct lambertian
{
    vec3_t albedo;
} lambertian_t;

typedef struct metal
{
    vec3_t albedo;
    float fuzz;
} metal_t;

typedef struct dielectric
{
    float refraction_index;
} dielectric_t;

typedef struct material
{
    union 
    {
        lambertian_t lambertian;
        metal_t metal;
        dielectric_t dielectric;
    } underlying;
    int ref_count;
    bool (*scatter)(const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenuation);
} material_t;

material_t* material_lambertian_new(const vec3_t albedo);

material_t* material_metal_new(const vec3_t albedo, float fuzz);

material_t* material_dielectric_new(float refraction_index);

void material_release(material_t* self);

material_t* material_acquire(material_t* other);

#endif