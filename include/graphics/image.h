#pragma once 

#include <stdint.h>

typedef enum {
    IMAGE_FORMAT_RGB = 3,
    IMAGE_FORMAT_RGBA = 4
} ImageFormat;

typedef struct {
    int32_t width;
    int32_t height;
    ImageFormat format;
    uint8_t *data;
} Image;

/**
 * @brief Loads an image from a file.
 * 
 * This function reads an image from the specified file and returns an Image object.
 * 
 * @param filename The path to the image file to be loaded.
 * @return Image The loaded image.
 */
Image image_load(const char *filename);

/**
 * @brief Frees the memory allocated for the given Image object.
 * 
 * This function releases the memory allocated for the image data.
 * The Image object itself is not freed, as it is not dynamically allocated.
 * 
 * @param image A pointer to the Image object whose data is to be freed.
 */
void image_free(Image image);
