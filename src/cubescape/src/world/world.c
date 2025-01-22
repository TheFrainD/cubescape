#include "world/world.h"

#include <stdlib.h>
#include <string.h>

#include <cubelog/cubelog.h>

#include "world/generator.h"

uint64_t jenkins_hash(void *key) {
    ivec2s *index = (ivec2s *)key;
    size_t hash   = 0;

    // Hash x component
    hash += index->x;
    hash += hash << 10;
    hash ^= hash >> 6;

    // Hash y component
    hash += index->y;
    hash += hash << 10;
    hash ^= hash >> 6;

    // Finalize hash
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

bool chunk_equals(void *key1, void *key2) {
    ivec2s a = *(ivec2s *)key1;
    ivec2s b = *(ivec2s *)key2;
    return glms_ivec2_eqv(a, b);
}

void chunk_destroy_fn(void *chunk) { return chunk_destroy((chunk_t *)chunk); }

world_t *world_create(world_settings_t settings) {
    world_t *world = malloc(sizeof(world_t));
    world->chunks  = htable_create(100, sizeof(ivec2s), jenkins_hash, chunk_equals, NULL, chunk_destroy_fn);
    return world;
}

void world_destroy(world_t *world) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_destroy' called with NULL world");
        return;
    }

    htable_destroy(world->chunks);
    free(world);
}

chunk_t *world_get_chunk(world_t *world, ivec2s index) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_get_chunk' called with NULL world");
        return NULL;
    }

    return (chunk_t *)htable_get(world->chunks, &index);
}

block_id_t world_get_block(world_t *world, ivec3s position) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_get_block' called with NULL world");
        return BLOCK_ID_AIR;
    }

    ivec2s index = (ivec2s) {{position.x / CHUNK_SIZE, position.z / CHUNK_SIZE}};

    chunk_t *chunk = world_get_chunk(world, index);
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

    ivec2s index = (ivec2s) {{position.x / CHUNK_SIZE, position.z / CHUNK_SIZE}};

    chunk_t *chunk = world_get_chunk(world, index);
    if (chunk == NULL) {
        return;
    }

    ivec3s block_position = (ivec3s) {{position.x % CHUNK_SIZE, position.y % CHUNK_HEIGHT, position.z % CHUNK_SIZE}};
    chunk_set_block(chunk, block_position, block);
}

chunk_t *world_add_chunk(world_t *world, ivec2s index) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_add_chunk' called with NULL world");
        return NULL;
    }

    if (world_get_chunk(world, index) != NULL) {
        CUBELOG_WARN("Chunk at position (%d, %d) already exists", index.x, index.y);
        return NULL;
    }

    chunk_t *chunk = chunk_create(index, world);
    chunk_set_neighbor(chunk, CHUNK_NEIGHBOR_FRONT, world_get_chunk(world, glms_ivec2_add(index, (ivec2s) {{0, -1}})));
    chunk_set_neighbor(chunk, CHUNK_NEIGHBOR_BACK, world_get_chunk(world, glms_ivec2_add(index, (ivec2s) {{0, 1}})));
    chunk_set_neighbor(chunk, CHUNK_NEIGHBOR_LEFT, world_get_chunk(world, glms_ivec2_add(index, (ivec2s) {{-1, 0}})));
    chunk_set_neighbor(chunk, CHUNK_NEIGHBOR_RIGHT, world_get_chunk(world, glms_ivec2_add(index, (ivec2s) {{1, 0}})));

    htable_set(world->chunks, &index, chunk);
    return chunk;
}
