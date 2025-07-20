#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "vec.h"
#include "ray.h"
#include "utils.h"
#include "scene.h"
#include "renderer.h"

int main(int argc, char** argv)
{
    scene_t scene;
    scene_default_init(&scene);
    vec3_t* pixels = (vec3_t*) malloc(PIXEL_WIDTH * PIXEL_HEIGHT * sizeof(vec3_t));
    
    clock_t begin, end;
    begin = clock();
    render(&scene, pixels, PIXEL_WIDTH, PIXEL_HEIGHT);
    end = clock();

    float elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
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