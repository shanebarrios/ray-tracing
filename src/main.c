#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "pcg_basic.h"
#include "vec.h"
#include "utils.h"
#include "scene.h"
#include "renderer.h"

#define TIME(fmt, ...) \
clock_gettime(CLOCK_MONOTONIC, &begin); \
do __VA_ARGS__ while(0); \
clock_gettime(CLOCK_MONOTONIC, &end); \
elapsed = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec) / 1e9; \
printf(fmt, elapsed) \


int main()
{
    struct timespec begin, end;
    double elapsed;
    pcg32_srandom(80, time(NULL));
    //srand(time(NULL));

    scene_t scene;
    TIME("Scene initialized in %f seconds\n", {
        scene_random_init(&scene);
    });

    vec3_t* pixels = malloc(PIXEL_WIDTH * PIXEL_HEIGHT * sizeof(vec3_t));
    
    TIME("Scene rendered in %f seconds\n", {
        render(&scene, pixels, PIXEL_WIDTH, PIXEL_HEIGHT);
    });

    int success = 0;
    if (!write_pixels_to_bmp(pixels, PIXEL_WIDTH, PIXEL_HEIGHT, "img.bmp"))
    {
        fprintf(stderr, "Failed to write pixels");
        success = -1;
    }
    free(pixels);
    scene_destroy(&scene);

    return success;
}

#undef TIME