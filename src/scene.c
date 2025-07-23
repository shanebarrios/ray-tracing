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

 static void scene_object_sphere_aabb(const sphere_t* sphere, aabb_t* out)
{
    vec3_t diff;
    vec3_fill(diff, sphere->radius);
    vec3_sub(sphere->center, diff, out->min);
    vec3_add(sphere->center, diff, out->max);
}

static void aabb_merge(const aabb_t* a1, const aabb_t* a2, aabb_t* out)
{
    vec3_min(a1->min, a2->min, out->min);
    vec3_max(a1->max, a2->max, out->max);
}

static void aabb_copy(const aabb_t* src, aabb_t* dst)
{
    memcpy(dst, src, sizeof(aabb_t));
}

enum axis aabb_largest_axis(const aabb_t* a)
{
    vec3_t axis_lengths;
    vec3_sub(a->max, a->min, axis_lengths);
    if (axis_lengths[0] >= axis_lengths[1])
    {
        if (axis_lengths[0] >= axis_lengths[2]) return AXIS_X;
        else return AXIS_Z;
    }
    else
    {
        if (axis_lengths[1] >= axis_lengths[2]) return AXIS_Y;
        else return AXIS_Z;
    }
}

static void scene_object_sphere_init(scene_object_t* self, material_t* material, const vec3_t center, float radius)
{
    sphere_t* sphere = &self->underlying.sphere;
    self->type = OBJECT_SPHERE;
    self->material = material;
    vec3_copy(center, sphere->center);
    sphere->radius = radius;
    scene_object_sphere_aabb(sphere, &self->aabb);    
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
        default:
            assert(false);
            return false;
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

static const scene_object_t* scene_add_sphere(scene_t* self, material_t* material, const vec3_t center, float radius)
{
    assert(self->num_objects < MAX_OBJECTS);
    scene_object_t* object = &self->objects[self->num_objects];
    scene_object_sphere_init(object, material, center, radius);
    self->num_objects++;
    return object;
}

static bool try_place_random_sphere_on_sphere(scene_t* self, const sphere_t* surface)
{
    const float radius = rand_unit_float() * rand_unit_float() * 5.0f + 5.0f;
    const float offset = surface->radius + radius;
    const float phi = acosf(rand_float_in_range(0.995f, 1.0f));
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

static bool ray_intersect_aabb(const ray_t* ray, const aabb_t* aabb, float tmin, float tmax)
{ 
    vec3_t reciprocal_dir;
    vec3_reciprocal(ray->dir, reciprocal_dir);
    const float tx1 = (aabb->min[0] - ray->begin[0]) * reciprocal_dir[0];
    const float tx2 = (aabb->max[0] - ray->begin[0]) * reciprocal_dir[0];

    tmin = fmaxf(tmin, fminf(tx1, tx2));
    tmax = fminf(tmax, fmaxf(tx1, tx2));

    const float ty1 = (aabb->min[1] - ray->begin[1]) * reciprocal_dir[1];
    const float ty2 = (aabb->max[1] - ray->begin[1]) * reciprocal_dir[1];

    tmin = fmaxf(fminf(ty1, ty2), tmin);
    tmax = fminf(fmaxf(ty1, ty2), tmax);

    const float tz1 = (aabb->min[2] - ray->begin[2]) * reciprocal_dir[2];
    const float tz2 = (aabb->max[2] - ray->begin[2]) * reciprocal_dir[2];

    tmin = fmaxf(fminf(tz1, tz2), tmin);
    tmax = fminf(fmaxf(tz1, tz2), tmax);

    return tmin <= tmax;
}

static bool ray_intersect_scene_object(const ray_t* ray, const scene_object_t* object, float tmin, float tmax, ray_hit_t* out)
{
    switch (object->type)
    {
        case OBJECT_SPHERE:
            return sphere_intersect_ray(object, ray, tmin, tmax, out);
    }
}

static bool ray_intersect_bvh_node(const scene_t* scene, const ray_t* ray, uint16_t node_index, float tmin, float tmax, ray_hit_t* out)
{
    const bvh_node_t* node = &scene->bvh_nodes[node_index];
    if (!ray_intersect_aabb(ray, &node->aabb, tmin, tmax)) return false;

    if (node->is_leaf)
    {
        return ray_intersect_scene_object(ray, &scene->objects[node->underlying.leaf.index], tmin, tmax, out);
    }
    const uint16_t left_index = node->underlying.children.left;
    const uint16_t right_index = node->underlying.children.right;
    const bool hit_left = ray_intersect_bvh_node(scene, ray, left_index, tmin, tmax, out);
    const bool hit_right = ray_intersect_bvh_node(scene, ray, right_index, tmin, hit_left ? out->t : tmax, out);
    return hit_left || hit_right;
}

static void scene_object_destroy(scene_object_t* self)
{
    material_release(self->material);
}

static int box_x_compare(const void* a, const void* b)
{
    const float v1 = ((scene_object_t*)a)->aabb.min[0];
    const float v2 = ((scene_object_t*)b)->aabb.min[0];
    if (v1 < v2) return -1;
    if (v2 > v1) return 1;
    return 0; 
}

static int box_y_compare(const void* a, const void* b)
{
    const float v1 = ((scene_object_t*)a)->aabb.min[1];
    const float v2 = ((scene_object_t*)b)->aabb.min[1];
    if (v1 < v2) return -1;
    if (v2 > v1) return 1;
    return 0;
}

static int box_z_compare(const void* a, const void* b)
{
    const float v1 = ((scene_object_t*)a)->aabb.min[2];
    const float v2 = ((scene_object_t*)b)->aabb.min[2];
    if (v1 < v2) return -1;
    if (v2 > v1) return 1;
    return 0;
}

static uint16_t scene_build_bvh_node(scene_t* self, uint16_t start, uint16_t end)
{
    typedef int (*comparator)(const void*, const void*);

    static const comparator comparators[3] = {box_x_compare, box_y_compare, box_z_compare};

    const uint16_t node_index = self->num_nodes++;
    bvh_node_t* node = &self->bvh_nodes[node_index];

    if (start == end)
    {
        node->is_leaf = true;
        node->underlying.leaf.index = start;
        const scene_object_t* object = &self->objects[start];
        aabb_copy(&object->aabb, &node->aabb);
        return node_index;
    }

    aabb_t aabb;
    aabb_copy(&self->objects[start].aabb, &aabb);
    for (uint16_t i = start + 1; i <= end; i++)
    {
        aabb_merge(&aabb, &self->objects[i].aabb, &aabb);
    }
    const comparator compare_func = comparators[aabb_largest_axis(&aabb)];
    qsort(&self->objects[start], end-start+1, sizeof(scene_object_t), compare_func);
    const uint16_t mid = (start + end) / 2;
    const uint16_t left = scene_build_bvh_node(self, start, mid);
    const uint16_t right = scene_build_bvh_node(self, mid+1, end);
    node->is_leaf = false;
    node->underlying.children.left = left;
    node->underlying.children.right = right;
    aabb_merge(&self->bvh_nodes[left].aabb, &self->bvh_nodes[right].aabb, &node->aabb);
    return node_index;
}

static void scene_base_init(scene_t* self)
{
    self->num_objects = 0;
    self->num_nodes = 0;
}

void scene_default_init(scene_t* self)
{
    scene_base_init(self);
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
    scene_build_bvh_node(self, 0, self->num_objects-1);
}

void scene_random_init(scene_t* self)
{
    scene_base_init(self);
    const vec3_t ground_sphere_center = {0.0f, 0.0f, 0.0f};
    const float ground_sphere_radius = 1000.0f;
    material_t* ground_mat = material_lambertian_new((vec3_t){0.74f, 0.90f, 0.92f});
    const scene_object_t* ground = scene_add_sphere(self, ground_mat, ground_sphere_center, ground_sphere_radius);
   
    // Place camera at phi = 0
    vec3_t camera_pos;
    vec3_copy(ground_sphere_center, camera_pos);
    camera_pos[1] += ground_sphere_radius + 20.0f;
    camera_init(&self->camera, camera_pos, TO_RADS(45.0f), 20.0f, 100.0f, (float) PIXEL_WIDTH / PIXEL_HEIGHT, TO_RADS(0.0f));
    camera_set_forward(&self->camera, (vec3_t){0.0f, -0.2f, -1.0f});

    // "Sun"
    //material_t* sun_mat = material_point_light_new((vec3_t){1.0f, 0.95f, 0.9f});
    //scene_add_sphere(self, sun_mat, (vec3_t){0.0f, 20000.0f, -20000.0f}, 10000.0f);

    for (int i = 0; i < 100; i++)
    {
        while (!try_place_random_sphere_on_sphere(self, &ground->underlying.sphere));
    }
    scene_build_bvh_node(self, 0, self->num_objects-1);
}

void scene_destroy(scene_t* self)
{
    for (size_t i = 0; i < self->num_objects; i++)
    {
        scene_object_destroy(&self->objects[i]);
    }
}

bool ray_intersect_scene(const ray_t* ray, const scene_t* scene, float tmin, float tmax, ray_hit_t* out)
{
    return ray_intersect_bvh_node(scene, ray, 0, tmin, tmax, out);
}