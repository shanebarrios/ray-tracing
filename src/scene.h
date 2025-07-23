#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "common.h"

#define MAX_OBJECTS 16384

struct ray;
struct ray_hit;
struct material;
typedef struct ray ray_t;
typedef struct ray_hit ray_hit_t;
typedef struct material material_t;

enum scene_object_type
{
    OBJECT_SPHERE
};

typedef struct sphere
{
    vec3_t center;
    float radius;
} sphere_t;

typedef struct aabb
{
    vec3_t min;
    vec3_t max;
} aabb_t;

typedef struct scene_object
{
    union
    {
        sphere_t sphere;
    } underlying;
    aabb_t aabb;
    material_t* material;
    enum scene_object_type type;
} scene_object_t;


struct bvh_children
{
    uint16_t left;
    uint16_t right;
};

struct bvh_leaf
{
    uint16_t index;
};

typedef struct bvh_node
{
    aabb_t aabb;
    union
    {
        struct bvh_children children;
        struct bvh_leaf leaf;
    } underlying;
    bool is_leaf;
} bvh_node_t;

typedef struct scene
{
    scene_object_t objects[MAX_OBJECTS];
    bvh_node_t bvh_nodes[2 * MAX_OBJECTS];
    size_t num_objects;
    size_t num_nodes;
    camera_t camera;
} scene_t;

void scene_default_init(scene_t* self);

void scene_random_init(scene_t* self);

void scene_destroy(scene_t* self);

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out);

#endif