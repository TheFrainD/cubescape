#pragma once

#include <cglm/struct.h>

#include "collections/llist.h"

#include "world/chunk.h"

typedef struct {
    llist_t *chunks;
} world_t;

typedef struct {
    int size;
} world_settings_t;

/**
 * @brief Creates a new world.
 *
 * @param world_settings The settings to use for the world.
 *
 * @return world_t* The created world.
 */
world_t *world_create(world_settings_t settings);

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
 * @param index The index of the chunk.
 * @return chunk_t* The chunk at the specified position.
 */
chunk_t *world_get_chunk(world_t *world, ivec2s index);

/**
 * @brief Converts a world position to a block position.
 *
 * @param world_pos The world position to convert.
 *
 * @return ivec3s The block position.
 */
ivec3s world_to_block(vec3s world_pos);

/**
 * @brief Retrieves the block at the specified position in the world.
 *
 * @param world The world to retrieve the block from.
 * @param position The position of the block.
 *
 * @return block_id_t The block at the specified position.
 */
block_id_t world_get_block(world_t *world, ivec3s position);

/**
 * @brief Sets the block at the specified position in the world.
 *
 * @param world The world to set the block in.
 * @param position The position of the block.
 * @param block The block to set.
 */
void world_set_block(world_t *world, ivec3s position, block_id_t block);

/**
 * @brief Adds a chunk to the world.
 *
 * @param world The world to add the chunk to.
 * @param index The index of the chunk.
 * @return chunk_t* The added chunk.
 */
chunk_t *world_add_chunk(world_t *world, ivec2s index);
