#pragma once

#include <stdbool.h>

#include <cglm/struct.h>

#include <cubegl/buffer.h>
#include <cubegl/shader_program.h>

#include "graphics/vertex.h"

struct mesh_flags {
    bool ready_to_upload : 1;
    bool uploaded : 1;
};

typedef struct {
    vertex_t *vertices;
    size_t vertex_count;

    uint32_t *indices;
    size_t index_count;

    uint32_t vertex_array;
    buffer_t *vertex_buffer;
    buffer_t *index_buffer;
    shader_program_t *shader_program;
    uint32_t texture;

    struct mesh_flags flags;
} mesh_t;

/**
 * @brief Creates a new mesh.
 *
 * This function creates a new mesh object with the specified vertices, indices, shader program, and texture.
 *
 * @param vertices An array of vertices.
 * @param vertex_count The number of vertices in the array.
 * @param indices An array of indices.
 * @param index_count The number of indices in the array.
 * @param shader_program The shader program to use when rendering the mesh.
 * @param texture The texture to use when rendering the mesh.
 * @return mesh_t* A pointer to the newly created mesh object.
 */
mesh_t *mesh_create(const vertex_t *vertices, size_t vertex_count, const uint32_t *indices, size_t index_count,
                    shader_program_t *shader_program, uint32_t texture);

/**
 * @brief Destroys the specified mesh.
 *
 * This function frees the memory allocated for the specified mesh object.
 *
 * @param mesh A pointer to the mesh object to destroy.
 */
void mesh_destroy(mesh_t *mesh);

/**
 * @brief Sets the vertices of the specified mesh.
 *
 * This function sets the vertices of the specified mesh object.
 *
 * @param mesh A pointer to the mesh object.
 * @param vertices An array of vertices.
 * @param vertex_count The number of vertices in the array.
 */
void mesh_set_vertices(mesh_t *mesh, const vertex_t *vertices, size_t vertex_count);

/**
 * @brief Sets the indices of the specified mesh.
 *
 * This function sets the indices of the specified mesh object.
 *
 * @param mesh A pointer to the mesh object.
 * @param indices An array of indices.
 * @param index_count The number of indices in the array.
 */
void mesh_set_indices(mesh_t *mesh, const uint32_t *indices, size_t index_count);

/**
 * @brief Binds the specified mesh for rendering.
 *
 * This function binds the specified mesh for rendering.
 *
 * @param mesh A pointer to the mesh object to bind.
 */
void mesh_bind(const mesh_t *mesh);

/**
 * @brief Unbinds the currently bound mesh.
 *
 * This function unbinds the currently bound mesh.
 *
 * @param mesh A pointer to the mesh object to unbind.
 */
void mesh_unbind(const mesh_t *mesh);

/**
 * @brief Uploads the mesh to the GPU.
 *
 * This function uploads the mesh to the GPU.
 *
 * @param mesh A pointer to the mesh object to upload.
 */
void mesh_upload(mesh_t *mesh);
