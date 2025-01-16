#include "buffer.h"

#include <cubelog/cubelog.h>
#include <glad/glad.h>
#include <stdlib.h>

const char *buffer_target_to_string(buffer_target_t target) {
    switch (target) {
        case GL_ARRAY_BUFFER:
            return "Array Buffer";
        case GL_ELEMENT_ARRAY_BUFFER:
            return "Element Array Buffer";
        case GL_UNIFORM_BUFFER:
            return "Uniform Buffer";
        default:
            return "Unknown Buffer Target";
    }
}

#define BUFFER_TARGET_COUNT 3

static uint32_t bound_targets[BUFFER_TARGET_COUNT] = {0};

static int target_to_id(buffer_target_t target) {
    switch (target) {
        case BUFFER_TARGET_ARRAY_BUFFER:
            return 0;
        case BUFFER_TARGET_ELEMENT_ARRAY_BUFFER:
            return 1;
        case BUFFER_TARGET_UNIFORM_BUFFER:
            return 2;
    }
}

buffer_t *buffer_create(size_t size, const void *data, buffer_usage_t usage, buffer_target_t target) {
    buffer_t *buffer = malloc(sizeof(buffer_t));
    buffer->usage    = usage;
    buffer->target   = target;
    buffer->size     = size;

    glGenBuffers(1, &buffer->id);
    buffer_data(buffer, size, data);

    CUBELOG_TRACE("Created buffer with ID: %d", buffer);

    return buffer;
}

void buffer_destroy(buffer_t *buffer) {
    if (buffer == NULL) {
        CUBELOG_ERROR("'buffer_destroy' called with NULL buffer");
        return;
    }

    if (buffer->id == 0) {
        CUBELOG_WARN("'buffer_destroy' called with 0 buffer ID");
        return;
    }

    CUBELOG_TRACE("Deleting buffer with ID: %d", *buffer);
    glDeleteBuffers(1, &buffer->id);
    free(buffer);
}

void buffer_bind(const buffer_t *const buffer) {
    if (buffer == 0) {
        CUBELOG_WARN("'buffer_bind' called with 0 buffer");
        return;
    }

    glBindBuffer(buffer->target, buffer->id);
    bound_targets[target_to_id(buffer->target)] = buffer->id;
}

void buffer_bind_range(const buffer_t *const buffer, const uint32_t binding_point, size_t size) {
    if (buffer == 0) {
        CUBELOG_WARN("'buffer_bind_range' called with 0 buffer");
        return;
    }

    if (size > buffer->size) {
        CUBELOG_WARN("'buffer_bind_range' called with invalid size");
        return;
    }

    glBindBufferRange(buffer->target, binding_point, buffer->id, 0, size);
    bound_targets[target_to_id(buffer->target)] = buffer->id;
}

void buffer_bind_base(const buffer_t *const buffer, const uint32_t binding_point) {
    if (buffer == 0) {
        CUBELOG_WARN("'buffer_bind_base' called with 0 buffer");
        return;
    }

    glBindBufferBase(buffer->target, binding_point, buffer->id);
    bound_targets[target_to_id(buffer->target)] = buffer->id;
}

void buffer_unbind(const buffer_t *const buffer) {
    glBindBuffer(buffer->target, 0);
    bound_targets[target_to_id(buffer->target)] = 0;
}

void buffer_data(const buffer_t *buffer, size_t size, const void *data) {
    if (buffer == NULL) {
        CUBELOG_WARN("'buffer_data' called with NULL buffer");
        return;
    }

    uint32_t previous_buffer = bound_targets[target_to_id(buffer->target)];

    buffer_bind(buffer);
    glBufferData(buffer->target, size, data, buffer->usage);
    buffer_unbind(buffer);

    if (previous_buffer != 0) {
        glBindBuffer(buffer->target, previous_buffer);
    }
}

void buffer_sub_data(const buffer_t *buffer, size_t offset, size_t size, const void *data) {
    if (buffer == NULL) {
        CUBELOG_WARN("'buffer_sub_data' called with NULL buffer");
        return;
    }

    if (offset + size > buffer->size) {
        CUBELOG_WARN("'buffer_sub_data' called with invalid offset and size");
        return;
    }

    uint32_t previous_buffer = bound_targets[target_to_id(buffer->target)];

    buffer_bind(buffer);
    glBufferSubData(buffer->target, offset, size, data);
    buffer_unbind(buffer);

    if (previous_buffer != 0) {
        glBindBuffer(buffer->target, previous_buffer);
    }
}
