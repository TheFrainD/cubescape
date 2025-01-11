#include "graphics/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/log.h"

image_t image_load(const char *filename) {
    image_t image = {0};
    image.data = stbi_load(filename, &image.width, &image.height, (int *)&image.format, 0);
    if (!image.data) {
        LOG_ERROR("Failed to load image: %s", filename);
    }
    LOG_TRACE("Loaded image: %s (%dx%d)", filename, image.width, image.height);
    return image;
}

void image_free(image_t image) {
    stbi_image_free(image.data);
    LOG_TRACE("Freed image data");
}