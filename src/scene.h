#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "common.h"

#define MAX_OBJECTS 16384
#define MAX_NODES MAX_OBJECTS * 2
#if MAX_OBJECTS > 64
    #define USE_BVH
#endif

struct ray;
struct ray_hit;
struct material;
typedef struct ray ray_t;
typedef struct ray_hit ray_hit_t;
typedef struct material material_t;

enum scene_object_type
{
    OBJECT_SPHERE,
    OBJECT_QUAD,
    OBJECT_TRIANGLE
};

typedef struct sphere
{
    vec3_t center;
    float radius;
} sphere_t;

typedef struct quad
{
    vec3_t origin;
    vec3_t u;
    vec3_t v;
    vec3_t normal;
    vec3_t w;
} quad_t;

typedef struct triangle
{
    vec3_t v0;
    vec3_t v1;
    vec3_t v2;
} triangle_t;

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
        quad_t quad;
    } underlying;
#ifdef USE_BVH
    aabb_t aabb;
#endif
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
#ifdef USE_BVH
    bvh_node_t bvh_nodes[MAX_NODES];
    size_t num_nodes;
#endif
    size_t num_objects;
    camera_t camera;
} scene_t;

void scene_default_init(scene_t* self);

void scene_random_init(scene_t* self);

void scene_cornell_box_init(scene_t* self);

void scene_destroy(scene_t* self);

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out);

#endif