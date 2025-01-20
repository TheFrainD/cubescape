#include "world/world.h"

#include <stdlib.h>

#include <cubelog/cubelog.h>

#include "world/generator.h"

world_t *world_create(world_settings_t settings) {
    world_t *world = malloc(sizeof(world_t));
    world->chunks  = llist_create();
    return world;
}

void world_destroy(world_t *world) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_destroy' called with NULL world");
        return;
    }

    LLIST_FOREACH(world->chunks, node) {
        chunk_t *chunk = node->data;
        chunk_destroy(chunk);
    }
    llist_destroy(world->chunks);
    free(world);
}

chunk_t *world_get_chunk(world_t *world, ivec2s index) {
    if (world == NULL) {
        CUBELOG_ERROR("'world_get_chunk' called with NULL world");
        return NULL;
    }

    LLIST_FOREACH(world->chunks, node) {
        chunk_t *chunk = node->data;
        if (glms_ivec2_eqv(chunk->position, index)) {
            return chunk;
        }
    }

    return NULL;
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
    llist_append(world->chunks, chunk);
    return chunk;
}
