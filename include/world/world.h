#pragma once

#include <cglm/struct.h>

#include "world/chunk.h"
#include "graphics/tilemap.h"
#include "graphics/texture.h"
#include "graphics/shader_program.h"

#define WORLD_SIZE 32

typedef struct {
    chunk_t **chunks;
} world_t;

/**
 * @brief Creates a new world.
 * 
 * @param world The world to prepare.
 * @param tilemap The tilemap to use for the world.
 * @param texture The texture to use for the world.
 * @param shader_program The shader program to use for rendering.
 * 
 * @return world_t* The created world.
 */
world_t *world_create(tilemap_t *tilemap, uint32_t texture, shader_program_t *shader_program);

/**
 * @brief Destroys the specified world.
 * 
 * This function frees the memory allocated for the specified world object.
 * 
 * @param world A pointer to the world object to destroy.
 */
void world_destroy(world_t *world);

/**
 * @brief Retrieves the chunk at the specified position in the world.
 * 
 * @param world The world to retrieve the chunk from.
 * @param x The x-coordinate of the chunk.
 * @param y The y-coordinate of the chunk.
 * @return chunk_t* The chunk at the specified position.
 */
chunk_t *world_get_chunk(world_t *world, int x, int y);
