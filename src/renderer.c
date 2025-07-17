#include "renderer.h"

#include <float.h>
#include "scene.h"
#include "ray.h"

#define MAX_RAY_BOUNCES 10

static const vec3_t WHITE_COLOR = {1.0f, 1.0f, 1.0f};
static const vec3_t FILL_COLOR = {0.5f, 0.7f, 1.0f};

static void render_pixel(const struct scene* scene, const ray_t* ray, vec3_t pixel, int bounces)
{
    if (bounces >= MAX_RAY_BOUNCES)
    {
        vec3_zero(pixel);
        return;
    }

    ray_hit_t hit;
    if (ray_intersect_scene(ray, scene, &hit))
    {
        ray_t bounce_ray;
        // Offset slightly so it doesn't hit the same object again
        vec3_t epsilon_vec;
        vec3_copy(hit.normal, epsilon_vec);
        vec3_mult(epsilon_vec, 0.001f, epsilon_vec);

        vec3_copy(hit.position, bounce_ray.begin);
        vec3_add(bounce_ray.begin, epsilon_vec, bounce_ray.begin);
        vec3_random_unit(bounce_ray.dir);
        vec3_add(hit.normal, bounce_ray.dir, bounce_ray.dir);
        vec3_normalize(bounce_ray.dir, bounce_ray.dir);
        render_pixel(scene, &bounce_ray, pixel, bounces+1);
        vec3_mult(pixel, 0.5f, pixel);
    }
    else
    {
        float a = (ray->dir[1] + 1.0f) / 2.0f;

        vec3_copy(FILL_COLOR, pixel);
        vec3_mult(pixel, a, pixel);

        vec3_t scratch;
        vec3_mult(WHITE_COLOR, 1.0f - a, scratch);

        vec3_add(pixel, scratch, pixel);
    }
}

void render(const struct scene* scene, vec3_t* pixels, size_t width, size_t height)
{
    const camera_t* cam = &scene->camera;

    const float half_viewport_height = tanf(cam->fov) * cam->near;
    const float half_viewport_width = half_viewport_height * cam->aspect;

    vec3_t forward, right, up, dir;
    vec3_mult(cam->forward, cam->near, forward);

    ray_t ray;
    vec3_copy(cam->position, ray.begin);

    for (size_t j = 0; j < height; j++)
    {
        const float ndc_y = (j + 0.5f) / height * 2.0f - 1.0f;
        const float world_y = ndc_y * half_viewport_height;
        vec3_mult(cam->up, world_y, up);
        for (size_t i = 0; i < width; i++)
        {
            const float ndc_x = (i + 0.5f) / width * 2.0f - 1.0f;
            const float world_x = ndc_x * half_viewport_width;
            vec3_mult(cam->right, world_x, right);
        
            vec3_add(forward, up, dir);
            vec3_add(dir, right, dir);
            vec3_normalize(dir, dir);
            vec3_copy(dir, ray.dir);

            render_pixel(scene, &ray, pixels[j * width + i], 0);
        }
    }
}