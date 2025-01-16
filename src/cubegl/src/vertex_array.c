#include "vertex_array.h"

#include <cubelog/cubelog.h>
#include <glad/glad.h>

uint32_t vertex_array_create() {
    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);
    CUBELOG_TRACE("Created vertex array with ID: %d", vertex_array);
    return vertex_array;
}

void vertex_array_destroy(uint32_t *vertex_array) {
    if (vertex_array == NULL) {
        CUBELOG_ERROR("'vertex_array_destroy' called with NULL vertex array");
        return;
    }

    if (*vertex_array == 0) {
        CUBELOG_ERROR("'vertex_array_destroy' called with 0 vertex array");
        return;
    }

    CUBELOG_TRACE("Deleting vertex array with ID: %d", *vertex_array);
    glDeleteVertexArrays(1, vertex_array);
}

void vertex_array_bind(uint32_t vertex_array) {
    if (vertex_array == 0) {
        CUBELOG_ERROR("'vertex_array_bind' called with 0 vertex array");
        return;
    }

    glBindVertexArray(vertex_array);
}

void vertex_array_unbind() { glBindVertexArray(0); }

void vertex_array_attrib(uint32_t index, int size, vertex_array_data_type_t type, int stride, const void *pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
}
