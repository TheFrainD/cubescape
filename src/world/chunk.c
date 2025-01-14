#include "world/chunk.h"

#include <glad/glad.h>
#include <stdlib.h>

#include "core/log.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"

#define ADD_VERTEX(vertices, x, y, z, uv)                      \
    do {                                                       \
        vertices[vertex_count] = (vertex_t) {{{x, y, z}}, uv}; \
        ++vertex_count;                                        \
    } while (0)

chunk_t *chunk_create(ivec2s position) {
    chunk_t *chunk = malloc(sizeof(chunk_t));
    chunk->position = position;
    chunk->blocks  = malloc(CHUNK_VOLUME * sizeof(block_id_t));
    chunk->mesh    = mesh_create(NULL, 0, NULL, 0, NULL, -1);
    chunk->dirty   = 1;

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

    LOG_TRACE("Created chunk at (%d, %d)", position.x, position.y);

    return chunk;
}

block_id_t chunk_get_block(chunk_t *chunk, ivec3s position) {
    if (chunk == NULL) {
        LOG_ERROR("'chunk_get_block' called with NULL chunk");
        return BLOCK_ID_AIR;
    }

    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 || position.z >= CHUNK_SIZE) {
        return BLOCK_ID_AIR;
    }

    return chunk->blocks[position.x + position.y * CHUNK_SIZE + position.z * (CHUNK_SIZE * CHUNK_HEIGHT)];
}

void chunk_set_block(chunk_t *chunk, ivec3s position, block_id_t block) {
    if (chunk == NULL) {
        LOG_ERROR("'chunk_set_block' called with NULL chunk");
        return;
    }

    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 || position.z >= CHUNK_SIZE) {
        return;
    }

    chunk->blocks[position.x + position.y * CHUNK_SIZE + position.z * (CHUNK_SIZE * CHUNK_HEIGHT)] = block;
    chunk->dirty = 1;
}

void chunk_generate_mesh(chunk_t *chunk, shader_program_t *shader_program, tilemap_t *tilemap) {
    if (chunk == NULL) {
        LOG_ERROR("'chunk_generate_mesh' called with NULL chunk");
        return;
    }

    if (shader_program == NULL) {
        LOG_ERROR("'chunk_generate_mesh' called with NULL shader program");
        return;
    }

    vertex_t *vertices  = (vertex_t *)malloc(CHUNK_VOLUME * 36 * sizeof(vertex_t));
    uint32_t *indices   = (uint32_t *)malloc(CHUNK_VOLUME * 36 * sizeof(uint32_t));
    size_t vertex_count = 0;
    size_t index_count  = 0;

    for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
        // Don't render transparent blocks
        if (!block_is_opaque(chunk->blocks[i])) {
            continue;
        }

        int x = i % CHUNK_SIZE;
        int y = (i / CHUNK_SIZE) % CHUNK_HEIGHT;
        int z = i / (CHUNK_SIZE * CHUNK_HEIGHT);

        block_id_t block    = chunk->blocks[i];
        block_tiles_t tiles = block_get_tiles(block);

        // Top face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x, y + 1, z}}))) {
            tile_id_t tile_id = tiles.up;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }

        // Bottom face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x, y - 1, z}}))) {
            tile_id_t tile_id = tiles.bottom;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }

        // Front face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x, y, z + 1}}))) {
            tile_id_t tile_id = tiles.front;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }

        // Back face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x, y, z - 1}}))) {
            tile_id_t tile_id = tiles.back;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }

        // Left face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x - 1, y, z}}))) {
            tile_id_t tile_id = tiles.left;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }

        // Right face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s){{x + 1, y, z}}))) {
            tile_id_t tile_id = tiles.right;
            tile_uv_t uv      = tilemap_get_tile_uv(tilemap, tile_id);

            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, uv.value[0]);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, uv.value[1]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, uv.value[2]);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, uv.value[3]);

            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 1;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count;
            indices[index_count++] = vertex_count + 2;
            indices[index_count++] = vertex_count + 3;
        }
    }

    vertices = (vertex_t *)realloc(vertices, vertex_count * sizeof(vertex_t));
    indices  = (uint32_t *)realloc(indices, index_count * sizeof(uint32_t));

    mesh_set_vertices(chunk->mesh, vertices, vertex_count);
    mesh_set_indices(chunk->mesh, indices, index_count);
    free(vertices);
    free(indices);

    chunk->mesh->texture        = tilemap->texture;
    chunk->mesh->shader_program = shader_program;
    chunk->dirty                = 0;
}

void chunk_destroy(chunk_t *chunk) {
    if (chunk == NULL) {
        LOG_ERROR("'chunk_destroy' called with NULL chunk");
        return;
    }

    mesh_destroy(chunk->mesh);
    free(chunk->blocks);
    free(chunk);
}
