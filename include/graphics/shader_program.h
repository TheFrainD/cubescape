#pragma once

#include <stdint.h>

#define UNIFROM_BLOCK_INDEX_SIZE 64

typedef struct ShaderProgram {
    uint32_t id;
    char *uniform_block_indeces[UNIFROM_BLOCK_INDEX_SIZE];
} ShaderProgram;

/**
 * @brief Creates a new shader program.
 * 
 * @return ShaderProgram The created shader program.
 */
ShaderProgram create_shader_program();

/**
 * @brief Destroys the given shader program.
 * 
 * @param program Pointer to the shader program to be destroyed.
 */
void destroy_shader_program(ShaderProgram *program);

/**
 * @brief Attaches a shader to the given shader program.
 * 
 * @param program Pointer to the shader program.
 * @param shader The shader to be attached.
 */
void attach_shader_to_shader_program(ShaderProgram *program, uint32_t shader);

/**
 * @brief Links the given shader program.
 * 
 * @param program Pointer to the shader program to be linked.
 */
void link_shader_program(ShaderProgram *program);

/**
 * @brief Uses the given shader program.
 * 
 * @param program Pointer to the shader program to be used.
 */
void use_shader_program(ShaderProgram *program);

/**
 * @brief Gets the uniform block index for the given name in the shader program.
 * 
 * @param program Pointer to the shader program.
 * @param name The name of the uniform block.
 * @return uint32_t The index of the uniform block.
 */
uint32_t shader_program_get_uniform_block_index(ShaderProgram *program, const char *name);

/**
 * @brief Binds the uniform block to the given binding point in the shader program.
 * 
 * @param program Pointer to the shader program.
 * @param name The name of the uniform block.
 * @param binding_point The binding point to bind the uniform block to.
 */
void shader_program_bind_uniform_block(ShaderProgram *program, const char *name, uint32_t binding_point);