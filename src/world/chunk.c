#include "world/chunk.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"

#define VERTEX_POSITION_SIZE 3
#define VERTEX_TEXTURE_COORD_SIZE 2

#define VERTEX_SIZE (VERTEX_POSITION_SIZE + VERTEX_TEXTURE_COORD_SIZE)

#define ADD_VERTEX(vertices, x, y, z, u, v) \
    vertices[chunk->vertex_count + 0] = x; \
    vertices[chunk->vertex_count + 1] = y; \
    vertices[chunk->vertex_count + 2] = z; \
    vertices[chunk->vertex_count + 3] = u; \
    vertices[chunk->vertex_count + 4] = v; \
    chunk->vertex_count += VERTEX_SIZE;

chunk_t *create_chunk(int x, int y, tilemap_t *tilemap) {
    chunk_t *chunk = malloc(sizeof(chunk_t));
    chunk->x = x;
    chunk->y = y;
    chunk->tilemap = tilemap;
    chunk->blocks = malloc(CHUNK_VOLUME * sizeof(block_id_t));
    chunk->vertex_array = create_vertex_array();
    chunk->vertex_buffer = create_buffer(CHUNK_VOLUME * VERTEX_SIZE * 36 * sizeof(float), NULL, BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
    chunk->index_buffer = create_buffer(CHUNK_VOLUME * 36 * sizeof(int), NULL, BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    bind_vertex_array(chunk->vertex_array);
    bind_buffer(chunk->vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER);

    vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, 5 * sizeof(float), (void*)0);
    vertex_array_attrib(1, 2, VERTEX_ARRAY_DATA_TYPE_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    unbind_buffer(BUFFER_TARGET_ARRAY_BUFFER);
    unbind_vertex_array();

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

    LOG_TRACE("Created chunk at (%d, %d)", x, y);

    return chunk;
}

block_id_t get_block(chunk_t *chunk, int x, int y, int z) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) {
        return BLOCK_ID_AIR;
    }

    return chunk->blocks[x + y * CHUNK_SIZE + z * (CHUNK_SIZE * CHUNK_HEIGHT)];
}

void chunk_set_block(chunk_t *chunk, int x, int y, int z, block_id_t block) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) {
        return;
    }

    chunk->blocks[x + y * CHUNK_SIZE + z * (CHUNK_SIZE * CHUNK_HEIGHT)] = block;
}

void chunk_generate_mesh(chunk_t *chunk) {
    float *vertices = (float *)malloc(CHUNK_VOLUME * VERTEX_SIZE * 36 * sizeof(float));
    int *indices = (int *)malloc(CHUNK_VOLUME * 36 * sizeof(int));
    chunk->vertex_count = 0;
    int index_count = 0;

    for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
        // Don't render transparent blocks
        if (!is_block_opaque(chunk->blocks[i])) {
            continue;
        }

        int x = i % CHUNK_SIZE;
        int y = (i / CHUNK_SIZE) % CHUNK_HEIGHT;
        int z = i / (CHUNK_SIZE * CHUNK_HEIGHT);

        block_id_t block = chunk->blocks[i];
        block_tiles_t tiles = get_block_tiles(block);
        int row_size = chunk->tilemap->map_size / chunk->tilemap->tile_size;

        // Top face
        if (!is_block_opaque(get_block(chunk, x, y + 1, z))) {
            tile_id_t tile_id = tiles.up;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, u_min, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, u_min, v_min);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }

        // Bottom face
        if (!is_block_opaque(get_block(chunk, x, y - 1, z))) {
            tile_id_t tile_id = tiles.bottom;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, u_min, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, u_min, v_min);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }

        // Front face
        if (!is_block_opaque(get_block(chunk, x, y, z + 1))) {
            tile_id_t tile_id = tiles.front;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, u_min, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, u_min, v_min);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }

        // Back face
        if (!is_block_opaque(get_block(chunk, x, y, z - 1))) {
            tile_id_t tile_id = tiles.back;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, u_min, v_min);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, u_min, v_max);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }

        // Left face
        if (!is_block_opaque(get_block(chunk, x - 1, y, z))) {
            tile_id_t tile_id = tiles.left;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z - 0.5f, u_min, v_max);
            ADD_VERTEX(vertices, x - 0.5f, y - 0.5f, z + 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z + 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x - 0.5f, y + 0.5f, z - 0.5f, u_min, v_min);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }

        // Right face
        if (!is_block_opaque(get_block(chunk, x + 1, y, z))) {
            tile_id_t tile_id = tiles.right;

            float u_min = (tile_id % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;
            float u_max = ((tile_id + 1) % chunk->tilemap->tile_size) / (float)chunk->tilemap->tile_size;

            float v_min = (tile_id / row_size) / (float)chunk->tilemap->tile_size;
            float v_max = ((tile_id + row_size) / row_size) / (float)chunk->tilemap->tile_size;

            int start_index = chunk->vertex_count / VERTEX_SIZE;
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z + 0.5f, u_min, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y - 0.5f, z - 0.5f, u_max, v_max);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z - 0.5f, u_max, v_min);
            ADD_VERTEX(vertices, x + 0.5f, y + 0.5f, z + 0.5f, u_min, v_min);

            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 1;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index;
            indices[index_count++] = start_index + 2;
            indices[index_count++] = start_index + 3;
        }
    }

    vertices = (float *)realloc(vertices, chunk->vertex_count * sizeof(float));
    indices = (int *)realloc(indices, index_count * sizeof(int));
    buffer_sub_data(chunk->vertex_buffer, 0, chunk->vertex_count * sizeof(float), vertices);
    buffer_sub_data(chunk->index_buffer, 0, index_count * sizeof(int), indices);

    free(vertices);
    free(indices);
}

void chunk_render(chunk_t *chunk, shader_program_t *shader_program) {
    use_shader_program(shader_program);
    bind_vertex_array(chunk->vertex_array);
    bind_buffer(chunk->index_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
    glDrawElements(GL_TRIANGLES, chunk->vertex_count / VERTEX_SIZE * 6, GL_UNSIGNED_INT, 0);
    unbind_buffer(BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
}

void destroy_chunk(chunk_t *chunk) {
    destroy_buffer(&chunk->vertex_buffer);
    destroy_vertex_array(&chunk->vertex_array);
    free(chunk->blocks);
    free(chunk);
}
