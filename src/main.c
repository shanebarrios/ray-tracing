#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "vec.h"
#include "utils.h"
#include "scene.h"
#include "renderer.h"

int main()
{
    srand(time(NULL));
    scene_t scene;
    scene_random_init(&scene);
    vec3_t* pixels = malloc(PIXEL_WIDTH * PIXEL_HEIGHT * sizeof(vec3_t));
    
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    render(&scene, pixels, PIXEL_WIDTH, PIXEL_HEIGHT);
    clock_gettime(CLOCK_MONOTONIC, &end);

    const double elapsed = (double)(end.tv_sec - begin.tv_sec) +   
        (double)(end.tv_nsec - begin.tv_nsec) / 1e9;
    printf("Rendered after %f seconds\n", elapsed);

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