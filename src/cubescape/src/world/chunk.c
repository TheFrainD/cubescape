#include "world/chunk.h"

#include <cubegl/buffer.h>
#include <cubegl/vertex_array.h>
#include <cubelog/cubelog.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

static int should_render_face(chunk_t *chunk, block_face_t face, ivec3s position, chunk_t **neighbors) {
    ivec3s adjacent_position;

    block_id_t block = -1;

    switch (face) {
        case BLOCK_FACE_TOP:
            adjacent_position = (ivec3s) {{position.x, position.y + 1, position.z}};
            if (adjacent_position.y >= CHUNK_HEIGHT) {
                return 1;
            }
            break;
        case BLOCK_FACE_BOTTOM:
            adjacent_position = (ivec3s) {{position.x, position.y - 1, position.z}};
            if (adjacent_position.y < 0) {
                return 1;
            }
            break;
        case BLOCK_FACE_FRONT:
            adjacent_position = (ivec3s) {{position.x, position.y, position.z + 1}};
            if (adjacent_position.z >= CHUNK_SIZE) {
                if (neighbors[CHUNK_NEIGHBOR_FRONT] == NULL) {
                    return 1;
                }
                block = chunk_get_block(neighbors[CHUNK_NEIGHBOR_FRONT], (ivec3s) {{position.x, position.y, 0}});
            }
            break;
        case BLOCK_FACE_BACK:
            adjacent_position = (ivec3s) {{position.x, position.y, position.z - 1}};
            if (adjacent_position.z < 0) {
                if (neighbors[CHUNK_NEIGHBOR_BACK] == NULL) {
                    return 1;
                }
                block = chunk_get_block(neighbors[CHUNK_NEIGHBOR_BACK],
                                        (ivec3s) {{position.x, position.y, CHUNK_SIZE - 1}});
            }
            break;
        case BLOCK_FACE_LEFT:
            adjacent_position = (ivec3s) {{position.x - 1, position.y, position.z}};
            if (adjacent_position.x < 0) {
                if (neighbors[CHUNK_NEIGHBOR_LEFT] == NULL) {
                    return 1;
                }
                block = chunk_get_block(neighbors[CHUNK_NEIGHBOR_LEFT],
                                        (ivec3s) {{CHUNK_SIZE - 1, position.y, position.z}});
            }
            break;
        case BLOCK_FACE_RIGHT:
            adjacent_position = (ivec3s) {{position.x + 1, position.y, position.z}};
            if (adjacent_position.x >= CHUNK_SIZE) {
                if (neighbors[CHUNK_NEIGHBOR_RIGHT] == NULL) {
                    return 1;
                }
                block = chunk_get_block(neighbors[CHUNK_NEIGHBOR_RIGHT], (ivec3s) {{0, position.y, position.z}});
            }
            break;
    }

    if (block == -1) {
        block = chunk_get_block(chunk, adjacent_position);
    }

    return !block_is_opaque(block);
}

chunk_t *chunk_create(ivec2s position) {
    chunk_t *chunk  = malloc(sizeof(chunk_t));
    chunk->position = position;
    chunk->blocks   = malloc(CHUNK_VOLUME * sizeof(block_id_t));
    chunk->mesh     = mesh_create(NULL, 0, NULL, 0, NULL, -1);
    chunk->dirty    = 1;
    return chunk;
}

block_id_t chunk_get_block(chunk_t *chunk, ivec3s position) {
    if (chunk == NULL) {
        CUBELOG_ERROR("'chunk_get_block' called with NULL chunk");
        return BLOCK_ID_AIR;
    }

    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 ||
        position.z >= CHUNK_SIZE) {
        return BLOCK_ID_AIR;
    }

    return chunk->blocks[position.x + position.y * CHUNK_SIZE + position.z * (CHUNK_SIZE * CHUNK_HEIGHT)];
}

void chunk_set_block(chunk_t *chunk, ivec3s position, block_id_t block) {
    if (chunk == NULL) {
        CUBELOG_ERROR("'chunk_set_block' called with NULL chunk");
        return;
    }

    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 ||
        position.z >= CHUNK_SIZE) {
        return;
    }

    chunk->blocks[position.x + position.y * CHUNK_SIZE + position.z * (CHUNK_SIZE * CHUNK_HEIGHT)] = block;
    chunk->dirty                                                                                   = 1;
}

void chunk_generate_mesh(chunk_t *chunk, shader_program_t *shader_program, tilemap_t *tilemap, chunk_t **neighbors) {
    if (chunk == NULL) {
        CUBELOG_ERROR("'chunk_generate_mesh' called with NULL chunk");
        return;
    }

    if (shader_program == NULL) {
        CUBELOG_ERROR("'chunk_generate_mesh' called with NULL shader program");
        return;
    }

    vertex_t *vertices  = (vertex_t *)malloc(CHUNK_VOLUME * 36 * sizeof(vertex_t));
    uint32_t *indices   = (uint32_t *)malloc(CHUNK_VOLUME * 36 * sizeof(uint32_t));
    size_t vertex_count = 0;
    size_t index_count  = 0;

    block_faces_t faces = {0};
    for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
        // Don't render transparent blocks
        if (!block_is_opaque(chunk->blocks[i])) {
            continue;
        }

        int x = i % CHUNK_SIZE;
        int y = (i / CHUNK_SIZE) % CHUNK_HEIGHT;
        int z = i / (CHUNK_SIZE * CHUNK_HEIGHT);

        block_id_t block = chunk->blocks[i];
        block_get_faces(block, (vec3s) {{x, y, z}}, tilemap, &faces);

        for (int j = 0; j < 6; ++j) {
            if (!should_render_face(chunk, j, (ivec3s) {{x, y, z}}, neighbors)) {
                continue;
            }

            memcpy(&vertices[vertex_count], faces.values[j].vertices, 4 * sizeof(vertex_t));
            vertex_count += 4;

            indices[index_count++] = vertex_count - 4;
            indices[index_count++] = vertex_count - 3;
            indices[index_count++] = vertex_count - 2;
            indices[index_count++] = vertex_count - 2;
            indices[index_count++] = vertex_count - 1;
            indices[index_count++] = vertex_count - 4;
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
        CUBELOG_ERROR("'chunk_destroy' called with NULL chunk");
        return;
    }

    mesh_destroy(chunk->mesh);
    free(chunk->blocks);
    free(chunk);
}
