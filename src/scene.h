#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vec.h"
#include "camera.h"

#define MAX_SPHERES 1024

typedef struct sphere
{
    vec3_t center;
    float radius;
} sphere_t;

typedef struct scene
{
    sphere_t spheres[MAX_SPHERES];
    size_t num_spheres;
    camera_t camera;
} scene_t;

void scene_init(scene_t* self);

void scene_add_sphere(scene_t* self, const vec3_t center, float radius);

#endif