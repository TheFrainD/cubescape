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
} buffer_usage_t;

typedef enum {
    BUFFER_TARGET_ARRAY_BUFFER = 0x8892,
    BUFFER_TARGET_ELEMENT_ARRAY_BUFFER = 0x8893,
    BUFFER_TARGET_UNIFORM_BUFFER = 0x8A11
} buffer_target_t;

/**
 * @brief Creates a buffer with the specified size and data.
 *
 * @param size The size of the buffer to create.
 * @param data A pointer to the data to initialize the buffer with.
 * @param usage The intended usage pattern of the buffer.
 * @param target The target to which the buffer is bound.
 * @return The ID of the created buffer.
 */
uint32_t create_buffer(size_t size, const void *data, buffer_usage_t usage, buffer_target_t target);

/**
 * @brief Destroys the specified buffer.
 *
 * @param buffer A pointer to the ID of the buffer to destroy.
 */
void destroy_buffer(uint32_t *buffer);

/**
 * @brief Binds the specified buffer to the given target.
 *
 * @param buffer The ID of the buffer to bind.
 * @param target The target to which the buffer is bound.
 */
void bind_buffer(uint32_t buffer, buffer_target_t target);

/**
 * @brief Binds a range of the specified buffer to the given target.
 *
 * @param buffer The ID of the buffer to bind.
 * @param target The target to which the buffer is bound.
 * @param binding_point The binding point within the target.
 * @param size The size of the range to bind.
 */
void bind_buffer_range(uint32_t buffer, buffer_target_t target, const uint32_t binding_point, size_t size);

/**
 * @brief Binds the base of the specified buffer to the given target.
 *
 * @param buffer The ID of the buffer to bind.
 * @param target The target to which the buffer is bound.
 * @param binding_point The binding point within the target.
 */
void bind_buffer_base(uint32_t buffer, buffer_target_t target, const uint32_t binding_point);

/**
 * @brief Unbinds the buffer from the given target.
 *
 * @param target The target from which the buffer is unbound.
 */
void unbind_buffer(buffer_target_t target);

/**
 * @brief Updates the data of the specified buffer.
 *
 * @param buffer The ID of the buffer to update.
 * @param size The size of the data to update.
 * @param data A pointer to the data to update the buffer with.
 * @param usage The intended usage pattern of the buffer.
 */
void buffer_data(uint32_t buffer, size_t size, const void *data, buffer_usage_t usage);

/**
 * @brief Updates a sub-region of the specified buffer.
 *
 * @param buffer The ID of the buffer to update.
 * @param offset The offset within the buffer to start updating.
 * @param size The size of the data to update.
 * @param data A pointer to the data to update the buffer with.
 */
void buffer_sub_data(uint32_t buffer, size_t offset, size_t size, const void *data);
