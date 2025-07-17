#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

typedef struct camera
{
    vec3_t position;
    vec3_t forward;
    vec3_t right;
    vec3_t up;

    float fov;
    float near;
    float far;
    float aspect;
} camera_t;

void camera_init(camera_t* self, const vec3_t position, float fov, float near, float far, float aspect);

void camera_set_forward(camera_t* self, const vec3_t forward);

void camera_world_to_view(const camera_t* self, const vec3_t v, vec3_t out);

void camera_view_to_world(const camera_t* self, const vec3_t v, vec3_t out);

#endif
