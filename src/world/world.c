#include "world/world.h"

#include <stdlib.h>
#include <string.h>

#include "core/assert.h"
#include "core/log.h"

static uint64_t jenkins_hash(void *key) {
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

static bool_t key_equal(void *key1, void *key2) {
    ivec2s a = *(ivec2s *)key1;
    ivec2s b = *(ivec2s *)key2;
    return glms_ivec2_eqv(a, b);
}

world_t *world_create(world_settings_t settings) {
    world_t *world = malloc(sizeof(world_t));
    if (world == NULL) {
        LOG_ERROR("Failed to allocate memory for world");
        return NULL;
    }
    world->chunks = htable_create(settings.htable_initial_capacity, sizeof(ivec2s), jenkins_hash, key_equal, NULL,
                                  (htable_data_free_fn)chunk_destroy);
    if (world->chunks == NULL) {
        LOG_ERROR("Failed to create hash table for chunks");
        free(world);
        return NULL;
    }

    world->generator = world_generator_create(settings.generator_parameters);
    return world;
}

void world_destroy(world_t *world) {
    ASSERT(world != NULL);

    world_generator_destroy(world->generator);
    htable_destroy(world->chunks);
    free(world);
}

chunk_t *world_get_chunk(world_t *world, ivec2s index) {
    if (world == NULL) {
        LOG_ERROR("'world_get_chunk' called with NULL world");
        return NULL;
    }

    return htable_get(world->chunks, &index);
}

block_id_t world_get_block(world_t *world, ivec3s position) {
    ASSERT(world != NULL);

    ivec2s index = (ivec2s) {{floorf((float)position.x / CHUNK_SIZE), floorf((float)position.z / CHUNK_SIZE)}};

    chunk_t *chunk = world_get_chunk(world, index);
    if (chunk == NULL) {
        return BLOCK_ID_AIR;
    }

    ivec3s block_position =
        (ivec3s) {{position.x - (index.x * CHUNK_SIZE), position.y, position.z - (index.y * CHUNK_SIZE)}};
    return chunk_get_block(chunk, block_position);
}

ivec3s world_to_block(vec3s world_pos) {
    return (ivec3s) {{floorf(world_pos.x), floorf(world_pos.y), floorf(world_pos.z)}};
}

void world_set_block(world_t *world, ivec3s position, block_id_t block) {
    ASSERT(world != NULL);

    ivec2s index = (ivec2s) {{floorf((float)position.x / CHUNK_SIZE), floorf((float)position.z / CHUNK_SIZE)}};

    chunk_t *chunk = world_get_chunk(world, index);
    if (chunk == NULL) {
        return;
    }

    ivec3s block_position =
        (ivec3s) {{position.x - (index.x * CHUNK_SIZE), position.y, position.z - (index.y * CHUNK_SIZE)}};
    chunk_set_block(chunk, block_position, block);
}

chunk_t *world_add_chunk(world_t *world, ivec2s index) {
    ASSERT(world != NULL);

    LOG_DEBUG("Adding chunk at index (%d, %d)", index.x, index.y);

    chunk_t *chunk = chunk_create(index, world);
    htable_set(world->chunks, &index, chunk);

    for (int i = 0; i < 4; ++i) {
        ivec2s neighbor_index = (ivec2s) {{index.x + (i % 2 == 0 ? 1 : -1), index.y + (i / 2 == 0 ? 1 : -1)}};
        chunk_t *neighbor     = world_get_chunk(world, neighbor_index);
        if (!neighbor) {
            continue;
        }
        neighbor->flags.dirty = CS_TRUE;
    }

    return chunk;
}

void world_generate_chunk(world_t *world, chunk_t *chunk) {
    ASSERT(world != NULL);
    ASSERT(chunk != NULL);

    world_generator_generate(world->generator, chunk);
}

void world_delete_far_chunks(world_t *world, vec3s camera_position, int draw_distance) {
    ASSERT(world != NULL);

    ivec2s index =
        (ivec2s) {{camera_position.x >= 0 ? (camera_position.x / CHUNK_SIZE) : (camera_position.x / CHUNK_SIZE - 1),
                   camera_position.z >= 0 ? (camera_position.z / CHUNK_SIZE) : (camera_position.z / CHUNK_SIZE - 1)}};

    htable_iter_t iter = htable_iter(world->chunks);
    while (htable_next(&iter)) {
        chunk_t *chunk = iter.value;

        if (chunk->flags.generating && chunk->flags.mesh_generating) {
            continue;
        }

        // Calculate the distance between the camera and the chunk
        int diff_x = abs(chunk->position.x - index.x);
        int diff_y = abs(chunk->position.y - index.y);
        int delete_distance =
            draw_distance + 2;  // Add 2 to the draw distance to prevent chunks from being deleted too early

        if (diff_x > delete_distance || diff_y > delete_distance) {
            LOG_DEBUG("Deleting chunk at position (%d, %d)", chunk->position.x, chunk->position.y);
            htable_remove(world->chunks, &chunk->position);
            break;
        }
    }
}