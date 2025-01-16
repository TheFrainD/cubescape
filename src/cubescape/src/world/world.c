#include "world/world.h"

#define FNL_IMPL

#include <cubelog/cubelog.h>
#include <stdlib.h>
#include <FastNoiseLite.h>

world_t *world_create(world_settings_t settings) {
    world_t *world = malloc(sizeof(world_t));
    world->size    = settings.size;
    world->chunks  = malloc(world->size * world->size * sizeof(chunk_t *));

    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

    for (size_t i = 0; i < world->size * world->size; ++i) {
        ivec2s chunk_pos = (ivec2s) {{i % world->size, i / world->size}};
        chunk_t *chunk   = chunk_create(chunk_pos, world);
        world->chunks[i] = chunk;

        for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
            ivec3s block_position =
                (ivec3s) {{i % CHUNK_SIZE, (i / CHUNK_SIZE) % CHUNK_HEIGHT, i / (CHUNK_SIZE * CHUNK_HEIGHT)}};
            ivec3s world_position = (ivec3s) {{chunk_pos.x * CHUNK_SIZE + block_position.x,
                                               block_position.y,
                                               chunk_pos.y * CHUNK_SIZE + block_position.z}};
            
            vec2s noise_sample = glms_vec2_scale((vec2s){{world_position.x, world_position.z}}, 1.0f / (float)CHUNK_SIZE);
            float height = fnlGetNoise2D(&noise, noise_sample.x, noise_sample.y);

            if (block_position.y > height * CHUNK_HEIGHT) {
                chunk->blocks[i] = BLOCK_ID_AIR;
                continue;
            }
            chunk->blocks[i] = BLOCK_ID_STONE;
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
