#include "graphics/shader_program.h"

#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#include "core/log.h"

static void print_info_log(uint32_t program, const char *message) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return;
    }

    char *info_log = malloc(length);
    glGetProgramInfoLog(program, length, &length, info_log);

    log_error("%s: %s", message, info_log);

    free(info_log);
}

ShaderProgram create_shader_program() {
    ShaderProgram program;
    program.id = glCreateProgram();
    memset(program.uniform_block_indeces, 0, sizeof(program.uniform_block_indeces));

    log_trace("Created shader program with ID: %d", program.id);
    return program;
}

void destroy_shader_program(ShaderProgram *program) {
    log_trace("Deleting shader program with ID: %d", program->id);

    glDeleteProgram(program->id);

    for (size_t i = 0; i < UNIFROM_BLOCK_INDEX_SIZE; i++) {
        if (program->uniform_block_indeces[i]) {
            free(program->uniform_block_indeces[i]);
        }
    }
}

void attach_shader(ShaderProgram *program, uint32_t shader) {
    glAttachShader(program->id, shader);
}

void link_shader_program(ShaderProgram *program) {
    glLinkProgram(program->id);

    int success;
    glGetProgramiv(program->id, GL_LINK_STATUS, &success);

    if (!success) {
        print_info_log(program->id, "Shader program link error");
    }
}

void use_shader_program(ShaderProgram *program) {
    glUseProgram(program->id);
}

uint32_t get_uniform_block_index(ShaderProgram *program, const char *name) {
    for (size_t i = 0; i < UNIFROM_BLOCK_INDEX_SIZE; i++) {
        if (program->uniform_block_indeces[i] && strcmp(program->uniform_block_indeces[i], name) == 0) {
            return i;
        }
    }

    uint32_t index = glGetUniformBlockIndex(program->id, name);
    if (index == GL_INVALID_INDEX) {
        log_error("Uniform block '%s' not found", name);
        return GL_INVALID_INDEX;
    }

    if (program->uniform_block_indeces[index]) {
        free(program->uniform_block_indeces[index]);
        log_warn("Uniform block index %d was overwritten", index);
    }

    program->uniform_block_indeces[index] = strdup(name);
    log_debug("Uniform block '%s' index: %d", name, index);

    return index;
}

void bind_uniform_block(ShaderProgram *program, const char *name, uint32_t binding_point) {
    uint32_t index = get_uniform_block_index(program, name);
    glUniformBlockBinding(program->id, index, binding_point);
}