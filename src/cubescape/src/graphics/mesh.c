#include "graphics/mesh.h"

#include <cubegl/buffer.h>
#include <cubegl/texture.h>
#include <cubegl/vertex_array.h>
#include <cubelog/cubelog.h>

#include "graphics/renderer.h"

struct mesh_private_data {
    size_t vertex_count;
    size_t index_count;

    uint32_t vertex_array;
    buffer_t *vertex_buffer;
    buffer_t *index_buffer;
};

mesh_t *mesh_create(const vertex_t *const vertices, size_t vertex_count, const uint32_t *const indices,
                    size_t index_count, shader_program_t *shader_program, uint32_t texture) {
    mesh_t *mesh         = malloc(sizeof(mesh_t));
    mesh->shader_program = shader_program;
    mesh->texture        = texture;

    mesh->private_data               = malloc(sizeof(mesh_private_data_t));
    mesh->private_data->vertex_count = vertex_count;
    mesh->private_data->index_count  = index_count;

    mesh->private_data->vertex_array = vertex_array_create();
    mesh->private_data->vertex_buffer =
        buffer_create(vertex_count * sizeof(vertex_t), vertices, BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    mesh->private_data->index_buffer = buffer_create(index_count * sizeof(uint32_t), indices, BUFFER_USAGE_DYNAMIC_DRAW,
                                                     BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    vertex_array_bind(mesh->private_data->vertex_array);
    buffer_bind(mesh->private_data->vertex_buffer);
    vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
    vertex_array_attrib(1, 2, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, uv));
    buffer_unbind(mesh->private_data->vertex_buffer);
    vertex_array_unbind();

    return mesh;
}

void mesh_destroy(mesh_t *mesh) {
    if (!mesh) {
        CUBELOG_TRACE("'mesh_destroy' called with NULL mesh");
        return;
    }

    CUBELOG_TRACE("Destroying mesh");

    buffer_destroy(mesh->private_data->vertex_buffer);
    buffer_destroy(mesh->private_data->index_buffer);
    vertex_array_destroy(&mesh->private_data->vertex_array);
    free(mesh->private_data);
    free(mesh);
}

void mesh_set_vertices(const mesh_t *mesh, const vertex_t *const vertices, size_t vertex_count) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_set_vertices' called with NULL mesh");
        return;
    }

    if (vertices == NULL) {
        CUBELOG_ERROR("'mesh_set_vertices' called with NULL vertices");
        return;
    }

    if (vertex_count != mesh->private_data->vertex_count) {
        buffer_data(mesh->private_data->vertex_buffer, vertex_count * sizeof(vertex_t), vertices);
    } else {
        buffer_sub_data(mesh->private_data->vertex_buffer, 0, vertex_count * sizeof(vertex_t), vertices);
    }

    mesh->private_data->vertex_count = vertex_count;
}

void mesh_set_indices(const mesh_t *mesh, const uint32_t *const indices, size_t index_count) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_set_indices' called with NULL mesh");
        return;
    }

    if (indices == NULL) {
        CUBELOG_ERROR("'mesh_set_indices' called with NULL indices");
        return;
    }

    if (index_count != mesh->private_data->index_count) {
        buffer_data(mesh->private_data->index_buffer, index_count * sizeof(uint32_t), indices);
    } else {
        buffer_sub_data(mesh->private_data->index_buffer, 0, index_count * sizeof(uint32_t), indices);
    }

    mesh->private_data->index_count = index_count;
}

void mesh_bind(const mesh_t *const mesh) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_bind' called with NULL mesh");
        return;
    }

    shader_program_use(mesh->shader_program);
    vertex_array_bind(mesh->private_data->vertex_array);
    buffer_bind(mesh->private_data->vertex_buffer);
    buffer_bind(mesh->private_data->index_buffer);
    texture_bind(mesh->texture, 0);
}

void mesh_unbind(const mesh_t *const mesh) {
    buffer_unbind(mesh->private_data->vertex_buffer);
    buffer_unbind(mesh->private_data->index_buffer);
    vertex_array_unbind();
}

size_t mesh_get_vertex_count(const mesh_t *const mesh) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_get_vertex_count' called with NULL mesh");
        return 0;
    }
    return mesh->private_data->vertex_count;
}

size_t mesh_get_index_count(const mesh_t *const mesh) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_get_index_count' called with NULL mesh");
        return 0;
    }
    return mesh->private_data->index_count;
}
