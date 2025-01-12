#include "graphics/buffer.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"

#define BUFFER_TARGET_ARRAY_SIZE 128

static buffer_target_t g_buffer_target_map[BUFFER_TARGET_ARRAY_SIZE] = {0};

const char* buffer_target_to_string(buffer_target_t target) {
    switch (target) {
        case GL_ARRAY_BUFFER: return "Array Buffer";
        case GL_ELEMENT_ARRAY_BUFFER: return "Element Array Buffer";
        case GL_UNIFORM_BUFFER: return "Uniform Buffer";
        default: return "Unknown Buffer Target";
    }
}

uint32_t buffer_create(size_t size, const void *data, buffer_usage_t usage, buffer_target_t target) {
    uint32_t buffer;

    glGenBuffers(1, &buffer);
    g_buffer_target_map[buffer] = target;
    buffer_data(buffer, size, data, usage);

    LOG_TRACE("Created buffer with ID: %d", buffer);

    return buffer;
}

void buffer_destroy(uint32_t *buffer) {
    if (buffer == NULL) {
        LOG_ERROR("'buffer_destroy' called with NULL buffer");
        return;
    }

    if (*buffer == 0) {
        LOG_WARN("'buffer_destroy' called with 0 buffer");
        return;
    }

    LOG_TRACE("Deleting buffer with ID: %d", *buffer);
    glDeleteBuffers(1, buffer);
    g_buffer_target_map[*buffer] = 0;
}

void buffer_bind(uint32_t buffer, buffer_target_t target) {
    if (buffer == 0) {
        LOG_WARN("'buffer_bind' called with 0 buffer");
        return;
    }

    glBindBuffer(target, buffer);
    g_buffer_target_map[buffer] = target;
}

void buffer_bind_range(uint32_t buffer, buffer_target_t target, const uint32_t binding_point, size_t size) {
    if (buffer == 0) {
        LOG_WARN("'buffer_bind_range' called with 0 buffer");
        return;
    }

    glBindBufferRange(target, binding_point, buffer, 0, size);
    g_buffer_target_map[buffer] = target;
}

void buffer_bind_base(uint32_t buffer, buffer_target_t target, const uint32_t binding_point) {
    if (buffer == 0) {
        LOG_WARN("'buffer_bind_base' called with 0 buffer");
        return;
    }

    glBindBufferBase(target, binding_point, buffer);
    g_buffer_target_map[buffer] = target;
}

void buffer_unbind(buffer_target_t target) {
    glBindBuffer(target, 0);
}

void buffer_data(uint32_t buffer, size_t size, const void *data, buffer_usage_t usage) {
    if (buffer == 0) {
        LOG_WARN("'buffer_data' called with 0 buffer");
        return;
    }

    buffer_target_t target = g_buffer_target_map[buffer];
    buffer_bind(buffer, target);
    glBufferData(target, size, data, usage);
    buffer_unbind(target);
}

void buffer_sub_data(uint32_t buffer, size_t offset, size_t size, const void *data) {
    if (buffer == 0) {
        LOG_WARN("'buffer_sub_data' called with 0 buffer");
        return;
    }

    buffer_target_t target = g_buffer_target_map[buffer];
    buffer_bind(buffer, target);
    glBufferSubData(target, offset, size, data);
    buffer_unbind(target);
}
