#include "graphics/vertex_array.h"

#include <glad/glad.h>

#include "core/log.h"

uint32_t create_vertex_array() {
    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);
    LOG_TRACE("Created vertex array with ID: %d", vertex_array);
    return vertex_array;
}

void destroy_vertex_array(uint32_t *vertex_array) {
    LOG_TRACE("Deleting vertex array with ID: %d", *vertex_array);
    glDeleteVertexArrays(1, vertex_array);
}

void bind_vertex_array(uint32_t vertex_array) {
    glBindVertexArray(vertex_array);
}

void unbind_vertex_array() {
    glBindVertexArray(0);
}

void vertex_array_attrib(uint32_t index, int size, vertex_array_data_type_t type, int stride, const void *pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
}