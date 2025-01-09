#pragma once

#include <stdint.h>

#include "graphics/image.h"

typedef enum {
    TEXTURE_WRAPPING_CLAMP_TO_EDGE = 0x812F,
    TEXTURE_WRAPPING_CLAMP_TO_BORDER = 0x812D,
    TEXTURE_WRAPPING_REPEAT = 0x2901,
    TEXTURE_WRAPPING_MIRRORED_REPEAT = 0x8370,
    TEXTURE_WRAPPING_MIRROR_CLAMP_TO_EDGE = 0x8743
} TextureWrapping;

typedef enum {
    TEXTURE_FILTERING_NEAREST = 0x2600,
    TEXTURE_FILTERING_LINEAR = 0x2601,
    TEXTURE_FILTERING_NEAREST_MIPMAP_NEAREST = 0x2700,
    TEXTURE_FILTERING_LINEAR_MIPMAP_NEAREST = 0x2701,
    TEXTURE_FILTERING_NEAREST_MIPMAP_LINEAR = 0x2702,
    TEXTURE_FILTERING_LINEAR_MIPMAP_LINEAR = 0x2703
} TextureFiltering;

/**
 * @brief Creates a new texture.
 * 
 * @return The ID of the created texture.
 */
uint32_t create_texture();

/**
 * @brief Binds a texture to a specified slot.
 * 
 * @param texture The ID of the texture to bind.
 * @param slot The slot to bind the texture to.
 */
void bind_texture(uint32_t texture, uint32_t slot);

/**
 * @brief Unbinds the texture from the specified slot.
 * 
 * @param slot The slot to unbind the texture from.
 */
void unbind_texture(uint32_t slot);

/**
 * @brief Sets the data for a texture.
 * 
 * @param texture The ID of the texture.
 * @param data The data to set for the texture.
 * @param width The width of the texture.
 * @param height The height of the texture.
 * @param format The format of the image data.
 */
void texture_set_data(uint32_t texture, const uint8_t *data, int width, int height, ImageFormat format);

/**
 * @brief Sets the image for a texture.
 * 
 * @param texture The ID of the texture.
 * @param image The image to set for the texture.
 */
void texture_set_image(uint32_t texture, Image image);

/**
 * @brief Sets the wrapping mode for a texture.
 * 
 * @param texture The ID of the texture.
 * @param s The wrapping mode for the S coordinate.
 * @param t The wrapping mode for the T coordinate.
 */
void texture_set_wrapping(uint32_t texture, TextureWrapping s, TextureWrapping t);

/**
 * @brief Sets the filtering mode for a texture.
 * 
 * @param texture The ID of the texture.
 * @param min The minifying filter.
 * @param mag The magnifying filter.
 */
void texture_set_filtering(uint32_t texture, TextureFiltering min, TextureFiltering mag);

/**
 * @brief Generates mipmaps for a texture.
 * 
 * @param texture The ID of the texture.
 */
void texture_generate_mipmaps(uint32_t texture);

/**
 * @brief Sets the anisotropic filtering level for a given texture.
 *
 * Anisotropic filtering improves the quality of textures on surfaces that are at oblique viewing angles.
 *
 * @param texture The identifier of the texture for which to set the anisotropy level.
 * @param anisotropy The level of anisotropic filtering to apply. Higher values result in better quality but may impact performance.
 */
void texture_set_anisotropy(uint32_t texture, float anisotropy);

/**
 * @brief Destroys a texture.
 * 
 * @param texture A pointer to the ID of the texture to destroy.
 */
void destroy_texture(uint32_t *texture);