#include "texture.h"

#include <assert.h>
#include "vec.h"

static void texture_solid_sample(const texture_t* self, float u, float v, const vec3_t pos, vec3_t out)
{
    vec3_copy(self->underlying.solid.color, out);
}  

static void texture_checkered_sample(const texture_t* self, float u, float v, const vec3_t pos, vec3_t out)
{
    vec3_t vec;
    vec3_div(pos, self->underlying.checkered.width, vec);
    vec3_floor(vec, vec);
    size_t option = abs(((int) vec[0] + (int) vec[1] + (int) vec[2]) % 2);
    return texture_sample(self->underlying.checkered.textures[option], u, v, pos, out);
}   

static void texture_destroy(texture_t* self)
{
    switch (self->type)
    {
        case TEXTURE_CHECKERED:
            texture_release(self->underlying.checkered.textures[0]);
            texture_release(self->underlying.checkered.textures[1]);
            break;
        default:
            ;
    }
    free(self);
}

void texture_sample(const texture_t* self, float u, float v, const vec3_t pos, vec3_t out)
{
    switch (self->type)
    {
        case TEXTURE_SOLID:
            return texture_solid_sample(self, u, v, pos, out);
        case TEXTURE_CHECKERED:
            return texture_checkered_sample(self, u, v, pos, out);
        default:
            assert(false);
            vec3_zero(out);
    }
}

texture_t* texture_solid_new(const vec3_t color)
{
    texture_t* ret = malloc(sizeof(texture_t));
    ret->ref_count = 1;
    ret->type = TEXTURE_SOLID;
    vec3_copy(color, ret->underlying.solid.color);
    return ret;
}

texture_t* texture_checkered_new(texture_t* tex1, texture_t* tex2, float width)
{
    texture_t* ret = malloc(sizeof(texture_t));
    ret->ref_count = 1;
    ret->type = TEXTURE_CHECKERED;
    ret->underlying.checkered.textures[0] = texture_acquire(tex1);
    ret->underlying.checkered.textures[1] = texture_acquire(tex2);
    ret->underlying.checkered.width = width;
    return ret;
}

texture_t* texture_checkered_solid_new(const vec3_t color1, const vec3_t color2, float width)
{
    texture_t* ret = malloc(sizeof(texture_t));
    ret->ref_count = 1;
    ret->type = TEXTURE_CHECKERED;
    ret->underlying.checkered.textures[0] = texture_solid_new(color1);
    ret->underlying.checkered.textures[1] = texture_solid_new(color2);
    ret->underlying.checkered.width = width;
    return ret;
}

texture_t* texture_acquire(texture_t* other)
{
    other->ref_count++;
    return other;
}

void texture_release(texture_t* self)
{
    if (--self->ref_count == 0)
    {
        texture_destroy(self);
    }
}