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
    float c = vec3_norm_sq(c_vec) - sphere->radius * sphere->radius;
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

static bool sphere_intersect_sphere(const sphere_t* a, const sphere_t* b)
{
    vec3_t diff;
    vec3_sub(a->center, b->center, diff);
    const float dist = vec3_norm(diff);
    return dist <= a->radius + b->radius;
}

static bool sphere_intersect_scene_object(const sphere_t* sphere, const scene_object_t* object)
{
    switch (object->type)
    {
        case OBJECT_SPHERE:
            return sphere_intersect_sphere(sphere, &object->underlying.sphere);
    }
}

static bool sphere_intersect_scene(const sphere_t* sphere, const scene_t* scene)
{
    for (size_t i = 0; i < scene->num_objects; i++)
    {
        if (sphere_intersect_scene_object(sphere, &scene->objects[i])) 
        {
            return true;
        }
    }
    return false;
}

static bool try_place_random_sphere_on_sphere(scene_t* self, const sphere_t* surface)
{
    const float radius = rand_float_in_range(0.5f, 4.0f);
    const float offset = surface->radius + radius;
    const float phi = acosf(rand_float_in_range(0.998f, 1.0f));
    const float theta = rand_float_in_range(0.0f, 2.0f * PI);
    const float x = offset * sinf(phi) * cosf(theta);
    const float z = offset * sinf(phi) * sinf(theta);
    const float y = offset * cosf(phi);

    sphere_t sphere;
    vec3_set(sphere.center, x, y, z);
    vec3_add(sphere.center, surface->center, sphere.center);
    sphere.radius = radius;

    if (sphere_intersect_scene(&sphere, self)) return false;

    enum material_type type = rand_int_in_range(0, MATERIAL_TYPE_COUNT - 1);
    material_t* mat;
    vec3_t random_color = {rand_unit_float(), rand_unit_float(), rand_unit_float()};
    vec3_sq(random_color, random_color);
    switch (type)
    {
        case MATERIAL_LAMBERTIAN:
            mat = material_lambertian_new(random_color);
            break;
        case MATERIAL_METAL:
            mat = material_metal_new(random_color, rand_unit_float());
            break;
        case MATERIAL_DIELECTRIC:
            mat = material_dielectric_new(rand_float_in_range(1.0f, 2.0f));
            break;
        case MATERIAL_POINT_LIGHT:
            mat = material_point_light_new((vec3_t){sqrtf(rand_unit_float()), sqrtf(rand_unit_float()), sqrtf(rand_unit_float())});
            break;
        default:
            mat = NULL;
            assert(false);
    }
    scene_add_sphere(self, mat, sphere.center, radius);
    return true;
}

void scene_object_sphere_init(scene_object_t* self, material_t* material, const vec3_t center, float radius)
{
    sphere_t* sphere = &self->underlying.sphere;
    self->type = OBJECT_SPHERE;
    self->material = material;
    vec3_copy(center, sphere->center);
    sphere->radius = radius;
}

void scene_default_init(scene_t* self)
{
    self->num_objects = 0;
    const vec3_t camera_pos = {-2.0f, 2.0f, 1.0f};
    vec3_t forward;
    vec3_sub((vec3_t){0.0, 0.0f, -1.0f}, camera_pos, forward);
    camera_init(&self->camera, camera_pos, TO_RADS(10.0f), 3.4f, 100.0f, (float) PIXEL_WIDTH / PIXEL_HEIGHT, TO_RADS(5.0f));
    camera_set_forward(&self->camera, forward);

    material_t* ground_mat = material_lambertian_new((vec3_t){0.8f, 0.8f, 0.0f});
    material_t* center_mat = material_lambertian_new((vec3_t){0.1f, 0.2f, 0.5f});
    material_t* left_mat = material_dielectric_new(1.5f);
    material_t* bubble_mat = material_dielectric_new(1.0f / 1.5f);
    material_t* right_mat = material_metal_new((vec3_t){0.8f, 0.6f, 0.2f}, 1.0f);

    scene_add_sphere(self, ground_mat, (vec3_t){0.0f, -100.5f, -1.0f}, 100.0f);
    scene_add_sphere(self, center_mat, (vec3_t){0.0f, 0.0f, -1.2f}, 0.5f);
    scene_add_sphere(self, left_mat, (vec3_t){-1.0f, 0.0f, -1.0f}, 0.5f);
    scene_add_sphere(self, bubble_mat, (vec3_t){-1.0f, 0.0f, -1.0f}, 0.4f);
    scene_add_sphere(self, right_mat, (vec3_t){1.0f, 0.0f, -1.0f}, 0.5f);
}

void scene_random_init(scene_t* self)
{
    self->num_objects = 0;
    const vec3_t ground_sphere_center = {0.0f, 0.0f, 0.0f};
    const float ground_sphere_radius = 1000.0f;
    material_t* ground_mat = material_lambertian_new((vec3_t){0.74f, 0.90f, 0.92f});
    const scene_object_t* ground = scene_add_sphere(self, ground_mat, ground_sphere_center, ground_sphere_radius);
   
    // Place camera at phi = 0
    vec3_t camera_pos;
    vec3_copy(ground_sphere_center, camera_pos);
    camera_pos[1] += ground_sphere_radius + 10.0f;
    camera_init(&self->camera, camera_pos, TO_RADS(45.0f), 20.0f, 100.0f, (float) PIXEL_WIDTH / PIXEL_HEIGHT, TO_RADS(0.0f));
    camera_set_forward(&self->camera, (vec3_t){0.0f, -0.2f, -1.0f});

    for (int i = 0; i < 100; i++)
    {
        while (!try_place_random_sphere_on_sphere(self, &ground->underlying.sphere));
    }
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

const scene_object_t* scene_add_sphere(scene_t* self, material_t* material, const vec3_t center, float radius)
{
    assert(self->num_objects < MAX_OBJECTS);
    scene_object_t* object = &self->objects[self->num_objects];
    scene_object_sphere_init(object, material, center, radius);
    self->num_objects++;
    return object;
}

bool ray_intersect_scene_object(const ray_t* ray, const scene_object_t* object, float tmin, float tmax, ray_hit_t* out)
{
    switch (object->type)
    {
        case OBJECT_SPHERE:
            return sphere_intersect_ray(object, ray, tmin, tmax, out);
    }
}

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out)
{
    float min_depth = FLT_MAX;
    bool success = false;

    for (size_t i = 0; i < scene->num_objects; i++)
    {
        ray_hit_t hit;
        const scene_object_t* object = &scene->objects[i];
        if (ray_intersect_scene_object(ray, object, tmin, tmax, &hit) && hit.t < min_depth)
        {
            min_depth = hit.t;
            success = true;
            ray_hit_copy(&hit, out);
            min_depth = hit.t;
        }
    }
    return success;
}

bool ray_intersect_aabb(const ray_t* ray, const aabb_t* aabb)
{

}
