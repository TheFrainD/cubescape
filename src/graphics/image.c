#include "graphics/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/log.h"

Image image_load(const char *filename) {
    Image image = {0};
    image.data = stbi_load(filename, &image.width, &image.height, (int *)&image.format, 0);
    if (!image.data) {
        LOG_ERROR("Failed to load image: %s", filename);
    }
    LOG_TRACE("Loaded image: %s (%dx%d)", filename, image.width, image.height);
    return image;
}

void image_free(Image image) {
    stbi_image_free(image.data);
    LOG_TRACE("Freed image data");
}