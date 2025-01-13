#include "world/world.h"

#include <stdlib.h>

#include "core/log.h"

world_t *world_create(tilemap_t *tilemap, uint32_t texture, shader_program_t *shader_program) {
    world_t *world = malloc(sizeof(world_t));
    world->chunks = malloc(WORLD_SIZE * WORLD_SIZE * sizeof(chunk_t *));

    for (int x = 0; x < WORLD_SIZE; x++) {
        for (int y = 0; y < WORLD_SIZE; y++) {
            chunk_t *chunk = chunk_create(x, y, tilemap);
            world->chunks[x + y * WORLD_SIZE] = chunk;

            for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
                int y = (i / CHUNK_SIZE) % CHUNK_HEIGHT;
                if (y > 64) {
                    chunk->blocks[i] = BLOCK_ID_AIR;
                    continue;
                } else if (y > 63) {
                    chunk->blocks[i] = BLOCK_ID_GRASS;
                    continue;
                } else if (y > 59) {
                    chunk->blocks[i] = BLOCK_ID_DIRT;
                    continue;
                } else if (y > 54) {
                    chunk->blocks[i] = BLOCK_ID_COBBLESTONE;
                    continue;
                }
                chunk->blocks[i] = BLOCK_ID_STONE;
            }

            chunk_generate_mesh(chunk, shader_program, texture);
        }
    }

    return world;
}

void world_destroy(world_t *world) {
    if (world == NULL) {
        LOG_ERROR("'world_destroy' called with NULL world");
        return;
    }

    for (size_t i = 0; i < WORLD_SIZE * WORLD_SIZE; ++i) {
        chunk_destroy(world->chunks[i]);
    }

    free(world->chunks);
    free(world);
}

chunk_t *world_get_chunk(world_t *world, int x, int y) {
    if (world == NULL) {
        LOG_ERROR("'world_get_chunk' called with NULL world");
        return NULL;
    }

    if (x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE) {
        return NULL;
    }

    return world->chunks[x + y * WORLD_SIZE];
}
