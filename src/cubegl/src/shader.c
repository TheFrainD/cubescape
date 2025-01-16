#include "shader.h"

#include <cubelog/cubelog.h>
#include <glad/glad.h>
#include <stdlib.h>

static void print_info_log(uint32_t shader, const char *message) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_CUBELOG_LENGTH, &length);

    if (length == 0) {
        return;
    }

    char *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, &length, info_log);

    CUBELOG_ERROR("%s: %s", message, info_log);

    free(info_log);
}

uint32_t shader_create(shader_type_t type, const char *source) {
    uint32_t shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        print_info_log(shader, "Shader compilation error");
        return 0;
    }

    CUBELOG_TRACE("Created shader with ID: %d", shader);

    return shader;
}

void shader_destroy(uint32_t *shader) {
    if (*shader == 0) {
        CUBELOG_ERROR("'shader_destroy' called with 0 shader");
        return;
    }

    CUBELOG_TRACE("Deleting shader with ID: %d", *shader);
    glDeleteShader(*shader);
}
