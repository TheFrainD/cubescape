#include "graphics/shader_program.h"

#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#include "core/log.h"

struct shader_program{
    uint32_t id;
    char *uniform_block_indeces[UNIFROM_BLOCK_INDEX_SIZE];
};

static void print_info_log(uint32_t program, const char *message) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return;
    }

    char *info_log = malloc(length);
    glGetProgramInfoLog(program, length, &length, info_log);

    LOG_ERROR("%s: %s", message, info_log);

    free(info_log);
}

shader_program_t *shader_program_create() {
    shader_program_t *program = malloc(sizeof(shader_program_t));
    program->id = glCreateProgram();
    memset(program->uniform_block_indeces, 0, sizeof(program->uniform_block_indeces));

    LOG_TRACE("Created shader program with ID: %d", program->id);
    return program;
}

void shader_program_destroy(shader_program_t *program) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_destroy' called with NULL program");
        return;
    }

    LOG_TRACE("Destroying shader program with ID: %d", program->id);

    glDeleteProgram(program->id);

    for (size_t i = 0; i < UNIFROM_BLOCK_INDEX_SIZE; i++) {
        if (program->uniform_block_indeces[i]) {
            free(program->uniform_block_indeces[i]);
        }
    }

    free(program);
}

void shader_program_attach_shader(shader_program_t *program, uint32_t shader) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_attach_shader' called with NULL program");
        return;
    }
    glAttachShader(program->id, shader);
}

void shader_program_link(shader_program_t *program) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_link' called with NULL program");
        return;
    }

    glLinkProgram(program->id);

    int success;
    glGetProgramiv(program->id, GL_LINK_STATUS, &success);

    if (!success) {
        print_info_log(program->id, "Shader program link error");
    }
}

void shader_program_use(shader_program_t *program) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_use' called with NULL program");
        return;
    }

    glUseProgram(program->id);
}

uint32_t shader_program_get_uniform_block_index(shader_program_t *program, const char *name) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_get_uniform_block_index' called with NULL program");
        return GL_INVALID_INDEX;
    }

    for (size_t i = 0; i < UNIFROM_BLOCK_INDEX_SIZE; i++) {
        if (program->uniform_block_indeces[i] && strcmp(program->uniform_block_indeces[i], name) == 0) {
            return i;
        }
    }

    uint32_t index = glGetUniformBlockIndex(program->id, name);
    if (index == GL_INVALID_INDEX) {
        LOG_ERROR("Uniform block '%s' not found", name);
        return GL_INVALID_INDEX;
    }

    if (program->uniform_block_indeces[index]) {
        free(program->uniform_block_indeces[index]);
        LOG_WARN("Uniform block index %d was overwritten", index);
    }

    program->uniform_block_indeces[index] = strdup(name);
    LOG_DEBUG("Uniform block '%s' index: %d", name, index);

    return index;
}

void shader_program_bind_uniform_block(shader_program_t *program, const char *name, uint32_t binding_point) {
    if (program == NULL) {
        LOG_ERROR("'shader_program_bind_uniform_block' called with NULL program");
        return;
    }

    uint32_t index = shader_program_get_uniform_block_index(program, name);
    glUniformBlockBinding(program->id, index, binding_point);
}
