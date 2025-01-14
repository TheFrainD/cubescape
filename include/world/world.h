#pragma once

#include <cglm/struct.h>

#include "world/chunk.h"

typedef struct {
    chunk_t **chunks;
    int size;
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
 * @param x The x-coordinate of the chunk.
 * @param y The y-coordinate of the chunk.
 * @return chunk_t* The chunk at the specified position.
 */
chunk_t *world_get_chunk(world_t *world, int x, int y);
