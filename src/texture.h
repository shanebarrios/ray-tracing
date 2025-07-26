#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

enum texture_type
{
    TEXTURE_SOLID,
    TEXTURE_CHECKERED
};

struct solid_texture
{
    vec3_t color;
};

struct texture;

struct checkered_texture
{
    struct texture* textures[2];
    float width;
};

typedef struct texture
{
    enum texture_type type;
    union
    {
        struct solid_texture solid;
        struct checkered_texture checkered;
    } underlying;
    int ref_count;
} texture_t;

void texture_sample(const texture_t* self, float u, float v, const vec3_t pos, vec3_t out);

texture_t* texture_solid_new(const vec3_t color);

// Note: the inner textures should not have circular references to the containing texture!
texture_t* texture_checkered_new(texture_t* tex1, texture_t* tex2, float width);

texture_t* texture_checkered_solid_new(const vec3_t color1, const vec3_t color2, float width);

texture_t* texture_acquire(texture_t* other);

void texture_release(texture_t* self);

#endif