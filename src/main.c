#include <stdbool.h>
#include "vec.h"

#include "ray.h"

int main(int argc, char** argv)
{
    ray_t ray = {.begin = {0.0f, 0.0f, 0.0f}, .dir = {0.0f, 1.0f, 0.0f}};
    sphere_t sphere = { .center = {0.3f, 0.5f, 0.0f}, .radius = 0.2f };
    ray_hit_t hit;
    bool res = ray_intersect_sphere(&ray, &sphere, &hit);
    printf("%d", res);

    return 0;
}