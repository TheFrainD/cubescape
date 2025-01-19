#pragma once

#include <stddef.h>
#include <stdint.h>

#include "graphics/mesh.h"
#include "graphics/tilemap.h"

#include "world/block.h"

#define CHUNK_SIZE   16
#define CHUNK_HEIGHT 256
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT)

struct chunk_flags {
    bool dirty : 1;
    bool generated : 1;
    bool generating : 1;
    bool mesh_generating: 1;
};

typedef struct {
    ivec2s position;
    block_id_t *blocks;
    mesh_t *mesh;
    void *world;
    struct chunk_flags flags;
} chunk_t;

/**
 * @brief Creates a new chunk.
 *
 * @param position The position of the chunk.
 * @param world The world the chunk belongs to.
 *
 * @return chunk_t* The created chunk.
 */
chunk_t *chunk_create(ivec2s position, void *world);

/**
 * @brief Retrieves the block at the specified position in the chunk.
 *
 * @param chunk The chunk to retrieve the block from.
 * @param position The position of the block.
 *
 * @return block_id_t The block at the specified position.
 */
block_id_t chunk_get_block(chunk_t *chunk, ivec3s position);

/**
 * @brief Sets the block at the specified position in the chunk.
 *
 * @param chunk The chunk to set the block in.
 * @param position The position of the block.
 * @param block The block to set.
 */
void chunk_set_block(chunk_t *chunk, ivec3s position, block_id_t block);

/**
 * @brief Generates the mesh for the chunk.
 *
 * @param chunk The chunk to generate the mesh for.
 * @param shader_program The shader program to use for the mesh.
 * @param tilemap The tilemap to use for the mesh.
 */
void chunk_generate_mesh(chunk_t *chunk, shader_program_t *shader_program, tilemap_t *tilemap);

/**
 * @brief Destroys the chunk and releases any associated resources.
 *
 * @param chunk The chunk to destroy.
 */
void chunk_destroy(chunk_t *chunk);
