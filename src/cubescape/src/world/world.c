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

block_id_t world_get_block(world_t *world, ivec3s position) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_get_block' called with NULL world");
        return BLOCK_ID_AIR;
    }

    ivec3s chunk_position = (ivec3s) {{position.x / CHUNK_SIZE, position.y / CHUNK_HEIGHT, position.z / CHUNK_SIZE}};

    chunk_t *chunk = world_get_chunk(world, chunk_position.x, chunk_position.z);
    if (chunk == NULL) {
        return BLOCK_ID_AIR;
    }

    ivec3s block_position = (ivec3s) {{position.x % CHUNK_SIZE, position.y % CHUNK_HEIGHT, position.z % CHUNK_SIZE}};
    return chunk_get_block(chunk, block_position);
}

ivec3s world_to_block(vec3s world_pos) {
    return (ivec3s) {{floorf(world_pos.x), floorf(world_pos.y), floorf(world_pos.z)}};
}

void world_set_block(world_t *world, ivec3s position, block_id_t block) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_set_block' called with NULL world");
        return;
    }

    ivec3s chunk_position = (ivec3s) {{position.x / CHUNK_SIZE, position.y / CHUNK_HEIGHT, position.z / CHUNK_SIZE}};

    chunk_t *chunk = world_get_chunk(world, chunk_position.x, chunk_position.z);
    if (chunk == NULL) {
        return;
    }

    ivec3s block_position = (ivec3s) {{position.x % CHUNK_SIZE, position.y % CHUNK_HEIGHT, position.z % CHUNK_SIZE}};
    chunk_set_block(chunk, block_position, block);
}
