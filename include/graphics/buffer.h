#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum {
    BUFFER_USAGE_STREAM_DRAW  = 0x88E0,
    BUFFER_USAGE_STREAM_READ  = 0x88E1,
    BUFFER_USAGE_STREAM_COPY  = 0x88E2,
    BUFFER_USAGE_STATIC_DRAW  = 0x88E4,
    BUFFER_USAGE_STATIC_READ  = 0x88E5,
    BUFFER_USAGE_STATIC_COPY  = 0x88E6,
    BUFFER_USAGE_DYNAMIC_DRAW = 0x88E8,
    BUFFER_USAGE_DYNAMIC_READ = 0x88E9,
    BUFFER_USAGE_DYNAMIC_COPY = 0x88EA
} BufferUsage;

typedef enum {
    BUFFER_TARGET_ARRAY_BUFFER = 0x8892,
    BUFFER_TARGET_ELEMENT_ARRAY_BUFFER = 0x8893,
    BUFFER_TARGET_UNIFORM_BUFFER = 0x8A11
} BufferTarget;

uint32_t create_buffer(const void *data, size_t size, BufferUsage usage, BufferTarget target);
void destroy_buffer(uint32_t *buffer);

void bind_buffer(uint32_t buffer, BufferTarget target);
void bind_buffer_range(uint32_t buffer, BufferTarget target, const uint32_t binding_point, size_t size);
void bind_buffer_base(uint32_t buffer, BufferTarget target, const uint32_t binding_point);

void unbind_buffer(BufferTarget target);

void buffer_data(uint32_t buffer, const void *data, size_t size, BufferUsage usage, BufferTarget target);
void buffer_sub_data(uint32_t buffer, const void *data, size_t size, size_t offset, BufferTarget target);
