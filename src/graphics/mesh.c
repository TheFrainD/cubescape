#include "graphics/mesh.h"

#include "core/log.h"
#include "graphics/buffer.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

struct mesh_private_data {
    size_t vertex_count;
    size_t index_count;

    uint32_t vertex_buffer;
    uint32_t index_buffer;
};

mesh_t *mesh_create(vertex_t *vertices, size_t vertex_count, uint32_t *indices, size_t index_count,
                    shader_program_t *shader_program, uint32_t texture) {
    mesh_t *mesh         = malloc(sizeof(mesh_t));
    mesh->shader_program = shader_program;
    mesh->texture        = texture;

    mesh->private_data               = malloc(sizeof(mesh_private_data_t));
    mesh->private_data->vertex_count = vertex_count;
    mesh->private_data->index_count  = index_count;

    mesh->private_data->vertex_buffer =
        buffer_create(vertex_count * sizeof(vertex_t), vertices, BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    mesh->private_data->index_buffer = buffer_create(index_count * sizeof(uint32_t), indices, BUFFER_USAGE_DYNAMIC_DRAW,
                                                     BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    renderer_bind_vertex_buffer(mesh->private_data->vertex_buffer);

    return mesh;
}

void mesh_destroy(mesh_t *mesh) {
    if (!mesh) {
        LOG_TRACE("'mesh_destroy' called with NULL mesh");
        return;
    }

    LOG_TRACE("Destroying mesh");

    buffer_destroy(&mesh->private_data->vertex_buffer);
    buffer_destroy(&mesh->private_data->index_buffer);
    free(mesh->private_data);
    free(mesh);
}

void mesh_set_vertices(mesh_t *mesh, vertex_t *vertices, size_t vertex_count) {
    if (!mesh) {
        LOG_ERROR("'mesh_set_vertices' called with NULL mesh");
        return;
    }

    if (vertices == NULL) {
        LOG_ERROR("'mesh_set_vertices' called with NULL vertices");
        return;
    }

    if (vertex_count != mesh->private_data->vertex_count) {
        buffer_data(mesh->private_data->vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER, vertex_count * sizeof(vertex_t),
                    vertices, BUFFER_USAGE_DYNAMIC_DRAW);
    } else {
        buffer_sub_data(mesh->private_data->vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER, 0,
                        vertex_count * sizeof(vertex_t), vertices);
    }

    mesh->private_data->vertex_count = vertex_count;
}

void mesh_set_indices(mesh_t *mesh, uint32_t *indices, size_t index_count) {
    if (!mesh) {
        LOG_ERROR("'mesh_set_indices' called with NULL mesh");
        return;
    }

    if (indices == NULL) {
        LOG_ERROR("'mesh_set_indices' called with NULL indices");
        return;
    }

    if (index_count != mesh->private_data->index_count) {
        buffer_data(mesh->private_data->index_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER,
                    index_count * sizeof(uint32_t), indices, BUFFER_USAGE_DYNAMIC_DRAW);
    } else {
        buffer_sub_data(mesh->private_data->index_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER, 0,
                        index_count * sizeof(uint32_t), indices);
    }

    mesh->private_data->index_count = index_count;
}

void mesh_bind(mesh_t *mesh) {
    if (!mesh) {
        LOG_ERROR("'mesh_bind' called with NULL mesh");
        return;
    }

    shader_program_use(mesh->shader_program);
    buffer_bind(mesh->private_data->vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER);
    buffer_bind(mesh->private_data->index_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
    texture_bind(mesh->texture, 0);
}

void mesh_unbind() {
    buffer_unbind(BUFFER_TARGET_ARRAY_BUFFER);
    buffer_unbind(BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
}

size_t mesh_get_vertex_count(mesh_t *mesh) {
    if (!mesh) {
        LOG_ERROR("'mesh_get_vertex_count' called with NULL mesh");
        return 0;
    }
    return mesh->private_data->vertex_count;
}

size_t mesh_get_index_count(mesh_t *mesh) {
    if (!mesh) {
        LOG_ERROR("'mesh_get_index_count' called with NULL mesh");
        return 0;
    }
    return mesh->private_data->index_count;
}
