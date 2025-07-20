#include "scene.h"

#include <assert.h>
#include <float.h>
#include "utils.h"
#include "ray.h"
#include "material.h"

static bool sphere_intersect_ray(const scene_object_t* self, const ray_t* ray, float tmin, float tmax, ray_hit_t* out)
{
    const sphere_t* sphere = &self->underlying.sphere;
    vec3_t c_vec;
    vec3_sub(ray->begin, sphere->center, c_vec);
    float a = 1.0f;
    float b = 2.0f * vec3_dot(ray->dir, c_vec);
    float c = vec3_normsq(c_vec) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);

    if (t1 >= tmin && t1 <= tmax)
    {
        out->t = t1;
    }
    else if (t2 >= tmin && t2 <= tmax)
    {
        out->t = t2;
    }
    else
    {
        return false;
    }

    vec3_mult(ray->dir, out->t, out->position);
    vec3_add(out->position, ray->begin, out->position);
    vec3_sub(out->position, sphere->center, out->normal);
    vec3_normalize(out->normal, out->normal);
    out->material = self->material;
    
    out->front_face = vec3_dot(out->normal, ray->dir) < 0.0f;
    if (!out->front_face)
    {
        vec3_negate(out->normal, out->normal);
    }

    return true;
}

void sphere_init(scene_object_t* self, material_t* material, const vec3_t center, float radius)
{
    sphere_t* sphere = &self->underlying.sphere;
    self->intersect_ray = sphere_intersect_ray;
    self->material = material;
    vec3_copy(center, sphere->center);
    sphere->radius = radius;
}

void scene_default_init(scene_t* self)
{
    self->num_objects = 0;
    const vec3_t camera_pos = {0.0f, 0.0f, 0.0f};
    camera_init(&self->camera, camera_pos, TO_RADS(45.0f), 0.1f, 100.0f, (float) PIXEL_WIDTH / PIXEL_HEIGHT);

    material_t* ground_mat = material_lambertian_new((vec3_t){0.8f, 0.8f, 0.0f});
    material_t* center_mat = material_lambertian_new((vec3_t){0.1f, 0.2f, 0.5f});
    //material_t* left_mat = material_metal_new((vec3_t){0.8f, 0.8f, 0.8f}, 0.3f);
    material_t* left_mat = material_dielectric_new(1.5f);
    material_t* right_mat = material_metal_new((vec3_t){0.8f, 0.6f, 0.2f}, 1.0f);

    scene_add_sphere(self, ground_mat, (vec3_t){0.0f, -100.5f, -1.0f}, 100.0f);
    scene_add_sphere(self, center_mat, (vec3_t){0.0f, 0.0f, -1.2f}, 0.5f);
    scene_add_sphere(self, left_mat, (vec3_t){-1.0f, 0.0f, -1.0f}, 0.5f);
    scene_add_sphere(self, right_mat, (vec3_t){1.0f, 0.0f, -1.0f}, 0.5f);
}

void scene_destroy(scene_t* self)
{
    for (size_t i = 0; i < self->num_objects; i++)
    {
        scene_object_destroy(&self->objects[i]);
    }
}

void scene_object_destroy(scene_object_t* self)
{
    material_release(self->material);
}

void scene_add_sphere(scene_t* self, material_t* material, const vec3_t center, float radius)
{
    assert(self->num_objects < MAX_OBJECTS);
    scene_object_t* object = &self->objects[self->num_objects];
    sphere_init(object, material, center, radius);
    self->num_objects++;
}

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out)
{
    float min_depth = FLT_MAX;
    bool success = false;

    for (size_t i = 0; i < scene->num_objects; i++)
    {
        ray_hit_t hit;
        const scene_object_t* object = &scene->objects[i];
        if (object->intersect_ray(object, ray, tmin, tmax, &hit) && hit.t < min_depth)
        {
            min_depth = hit.t;
            success = true;
            ray_hit_copy(&hit, out);
            min_depth = hit.t;
        }
    }
    return success;
}
