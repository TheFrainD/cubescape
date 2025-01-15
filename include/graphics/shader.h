#pragma once

#include <stdint.h>

typedef enum {
    SHADER_TYPE_VERTEX = 0x8B31,
    SHADER_TYPE_FRAGMENT = 0x8B30,
    SHADER_TYPE_GEOMETRY = 0x8DD9
} shader_type_t;

/**
 * @brief Creates a shader of the specified type from the given source code.
 *
 * @param type The type of shader to create (e.g., vertex, fragment).
 * @param source The source code of the shader.
 * @return The ID of the created shader.
 */
uint32_t shader_create(shader_type_t type, const char *source);

/**
 * @brief Destroys the specified shader.
 *
 * @param shader A pointer to the ID of the shader to destroy.
 */
void shader_destroy(uint32_t *shader);
