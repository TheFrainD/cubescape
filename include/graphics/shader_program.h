#pragma once

#include <stdint.h>

#define UNIFROM_BLOCK_INDEX_SIZE 64

typedef struct ShaderProgram {
    uint32_t id;
    char *uniform_block_indeces[UNIFROM_BLOCK_INDEX_SIZE];
} ShaderProgram;

ShaderProgram create_shader_program();
void destroy_shader_program(ShaderProgram *program);

void attach_shader(ShaderProgram *program, uint32_t shader);

void link_shader_program(ShaderProgram *program);
void use_shader_program(ShaderProgram *program);

uint32_t get_uniform_block_index(ShaderProgram *program, const char *name);
void bind_uniform_block(ShaderProgram *program, const char *name, uint32_t binding_point);