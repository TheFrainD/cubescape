#include "graphics/buffer.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "core/log.h"

Buffer *create_buffer(const void *data, size_t size, BufferUsage usage, BufferTarget target) {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (!buffer) {
        log_error("Failed to allocate memory for buffer");
        return NULL;
    }

    glGenBuffers(1, &buffer->id);
    glBindBuffer(target, buffer->id);
    glBufferData(target, size, data, usage);

    return buffer;
}

void destroy_buffer(Buffer *buffer) {
    if (!buffer) {
        log_error("Attempted to destroy NULL buffer");
        return;
    }

    glDeleteBuffers(1, &buffer->id);
    free(buffer);
}

void bind_buffer(Buffer *buffer, BufferTarget target) {
    if (!buffer) {
        log_error("Attempted to bind NULL buffer");
        return;
    }

    glBindBuffer(target, buffer->id);
    buffer->bound_target = target;
}

void buffer_data(Buffer *buffer, const void *data, size_t size, BufferUsage usage, BufferTarget target) {
    if (!buffer) {
        log_error("Attempted to buffer data to NULL buffer");
        return;
    }

    bind_buffer(buffer, target);
    glBufferData(target, size, data, usage);
}

void buffer_sub_data(Buffer *buffer, const void *data, size_t size, size_t offset, BufferTarget target) {
    if (!buffer) {
        log_error("Attempted to buffer sub data to NULL buffer");
        return;
    }

    bind_buffer(buffer, target);
    buffer->size = size;
    glBufferSubData(target, offset, size, data);
}

uint32_t buffer_get_handle(Buffer *buffer) {
    if (!buffer) {
        log_error("Attempted to get handle of NULL buffer");
        return 0;
    }

    return buffer->id;
}
