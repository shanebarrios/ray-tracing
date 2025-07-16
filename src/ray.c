#include "ray.h"

bool ray_intersect_sphere(const ray_t* ray, const sphere_t* sphere, ray_hit_t* hit)
{
    vec3_t c_vec;
    vec3_sub(&ray->begin, &sphere->center, &c_vec);
    float a = 1.0f;
    float b = 2.0f * vec3_dot(&ray->dir, &c_vec);
    float c = vec3_normsq(&c_vec) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);

    if (t1 >= 0.0f && t2 >= 0.0f)
    {
        hit->t = MIN(t1, t2);
    }
    else if (t1 >= 0.0f)
    {
        hit->t = t1;
    }
    else if (t2 >= 0.0f)
    {
        hit->t = t2;
    }
    else
    {
        return false;
    }

    return true;
}