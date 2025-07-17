#include "scene.h"

#include "utils.h"
#include <assert.h>

void scene_add_sphere(scene_t* self, const vec3_t center, float radius)
{
    assert(self->num_spheres < MAX_SPHERES);
    sphere_t* sphere = &self->spheres[self->num_spheres];
    vec3_copy(center, sphere->center);
    sphere->radius = radius;
    self->num_spheres++;
}

void scene_init(scene_t* self)
{
    self->num_spheres = 0;
    const vec3_t camera_pos = {0.0f, 0.0f, 0.0f};
    camera_init(&self->camera, camera_pos, TO_RADS(45.0f), 0.1f, 100.0f, (float) PIXEL_WIDTH / PIXEL_HEIGHT);

    const vec3_t c1 = {-0.0f, 0.0f, -1.0f};
    const vec3_t c2 = {0.0f, -100.5f, -1.0f};
    scene_add_sphere(self, c1, 0.5f);
    scene_add_sphere(self, c2, 100.0f);
}