#include "camera.h"

static const vec3_t WORLD_UP = {0.0f, 1.0f, 0.0f};

static void camera_update_bases(camera_t* self)
{
    vec3_normalize(self->forward, self->forward);
    vec3_cross(self->forward, WORLD_UP, self->right);
    vec3_normalize(self->right, self->right);
    vec3_cross(self->right, self->forward, self->up);
    vec3_normalize(self->up, self->up);
}

void camera_init(camera_t* self, const vec3_t position, float fov, float near, float far, float aspect)
{
    vec3_copy(position, self->position);
    vec3_init(self->forward, 0.0f, 0.0f, -1.0f);
    self->fov = fov;
    self->near = near;
    self->far = far;
    self->aspect = aspect;
    camera_update_bases(self);
}

void camera_set_forward(camera_t* self, const vec3_t forward)
{
    vec3_copy(forward, self->forward);
    camera_update_bases(self);
}

void camera_world_to_view(const camera_t* self, const vec3_t v, vec3_t out)
{
    vec3_t world_dir;
    vec3_sub(v, self->position, world_dir);
    out[0] = vec3_dot(world_dir, self->right);
    out[1] = vec3_dot(world_dir, self->up);
    out[2] = vec3_dot(world_dir, self->forward);
}

void camera_view_to_world(const camera_t* self, const vec3_t v, vec3_t out)
{
    vec3_t scratch;
    vec3_copy(self->position, out);
    vec3_mult(self->right, v[0], scratch);
    vec3_add(out, scratch, out);
    vec3_mult(self->up, v[1], scratch);
    vec3_add(out, scratch, out);
    vec3_mult(self->forward, v[2], scratch);
    vec3_add(out, scratch, out);
}
