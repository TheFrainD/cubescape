#pragma once

#include <cglm/struct.h>
#include <cubegl/shader_program.h>

#include "graphics/vertex.h"

typedef struct mesh_private_data mesh_private_data_t;

typedef struct {
    shader_program_t *shader_program;
    uint32_t texture;

    mesh_private_data_t *private_data;
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
 *
 * @return mesh_t* A pointer to the newly created mesh object.
 */
mesh_t *mesh_create(const vertex_t *const vertices, size_t vertex_count, const uint32_t *const indices,
                    size_t index_count, shader_program_t *shader_program, uint32_t texture);

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
void mesh_set_vertices(const mesh_t *mesh, const vertex_t *const vertices, size_t vertex_count);

/**
 * @brief Sets the indices of the specified mesh.
 *
 * This function sets the indices of the specified mesh object.
 *
 * @param mesh A pointer to the mesh object.
 * @param indices An array of indices.
 * @param index_count The number of indices in the array.
 */
void mesh_set_indices(const mesh_t *mesh, const uint32_t *const indices, size_t index_count);

/**
 * @brief Binds the specified mesh for rendering.
 *
 * This function binds the specified mesh for rendering.
 *
 * @param mesh A pointer to the mesh object to bind.
 */
void mesh_bind(const mesh_t *const mesh);

/**
 * @brief Unbinds the currently bound mesh.
 *
 * This function unbinds the currently bound mesh.
 *
 * @param mesh A pointer to the mesh object to unbind.
 */
void mesh_unbind(const mesh_t *const mesh);

/**
 * @brief Returns the number of vertices in the specified mesh.
 *
 * This function returns the number of vertices in the specified mesh object.
 *
 * @param mesh A pointer to the mesh object.
 *
 * @return size_t The number of vertices in the mesh.
 */
size_t mesh_get_vertex_count(const mesh_t *const mesh);

/**
 * @brief Returns the number of indices in the specified mesh.
 *
 * This function returns the number of indices in the specified mesh object.
 *
 * @param mesh A pointer to the mesh object.
 *
 * @return size_t The number of indices in the mesh.
 */
size_t mesh_get_index_count(const mesh_t *const mesh);
