#include "ray.h"

#include <float.h>
#include "utils.h"

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, ray_hit_t* out)
{
    float min_depth = FLT_MAX;
    bool success = false;

    for (size_t i = 0; i < scene->num_spheres; i++)
    {
        ray_hit_t hit;
        if (ray_intersect_sphere(ray, &scene->spheres[i], &hit) && hit.t < min_depth)
        {
            min_depth = hit.t;
            success = true;
            ray_hit_copy(&hit, out);
            min_depth = hit.t;
        }
    }
    return success;
}

bool ray_intersect_sphere(const ray_t* ray, const sphere_t* sphere, ray_hit_t* out)
{
    vec3_t c_vec;
    vec3_sub(ray->begin, sphere->center, c_vec);
    float a = 1.0f;
    float b = 2.0f * vec3_dot(ray->dir, c_vec);
    float c = vec3_normsq(c_vec) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);

    if (t1 >= 0.0f && t2 >= 0.0f)
    {
        out->t = MIN(t1, t2);
    }
    else if (t1 >= 0.0f)
    {
        out->t = t1;
    }
    else if (t2 >= 0.0f)
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

    return true;
}