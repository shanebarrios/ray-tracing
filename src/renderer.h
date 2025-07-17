#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"

struct scene;

void render(const struct scene* scene, vec3_t* pixels, size_t width, size_t height);

#endif