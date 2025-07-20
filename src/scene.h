#ifndef SCENE_H
#define SCENE_H

#include "vec.h"
#include "camera.h"
#include "common.h"

#define MAX_OBJECTS 1024

struct ray;
struct ray_hit;
struct material;
typedef struct ray ray_t;
typedef struct ray_hit ray_hit_t;
typedef struct material material_t;

typedef struct sphere
{
    vec3_t center;
    float radius;
} sphere_t;

typedef struct scene_object
{
    union
    {
        sphere_t sphere;
    } underlying;
    material_t* material;
    bool (*intersect_ray)(const struct scene_object* self, const ray_t* ray, float tmin, float tmax, ray_hit_t* out);
} scene_object_t;

typedef struct scene
{
    scene_object_t objects[MAX_OBJECTS];
    size_t num_objects;
    camera_t camera;
} scene_t;

void sphere_init(scene_object_t* self, material_t* material, const vec3_t center, float radius);

void scene_default_init(scene_t* self);

void scene_destroy(scene_t* self);

void scene_object_destroy(scene_object_t* self);

void scene_add_sphere(scene_t* self, material_t* material, const vec3_t center, float radius);

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out);

#endif