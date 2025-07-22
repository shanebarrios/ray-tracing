#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vec.h"

#pragma pack(push, 1)

struct bitmap_header
{
    char header_field[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t image_offset;
};

struct dib_header
{
    uint32_t header_size;
    uint32_t bitmap_width;
    uint32_t bitmap_height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t num_colors;
    uint32_t num_important_colors;
};

#pragma pack(pop)

bool write_pixels_to_bmp(const vec3_t* pixels, size_t width, size_t height, const char* path)
{
    FILE* file = fopen(path, "wb");
    if (!file) return false;

    const uint32_t byte_padding = width % 4;
    const uint32_t row_size = width * 3 + byte_padding;
    const uint32_t pixels_size = row_size * height;

    const struct bitmap_header bitmap_header =
    {
        .header_field = {'B', 'M'},
        .file_size = 14 + 40 + pixels_size,
        .reserved = 0,
        .image_offset = 14 + 40 
    };

    const struct dib_header dib_header =
    {
        .header_size = 40,
        .bitmap_width = width,
        .bitmap_height = height,
        .color_planes = 1,
        .bits_per_pixel = 24,
        .compression_method = 0,
        .image_size = pixels_size,
        .horizontal_resolution = 0,
        .vertical_resolution = 0,
        .num_colors = 0,
        .num_important_colors = 0
    };

    uint8_t* buf = (uint8_t*) malloc(pixels_size);
    size_t buf_index = 0;

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            const uint32_t i = y * width + x;
            const vec3_t* color = &pixels[i];
            const uint8_t r = (uint8_t) (CLAMP((*color)[0], 0.0f, 1.0f) * 0xFF); 
            const uint8_t g = (uint8_t) (CLAMP((*color)[1], 0.0f, 1.0f) * 0xFF);
            const uint8_t b = (uint8_t) (CLAMP((*color)[2], 0.0f, 1.0f) * 0xFF);

            buf[buf_index++] = b;
            buf[buf_index++] = g;
            buf[buf_index++] = r;
        }
        memset(buf + buf_index, 0, byte_padding);
        buf_index += byte_padding;
    }

    fwrite(&bitmap_header, sizeof(struct bitmap_header), 1, file);
    fwrite(&dib_header, sizeof(struct dib_header), 1, file);
    fwrite(buf, pixels_size, 1, file);

    free(buf);
    fclose(file);

    return true;
}