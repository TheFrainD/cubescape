#include "world/world.h"

#include <cubelog/cubelog.h>
#include <stdlib.h>

world_t *world_create(world_settings_t settings) {
    world_t *world = malloc(sizeof(world_t));
    world->size    = settings.size;
    world->chunks  = malloc(world->size * world->size * sizeof(chunk_t *));

    for (int x = 0; x < world->size; x++) {
        for (int y = 0; y < world->size; y++) {
            chunk_t *chunk                     = chunk_create((ivec2s) {{x, y}});
            world->chunks[x + y * world->size] = chunk;

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
        }
    }

    return world;
}

void world_destroy(world_t *world) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_destroy' called with NULL world");
        return;
    }

    for (size_t i = 0; i < world->size * world->size; ++i) {
        chunk_destroy(world->chunks[i]);
    }

    free(world->chunks);
    free(world);
}

chunk_t *world_get_chunk(world_t *world, int x, int y) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_get_chunk' called with NULL world");
        return NULL;
    }

    if (x < 0 || x >= world->size || y < 0 || y >= world->size) {
        return NULL;
    }

    return world->chunks[x + y * world->size];
}
