#include "material.h"

#include <assert.h>
#include "ray.h"

static float reflectance(float cos_theta, float refraction_index)
{
    float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
    r0 *= r0;
    return r0 + (1.0f - r0) * powf(1.0f - cos_theta, 5.0f);
}

static bool material_lambertian_scatter(const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenuation)
{
    vec3_t random_unit;
    vec3_random_unit(random_unit);
    vec3_add(hit->normal, random_unit, out_ray->dir);
    if (unlikely(vec3_is_near_zero(out_ray->dir)))
    {
        vec3_copy(hit->normal, out_ray->dir);
    }
    vec3_normalize(out_ray->dir, out_ray->dir);
    vec3_copy(hit->position, out_ray->begin);
    vec3_copy(hit->material->underlying.lambertian.albedo, out_attenuation);
    return true;
}

static bool material_metal_scatter(const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenuation)
{
    vec3_reflect(ray->dir, hit->normal, out_ray->dir);
    vec3_t random_offset;
    vec3_random_unit(random_offset);
    vec3_mult(random_offset, hit->material->underlying.metal.fuzz, random_offset);
    vec3_add(out_ray->dir, random_offset, out_ray->dir);
    vec3_normalize(out_ray->dir, out_ray->dir);

    vec3_copy(hit->position, out_ray->begin);
    vec3_copy(hit->material->underlying.metal.albedo, out_attenuation);
    return (vec3_dot(out_ray->dir, hit->normal) > 0.0f);
}

static bool material_dielectric_scatter(const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenuation)
{
    const float refraction_index = hit->material->underlying.dielectric.refraction_index;
    vec3_copy((vec3_t){1.0f, 1.0f, 1.0f}, out_attenuation);
    const float eta = hit->front_face ? 1.0f / refraction_index : refraction_index;
    const float cos_theta = -vec3_dot(hit->normal, ray->dir);
    const float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    if (eta * sin_theta > 1.0f || reflectance(cos_theta, refraction_index) > rand_unit_float())
    {
        vec3_reflect(ray->dir, hit->normal, out_ray->dir);
    }
    else
    {
        vec3_refract(ray->dir, hit->normal, eta, out_ray->dir);
    }
    vec3_copy(hit->position, out_ray->begin);
    return true;
}

material_t* material_lambertian_new(const vec3_t albedo)
{
    material_t* ret = (material_t*) malloc(sizeof(material_t));
    ret->type = MATERIAL_LAMBERTIAN;
    ret->ref_count = 1;
    vec3_copy(albedo, ret->underlying.lambertian.albedo);
    return ret;
}

material_t* material_metal_new(const vec3_t albedo, float fuzz)
{
    material_t* ret = (material_t*) malloc(sizeof(material_t));
    ret->type = MATERIAL_METAL;
    ret->ref_count = 1;
    vec3_copy(albedo, ret->underlying.metal.albedo);
    ret->underlying.metal.fuzz = fuzz;
    return ret;
}

material_t* material_dielectric_new(float refraction_index)
{
    material_t* ret = (material_t*) malloc(sizeof(material_t));
    ret->type = MATERIAL_DIELECTRIC;
    ret->ref_count = 1;
    ret->underlying.dielectric.refraction_index = refraction_index;
    return ret;
}

void material_release(material_t* mat)
{
    if (--mat->ref_count == 0)
    {
        free(mat);
    }
}

material_t* material_acquire(material_t* mat)
{
    mat->ref_count++;
    return mat;
}

bool material_scatter(const material_t* self, const ray_t* ray, const ray_hit_t* hit, ray_t* out_ray, vec3_t out_attenuation)
{
    switch (self->type)
    {
        case MATERIAL_LAMBERTIAN:
            return material_lambertian_scatter(ray, hit, out_ray, out_attenuation);
        case MATERIAL_METAL:
            return material_metal_scatter(ray, hit, out_ray, out_attenuation);
        case MATERIAL_DIELECTRIC:
            return material_dielectric_scatter(ray, hit, out_ray, out_attenuation);
        default:
            assert(false);
            return false;
    }
}