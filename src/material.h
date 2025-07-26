#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

struct ray;
struct ray_hit;
struct texture;
typedef struct ray ray_t;
typedef struct ray_hit ray_hit_t;
typedef struct texture texture_t;

enum material_type
{
    MATERIAL_LAMBERTIAN,
    MATERIAL_METAL,
    MATERIAL_DIELECTRIC,
    MATERIAL_POINT_LIGHT,
    MATERIAL_TYPE_COUNT
};

struct lambertian
{
    texture_t* tex;
};

struct metal
{
    vec3_t albedo;
    float fuzz;
};

struct dielectric
{
    float refraction_index;
};

struct point_light
{
    vec3_t color;
};

typedef struct material
{
    union 
    {
        struct lambertian lambertian;
        struct metal metal;
        struct dielectric dielectric;
        struct point_light point_light;
    } underlying;
    enum material_type type;
    int ref_count;
} material_t;

material_t* material_lambertian_new(texture_t* tex);

material_t* material_lambertian_solid_new(const vec3_t albedo);

material_t* material_metal_new(const vec3_t albedo, float fuzz);

material_t* material_dielectric_new(float refraction_index);

material_t* material_point_light_new(const vec3_t color);

void material_release(material_t* self);

material_t* material_acquire(material_t* other);

bool material_scatter(const material_t* self, const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenutation);

bool material_emit(const material_t* self, vec3_t out_color);

#endif