#include "graphics/vertex_array.h"

#include <glad/glad.h>

uint32_t create_vertex_array() {
    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);
    return vertex_array;
}

void destroy_vertex_array(uint32_t *vertex_array) {
    glDeleteVertexArrays(1, vertex_array);
}

void bind_vertex_array(uint32_t vertex_array) {
    glBindVertexArray(vertex_array);
}

void unbind_vertex_array() {
    glBindVertexArray(0);
}

void vertex_array_attrib(uint32_t index, int size, VertexArrayDataType type, int stride, const void *pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
}