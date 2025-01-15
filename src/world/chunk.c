#include "world/chunk.h"

#include <glad/glad.h>
#include <stdlib.h>

#include "core/log.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"

#define ADD_VERTEX(vertex)               \
    do {                                 \
        vertices[vertex_count] = vertex; \
        ++vertex_count;                  \
    } while (0)

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
        LOG_ERROR("'chunk_get_block' called with NULL chunk");
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
        LOG_ERROR("'chunk_set_block' called with NULL chunk");
        return;
    }

    if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 || position.y >= CHUNK_HEIGHT || position.z < 0 ||
        position.z >= CHUNK_SIZE) {
        return;
    }

    chunk->blocks[position.x + position.y * CHUNK_SIZE + position.z * (CHUNK_SIZE * CHUNK_HEIGHT)] = block;
    chunk->dirty                                                                                   = 1;
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
        block_faces_t faces = block_get_faces(block, (vec3s) {{x, y, z}}, tilemap);
        int face_count      = 0;

        // Top face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x, y + 1, z}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_TOP];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        // Bottom face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x, y - 1, z}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_BOTTOM];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        // Front face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x, y, z + 1}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_FRONT];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        // Back face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x, y, z - 1}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_BACK];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        // Left face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x - 1, y, z}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_LEFT];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        // Right face
        if (!block_is_opaque(chunk_get_block(chunk, (ivec3s) {{x + 1, y, z}}))) {
            block_face_data_t face = faces.values[BLOCK_FACE_RIGHT];
            for (int j = 0; j < 4; ++j) {
                ADD_VERTEX(face.vertices[j]);
            }
            ++face_count;
        }

        int t = vertex_count - 4 * face_count;
        for (int j = 0; j < face_count; ++j) {
            indices[index_count++] = t + j * 4 + 0;
            indices[index_count++] = t + j * 4 + 1;
            indices[index_count++] = t + j * 4 + 2;
            indices[index_count++] = t + j * 4 + 2;
            indices[index_count++] = t + j * 4 + 3;
            indices[index_count++] = t + j * 4 + 0;
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
