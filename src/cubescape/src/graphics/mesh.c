#include "graphics/mesh.h"

#include <cubegl/texture.h>
#include <cubegl/vertex_array.h>
#include <cubelog/cubelog.h>

#include "graphics/renderer.h"

mesh_t *mesh_create(vertex_t *vertices, size_t vertex_count, uint32_t *indices, size_t index_count,
                    shader_program_t *shader_program, uint32_t texture) {
    mesh_t *mesh         = malloc(sizeof(mesh_t));
    mesh->shader_program = shader_program;
    mesh->texture        = texture;
    mesh->state          = MESH_STATE_UNINITIALIZED;

    mesh->vertices     = malloc(vertex_count * sizeof(vertex_t));
    mesh->vertex_count = vertex_count;

    mesh->indices     = malloc(index_count * sizeof(uint32_t));
    mesh->index_count = index_count;

    mesh->vertex_array  = 0;
    mesh->vertex_buffer = NULL;
    mesh->index_buffer  = NULL;

    if (vertices && indices && vertex_count > 0 && index_count > 0) {
        mesh->state = MESH_STATE_READY_TO_UPLOAD;
    }

    return mesh;
}

void mesh_destroy(mesh_t *mesh) {
    if (!mesh) {
        CUBELOG_TRACE("'mesh_destroy' called with NULL mesh");
        return;
    }

    CUBELOG_TRACE("Destroying mesh");

    buffer_destroy(mesh->vertex_buffer);
    buffer_destroy(mesh->index_buffer);
    vertex_array_destroy(&mesh->vertex_array);
    free(mesh);
}

void mesh_set_vertices(mesh_t *mesh, vertex_t *vertices, size_t vertex_count) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_set_vertices' called with NULL mesh");
        return;
    }

    if (vertices == NULL) {
        CUBELOG_ERROR("'mesh_set_vertices' called with NULL vertices");
        return;
    }

    mesh->vertices     = vertices;
    mesh->vertex_count = vertex_count;

    if (mesh->vertices && mesh->indices && mesh->vertex_count > 0 && mesh->index_count > 0) {
        mesh->state = MESH_STATE_READY_TO_UPLOAD;
    }
}

void mesh_set_indices(mesh_t *mesh, uint32_t *indices, size_t index_count) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_set_indices' called with NULL mesh");
        return;
    }

    if (indices == NULL) {
        CUBELOG_ERROR("'mesh_set_indices' called with NULL indices");
        return;
    }

    mesh->indices     = indices;
    mesh->index_count = index_count;

    if (mesh->vertices && mesh->indices && mesh->vertex_count > 0 && mesh->index_count > 0) {
        mesh->state = MESH_STATE_READY_TO_UPLOAD;
    }
}

void mesh_upload(mesh_t *mesh) {
    if (mesh == NULL) {
        CUBELOG_ERROR("'mesh_upload' called with NULL mesh");
        return;
    }

    if (mesh->vertex_buffer == NULL) {
        mesh->vertex_buffer = buffer_create(mesh->vertex_count * sizeof(vertex_t), mesh->vertices,
                                            BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    } else {
        buffer_data(mesh->vertex_buffer, mesh->vertex_count * sizeof(vertex_t), mesh->vertices);
    }

    if (mesh->index_buffer == NULL) {
        mesh->index_buffer = buffer_create(mesh->index_count * sizeof(uint32_t), mesh->indices,
                                           BUFFER_USAGE_DYNAMIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);
    } else {
        buffer_data(mesh->index_buffer, mesh->index_count * sizeof(uint32_t), mesh->indices);
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

    mesh->state = MESH_STATE_UPLOADED;
}

void mesh_bind(mesh_t *mesh) {
    if (!mesh) {
        CUBELOG_ERROR("'mesh_bind' called with NULL mesh");
        return;
    }

    shader_program_use(mesh->shader_program);
    vertex_array_bind(mesh->vertex_array);
    buffer_bind(mesh->vertex_buffer);
    buffer_bind(mesh->index_buffer);
    texture_bind(mesh->texture, 0);
}

void mesh_unbind(mesh_t *mesh) {
    buffer_unbind(mesh->vertex_buffer);
    buffer_unbind(mesh->index_buffer);
    vertex_array_unbind();
}
