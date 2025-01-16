#include "image.h"

#define STB_IMAGE_IMPLEMENTATION

#include <cubelog/cubelog.h>
#include <stb_image.h>

image_t *image_load(const char *filename) {
    image_t *image = malloc(sizeof(image_t));
    image->data    = stbi_load(filename, &image->width, &image->height, (int *)&image->format, 0);
    if (!image->data) {
        CUBELOG_ERROR("Failed to load image: %s", filename);
    }
    CUBELOG_TRACE("Loaded image: %s (%dx%d)", filename, image->width, image->height);
    return image;
}

void image_free(image_t *image) {
    if (!image) {
        CUBELOG_TRACE("'image_free' called with NULL image");
        return;
    }

    stbi_image_free(image->data);
    free(image);

    CUBELOG_TRACE("Freed image");
}
