#pragma once 

#include <stdint.h>

typedef enum {
    IMAGE_FORMAT_RGB = 3,
    IMAGE_FORMAT_RGBA = 4
} image_format_t;

typedef struct {
    int32_t width;
    int32_t height;
    image_format_t format;
    uint8_t *data;
} image_t;

/**
 * @brief Loads an image from a file.
 * 
 * This function reads an image from the specified file and returns an image_t object.
 * 
 * @param filename The path to the image file to be loaded.
 * @return image_t* A pointer to the image_t object representing the loaded image.
 */
image_t *image_load(const char *filename);

/**
 * @brief Frees the memory allocated for the given image_t object.
 * 
 * This function releases the memory allocated for the image data.
 * The image_t object itself is not freed, as it is not dynamically allocated.
 * 
 * @param image A pointer to the image_t object whose data is to be freed.
 */
void image_free(image_t *image);
