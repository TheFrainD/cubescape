#include "graphics/buffer.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"

#define BUFFER_TARGET_ARRAY_SIZE 128

static BufferTarget g_buffer_target_map[BUFFER_TARGET_ARRAY_SIZE] = {0};

const char* buffer_target_to_string(BufferTarget target) {
    switch (target) {
        case GL_ARRAY_BUFFER: return "Array Buffer";
        case GL_ELEMENT_ARRAY_BUFFER: return "Element Array Buffer";
        case GL_UNIFORM_BUFFER: return "Uniform Buffer";
        default: return "Unknown Buffer Target";
    }
}

uint32_t create_buffer(size_t size, const void *data, BufferUsage usage, BufferTarget target) {
    uint32_t buffer;

    glGenBuffers(1, &buffer);
    g_buffer_target_map[buffer] = target;
    buffer_data(buffer, size, data, usage);

    LOG_TRACE("Created buffer with ID: %d", buffer);

    return buffer;
}

void destroy_buffer(uint32_t *buffer) {
    LOG_TRACE("Deleting buffer with ID: %d", *buffer);
    glDeleteBuffers(1, buffer);
    g_buffer_target_map[*buffer] = 0;
}

void bind_buffer(uint32_t buffer, BufferTarget target) {
    glBindBuffer(target, buffer);
    g_buffer_target_map[buffer] = target;
}

void bind_buffer_range(uint32_t buffer, BufferTarget target, const uint32_t binding_point, size_t size) {
    glBindBufferRange(target, binding_point, buffer, 0, size);
    g_buffer_target_map[buffer] = target;
}

void bind_buffer_base(uint32_t buffer, BufferTarget target, const uint32_t binding_point) {
    glBindBufferBase(target, binding_point, buffer);
    g_buffer_target_map[buffer] = target;
}

void unbind_buffer(BufferTarget target) {
    glBindBuffer(target, 0);
}

void buffer_data(uint32_t buffer, size_t size, const void *data, BufferUsage usage) {
    BufferTarget target = g_buffer_target_map[buffer];
    bind_buffer(buffer, target);
    glBufferData(target, size, data, usage);
    unbind_buffer(target);
}

void buffer_sub_data(uint32_t buffer, size_t offset, size_t size, const void *data) {
    BufferTarget target = g_buffer_target_map[buffer];
    bind_buffer(buffer, target);
    glBufferSubData(target, offset, size, data);
    unbind_buffer(target);
}
