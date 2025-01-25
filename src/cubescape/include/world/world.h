#pragma once

#include <cglm/struct.h>

#include "collections/htable.h"

#include "world/chunk.h"
#include "world/generator.h"

typedef struct {
    htable_t *chunks;
    world_generator_t *generator;
} world_t;

typedef struct {
    size_t htable_initial_capacity;
    world_generator_parameters_t generator_parameters;
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
 * @param create_if_null Whether to create the chunk if it does not exist.
 * @return chunk_t* The chunk at the specified position.
 */
chunk_t *world_get_chunk(world_t *world, ivec2s index, bool create_if_null);

/**
 * @brief Generates the specified chunk.
 *
 * @param world The world to generate the chunk in.
 * @param chunk The chunk to generate.
 */
void world_generate_chunk(world_t *world, chunk_t *chunk);

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

/**
 * @brief Deletes chunks that are further than the specified draw distance from the camera.
 *
 * @param world The world to delete chunks from.
 * @param camera_position The position of the camera.
 * @param draw_distance The draw distance.
 */
void world_delete_far_chunks(world_t *world, vec3s camera_position, int draw_distance);
