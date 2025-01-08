#include "graphics/buffer.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"

uint32_t create_buffer(const void *data, size_t size, BufferUsage usage, BufferTarget target) {
    uint32_t buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, usage);

    return buffer;
}

void destroy_buffer(uint32_t *buffer) {
    glDeleteBuffers(1, buffer);
}

void bind_buffer(uint32_t buffer, BufferTarget target) {
    glBindBuffer(target, buffer);
}

void bind_buffer_range(uint32_t buffer, BufferTarget target, const uint32_t binding_point, size_t size) {
    glBindBufferRange(target, binding_point, buffer, 0, size);
}

void bind_buffer_base(uint32_t buffer, BufferTarget target, const uint32_t binding_point) {
    glBindBufferBase(target, binding_point, buffer);
}

void unbind_buffer(BufferTarget target) {
    glBindBuffer(target, 0);
}

void buffer_data(uint32_t buffer, const void *data, size_t size, BufferUsage usage, BufferTarget target) {
    bind_buffer(buffer, target);
    glBufferData(target, size, data, usage);
}

void buffer_sub_data(uint32_t buffer, const void *data, size_t size, size_t offset, BufferTarget target) {
    bind_buffer(buffer, target);
    glBufferSubData(target, offset, size, data);
}
