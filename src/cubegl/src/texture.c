#include "texture.h"

#include <cubelog/cubelog.h>
#include <glad/glad.h>

uint32_t texture_create() {
    uint32_t texture;
    glGenTextures(1, &texture);
    CUBELOG_TRACE("Created texture with ID: %d", texture);
    return texture;
}

void texture_bind(uint32_t texture, uint32_t slot) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_bind' called with 0 texture");
        return;
    }

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void texture_unbind(uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_data(uint32_t texture, const uint8_t *data, int width, int height, image_format_t format) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_set_data' called with 0 texture");
        return;
    }

    if (data == NULL) {
        CUBELOG_ERROR("'texture_set_data' called with NULL data");
        return;
    }

    GLenum internal_format;
    GLenum display_format;

    switch (format) {
        case IMAGE_FORMAT_RGB:
            internal_format = GL_RGB;
            display_format  = GL_RGB;
            break;
        case IMAGE_FORMAT_RGBA:
            internal_format = GL_RGBA;
            display_format  = GL_RGBA;
            break;
        default:
            CUBELOG_ERROR("Unsupported image format: %d", format);
            return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, display_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_image(uint32_t texture, image_t *image) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_set_image' called with 0 texture");
        return;
    }

    if (image == NULL) {
        CUBELOG_ERROR("'texture_set_image' called with NULL image");
        return;
    }

    texture_set_data(texture, image->data, image->width, image->height, image->format);
}

void texture_set_wrapping(uint32_t texture, texture_wrapping_t s, texture_wrapping_t t) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_set_wrapping' called with 0 texture");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_filtering(uint32_t texture, texture_filtering_t min, texture_filtering_t mag) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_set_filtering' called with 0 texture");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_generate_mipmaps(uint32_t texture) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_generate_mipmaps' called with 0 texture");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_anisotropy(uint32_t texture, float anisotropy) {
    if (texture == 0) {
        CUBELOG_ERROR("'texture_set_anisotropy' called with 0 texture");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_destroy(uint32_t *texture) {
    if (texture == NULL) {
        CUBELOG_ERROR("'texture_destroy' called with NULL texture");
        return;
    }

    if (*texture == 0) {
        CUBELOG_ERROR("'texture_destroy' called with 0 texture");
        return;
    }

    CUBELOG_TRACE("Deleting texture with ID: %d", *texture);
    glDeleteTextures(1, texture);
}
