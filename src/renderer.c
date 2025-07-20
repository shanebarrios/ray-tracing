#include "renderer.h"

#include <float.h>
#include <pthread.h>
#include "scene.h"
#include "ray.h"
#include "material.h"

#define MAX_RAY_BOUNCES 10
#define NUM_SAMPLES 4
#define GAMMA_EXPONENT 2.2f
#define INV_GAMMA_EXPONENT 1.0f / 2.2f
#define NUM_THREADS 16

static const vec3_t WHITE_COLOR = {1.0f, 1.0f, 1.0f};
static const vec3_t FILL_COLOR = {0.5f, 0.7f, 1.0f};

static inline void linear_to_gamma(vec3_t color)
{
    color[0] = powf(color[0], INV_GAMMA_EXPONENT);
    color[1] = powf(color[1], INV_GAMMA_EXPONENT);
    color[2] = powf(color[2], INV_GAMMA_EXPONENT);
}

static void render_pixel(const struct scene* scene, const ray_t* ray, vec3_t pixel, int bounces)
{
    if (bounces >= MAX_RAY_BOUNCES)
    {
        vec3_zero(pixel);
        return;
    }

    ray_hit_t hit;
    if (ray_intersect_scene(ray, scene, 0.001, INFINITY, &hit))
    {
        ray_t bounce_ray;
        vec3_t attenuation;
        if (hit.material->scatter(ray, &hit, &bounce_ray, attenuation))
        {
            render_pixel(scene, &bounce_ray, pixel, bounces+1);
            vec3_element_mult(pixel, attenuation, pixel);       
        }
        else
        {
            vec3_zero(pixel);
        }
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

struct render_task_args
{
    const struct scene* scene;
    vec3_t* pixels;
    size_t row_start;
    size_t row_end;
    size_t width;
    size_t height;
};

static void* render_task(void* _args)
{
    struct render_task_args* args = (struct render_task_args*) _args;
    const camera_t* cam = &args->scene->camera;

    const float half_viewport_height = tanf(cam->fov) * cam->near;
    const float half_viewport_width = half_viewport_height * cam->aspect;

    vec3_t forward, right, up, dir;
    vec3_mult(cam->forward, cam->near, forward);

    ray_t ray;
    vec3_copy(cam->position, ray.begin);

    for (size_t j = args->row_start; j <= args->row_end; j++)
    {
        for (size_t i = 0; i < args->width; i++)
        {
            vec3_t* pixel = &args->pixels[j * args->width + i];
            vec3_zero(*pixel);

            for (size_t sample = 0; sample < NUM_SAMPLES; sample++)
            {
                const float ndc_x = (i + RAND_UNIT_FLOAT()) / args->width * 2.0f - 1.0f;
                const float world_x = ndc_x * half_viewport_width;
                const float ndc_y = (j + RAND_UNIT_FLOAT()) / args->height * 2.0f - 1.0f;
                const float world_y = ndc_y * half_viewport_height;
                vec3_mult(cam->up, world_y, up);
            
                vec3_mult(cam->right, world_x, right);
        
                vec3_add(forward, up, dir);
                vec3_add(dir, right, dir);
                vec3_normalize(dir, dir);
                vec3_copy(dir, ray.dir);

                vec3_t sample_color;
                render_pixel(args->scene, &ray, sample_color, 0);
                vec3_add(*pixel, sample_color, *pixel);
            }
            vec3_div(*pixel, NUM_SAMPLES, *pixel);
            linear_to_gamma(*pixel);
        }
    }

    return NULL;
}

void render(const struct scene* scene, vec3_t* pixels, size_t width, size_t height)
{
    pthread_t threads[NUM_THREADS];
    struct render_task_args args[NUM_THREADS];
    const size_t rows_per_thread = height / NUM_THREADS;

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        args[i].scene = scene;
        args[i].pixels = pixels;
        args[i].row_start = rows_per_thread * i;
        args[i].row_end = rows_per_thread * (i + 1) - 1;
        args[i].width = width;
        args[i].height = height;

        if (i == NUM_THREADS - 1)
        {
            args[i].row_end = height - 1;
        }
        pthread_create(&threads[i], NULL, render_task, &args[i]);
    }
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}