#include "graphics/mesh.h"

#include <stdlib.h>
#include <string.h>

#include "core/assert.h"
#include "core/log.h"

#include "gl/texture.h"
#include "gl/vertex_array.h"

#include "graphics/renderer.h"

#define READY_TO_UPLOAD (mesh->vertices && mesh->indices && mesh->vertex_count > 0 && mesh->index_count > 0)

mesh_t *mesh_create(const vertex_t *vertices, size_t vertex_count, const uint32_t *indices, size_t index_count,
                    shader_program_t *shader_program, uint32_t texture) {
    mesh_t *mesh = malloc(sizeof(mesh_t));
    if (mesh == NULL) {
        LOG_ERROR("Failed to allocate memory for mesh");
        return NULL;
    }

    mesh->shader_program = shader_program;
    mesh->texture        = texture;
    mesh->flags.uploaded = CS_FALSE;

    mesh->vertex_buffer = NULL;
    mesh->index_buffer  = NULL;
    mesh->vertex_array  = 0;

    mesh->vertices = vertex_count != 0 ? malloc(vertex_count * sizeof(vertex_t)) : NULL;
    if (vertices != NULL && mesh->vertices != NULL) {
        memcpy(mesh->vertices, vertices, vertex_count * sizeof(vertex_t));
    }
    mesh->vertex_count = vertex_count;

    mesh->indices = index_count != 0 ? malloc(index_count * sizeof(uint32_t)) : NULL;
    if (indices != NULL && mesh->indices != NULL) {
        memcpy(mesh->indices, indices, index_count * sizeof(uint32_t));
    }
    mesh->index_count = index_count;

    mesh->flags.ready_to_upload = READY_TO_UPLOAD;

    return mesh;
}

void mesh_destroy(mesh_t *mesh) {
    if (!mesh) {
        LOG_TRACE("'mesh_destroy' called with NULL mesh");
        return;
    }

    if (mesh->vertices) {
        free(mesh->vertices);
    }
    if (mesh->indices) {
        free(mesh->indices);
    }
    if (mesh->vertex_buffer) {
        buffer_destroy(mesh->vertex_buffer);
    }
    if (mesh->index_buffer) {
        buffer_destroy(mesh->index_buffer);
    }
    if (mesh->vertex_array) {
        vertex_array_destroy(&mesh->vertex_array);
    }

    free(mesh);
}

void mesh_set_vertices(mesh_t *mesh, const vertex_t *vertices, size_t vertex_count) {
    if (!mesh) {
        LOG_ERROR("'mesh_set_vertices' called with NULL mesh");
        return;
    }

    if (vertices == NULL) {
        LOG_ERROR("'mesh_set_vertices' called with NULL vertices");
        return;
    }

    if (mesh->vertices == NULL) {
        mesh->vertices = malloc(vertex_count * sizeof(vertex_t));
    } else if (vertex_count > mesh->vertex_count) {
        vertex_t *new_vertices = realloc(mesh->vertices, vertex_count * sizeof(vertex_t));
        if (new_vertices == NULL) {
            LOG_ERROR("Failed to reallocate memory for vertices");
            return;
        }
        mesh->vertices = new_vertices;
    }

    mesh->vertex_count = vertex_count;

    ASSERT(mesh->vertices != NULL);
    memcpy(mesh->vertices, vertices, vertex_count * sizeof(vertex_t));

    mesh->flags.ready_to_upload = READY_TO_UPLOAD;
}

void mesh_set_indices(mesh_t *mesh, const uint32_t *indices, size_t index_count) {
    if (!mesh) {
        LOG_ERROR("'mesh_set_indices' called with NULL mesh");
        return;
    }

    if (indices == NULL) {
        LOG_ERROR("'mesh_set_indices' called with NULL indices");
        return;
    }

    if (mesh->indices == NULL) {
        mesh->indices = malloc(index_count * sizeof(uint32_t));
    } else if (index_count > mesh->index_count) {
        uint32_t *new_indices = realloc(mesh->indices, index_count * sizeof(uint32_t));
        if (new_indices == NULL) {
            LOG_ERROR("Failed to reallocate memory for indices");
            return;
        }
        mesh->indices = new_indices;
    }

    mesh->index_count = index_count;

    ASSERT(mesh->indices != NULL);
    memcpy(mesh->indices, indices, index_count * sizeof(uint32_t));

    mesh->flags.ready_to_upload = READY_TO_UPLOAD;
}

void mesh_bind(const mesh_t *mesh) {
    if (!mesh) {
        LOG_ERROR("'mesh_bind' called with NULL mesh");
        return;
    }

    shader_program_use(mesh->shader_program);
    vertex_array_bind(mesh->vertex_array);
    buffer_bind(mesh->vertex_buffer);
    buffer_bind(mesh->index_buffer);
    texture_bind(mesh->texture, 0);
}

void mesh_unbind(const mesh_t *mesh) {
    buffer_unbind(mesh->vertex_buffer);
    buffer_unbind(mesh->index_buffer);
    vertex_array_unbind();
}

void mesh_upload(mesh_t *mesh) {
    if (!mesh) {
        LOG_ERROR("'mesh_upload' called with NULL mesh");
        return;
    }

    if (mesh->vertex_buffer) {
        buffer_data(mesh->vertex_buffer, mesh->vertex_count * sizeof(vertex_t), mesh->vertices);
    } else {
        mesh->vertex_buffer = buffer_create(mesh->vertex_count * sizeof(vertex_t), mesh->vertices,
                                            BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    }

    if (mesh->index_buffer) {
        buffer_data(mesh->index_buffer, mesh->index_count * sizeof(uint32_t), mesh->indices);
    } else {
        mesh->index_buffer = buffer_create(mesh->index_count * sizeof(uint32_t), mesh->indices,
                                           BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
    }

    if (mesh->vertex_array == 0) {
        mesh->vertex_array = vertex_array_create();

        vertex_array_bind(mesh->vertex_array);
        buffer_bind(mesh->vertex_buffer);
        vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
        vertex_array_attrib(1, 2, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, uv));
        vertex_array_attrib(2, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, tint));
        buffer_unbind(mesh->vertex_buffer);
        vertex_array_unbind();
    }

    mesh->flags.uploaded        = CS_TRUE;
    mesh->flags.ready_to_upload = CS_FALSE;
}