#include "graphics/texture.h"

#include <glad/glad.h>

#include "core/log.h"

uint32_t create_texture() {
    uint32_t texture;
    glGenTextures(1, &texture);
    LOG_TRACE("Created texture with ID: %d", texture);
    return texture;
}

void bind_texture(uint32_t texture, uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void unbind_texture(uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_data(uint32_t texture, const uint8_t *data, int width, int height, ImageFormat format) {
    GLenum internal_format;
    GLenum display_format;

    switch (format) {
        case IMAGE_FORMAT_RGB:
            internal_format = GL_RGB;
            display_format = GL_RGB;
            break;
        case IMAGE_FORMAT_RGBA:
            internal_format = GL_RGBA;
            display_format = GL_RGBA;
            break;
        default:
            LOG_ERROR("Unsupported image format: %d", format);
            return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, display_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_image(uint32_t texture, Image image) {
    texture_set_data(texture, image.data, image.width, image.height, image.format);
}

void texture_set_wrapping(uint32_t texture, TextureWrapping s, TextureWrapping t) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_filtering(uint32_t texture, TextureFiltering min, TextureFiltering mag) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_generate_mipmaps(uint32_t texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_set_anisotropy(uint32_t texture, float anisotropy) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void destroy_texture(uint32_t *texture) {
    LOG_TRACE("Deleting texture with ID: %d", *texture);
    glDeleteTextures(1, texture);
}