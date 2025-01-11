#include "graphics/shader.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"

static void print_info_log(uint32_t shader, const char *message) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return;
    }

    char *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, &length, info_log);

    LOG_ERROR("%s: %s", message, info_log);

    free(info_log);
}

uint32_t create_shader(shader_type_t type, const char *source) {
    uint32_t shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        print_info_log(shader, "Shader compilation error");
        return 0;
    }

    LOG_TRACE("Created shader with ID: %d", shader);

    return shader;
}

void destroy_shader(uint32_t *shader) {
    LOG_TRACE("Deleting shader with ID: %d", *shader);
    glDeleteShader(*shader);
}