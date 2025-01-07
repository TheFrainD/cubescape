#pragma once

#include <stdint.h>

typedef enum {
    SHADER_TYPE_VERTEX = 0x8B31,
    SHADER_TYPE_FRAGMENT = 0x8B30,
    SHADER_TYPE_GEOMETRY = 0x8DD9
} ShaderType;

uint32_t create_shader(ShaderType type, const char *source);
void destroy_shader(uint32_t *shader);