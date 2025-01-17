#pragma once

#include <stdint.h>

#define UNIFROM_BLOCK_INDEX_SIZE 64

typedef struct shader_program shader_program_t;

/**
 * @brief Creates a new shader program.
 * 
 * @return shader_program_t* A pointer to the newly created shader program.
 */
shader_program_t *shader_program_create();

/**
 * @brief Destroys the given shader program.
 * 
 * @param program Pointer to the shader program to be destroyed.
 */
void shader_program_destroy(shader_program_t *program);

/**
 * @brief Attaches a shader to the given shader program.
 * 
 * @param program Pointer to the shader program.
 * @param shader The shader to be attached.
 */
void shader_program_attach_shader(shader_program_t *program, uint32_t shader);

/**
 * @brief Links the given shader program.
 * 
 * @param program Pointer to the shader program to be linked.
 */
void shader_program_link(shader_program_t *program);

/**
 * @brief Uses the given shader program.
 * 
 * @param program Pointer to the shader program to be used.
 */
void shader_program_use(shader_program_t *program);

/**
 * @brief Gets the uniform block index for the given name in the shader program.
 * 
 * @param program Pointer to the shader program.
 * @param name The name of the uniform block.
 * @return uint32_t The index of the uniform block.
 */
uint32_t shader_program_get_uniform_block_index(shader_program_t *program, const char *name);

/**
 * @brief Binds the uniform block to the given binding point in the shader program.
 * 
 * @param program Pointer to the shader program.
 * @param name The name of the uniform block.
 * @param binding_point The binding point to bind the uniform block to.
 */
void shader_program_bind_uniform_block(shader_program_t *program, const char *name, uint32_t binding_point);
