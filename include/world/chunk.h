#pragma once

#include <stdint.h>
#include <stddef.h>

#include "world/block.h"
#include "graphics/tilemap.h"
#include "graphics/mesh.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT)

typedef struct {
    int x;
    int y;
    block_id_t *blocks;
    mesh_t *mesh;
    tilemap_t *tilemap;
} chunk_t;

/**
 * @brief Creates a new chunk.
 * 
 * @param x The x-coordinate of the chunk.
 * @param y The y-coordinate of the chunk.
 * @param tilemap The tilemap to use for the chunk.
 * @return chunk_t* The created chunk.
 */
chunk_t *chunk_create(int x, int y, tilemap_t *tilemap);

/**
 * @brief Retrieves the block at the specified position in the chunk.
 * 
 * @param chunk The chunk to retrieve the block from.
 * @param x The x-coordinate of the block.
 * @param y The y-coordinate of the block.
 * @param z The z-coordinate of the block.
 * @return block_id_t The block at the specified position.
 */
block_id_t chunk_get_block(chunk_t *chunk, int x, int y, int z);

/**
 * @brief Sets the block at the specified position in the chunk.
 * 
 * @param chunk The chunk to set the block in.
 * @param x The x-coordinate of the block.
 * @param y The y-coordinate of the block.
 * @param z The z-coordinate of the block.
 * @param block The block to set.
 */
void chunk_set_block(chunk_t *chunk, int x, int y, int z, block_id_t block);

/**
 * @brief Generates the mesh for the chunk.
 * 
 * @param chunk The chunk to generate the mesh for.
 */
void chunk_generate_mesh(chunk_t *chunk, shader_program_t *shader_program, uint32_t texture);

/**
 * @brief Renders the chunk.
 * 
 * @param chunk The chunk to render.
 * @param shader_program The shader program to use for rendering.
 */
void chunk_render(chunk_t *chunk, shader_program_t *shader_program);

/**
 * @brief Destroys the chunk and releases any associated resources.
 * 
 * @param chunk The chunk to destroy.
 */
void chunk_destroy(chunk_t *chunk);
