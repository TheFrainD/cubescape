#pragma once

#include <stdint.h>

typedef enum {
    VERTEX_ARRAY_DATA_TYPE_BYTE = 0x1400,
    VERTEX_ARRAY_DATA_TYPE_UNSIGNED_BYTE = 0x1401,
    VERTEX_ARRAY_DATA_TYPE_SHORT = 0x1402,
    VERTEX_ARRAY_DATA_TYPE_UNSIGNED_SHORT = 0x1403,
    VERTEX_ARRAY_DATA_TYPE_INT = 0x1404,
    VERTEX_ARRAY_DATA_TYPE_UNSIGNED_INT = 0x1405,
    VERTEX_ARRAY_DATA_TYPE_FLOAT = 0x1406,
    VERTEX_ARRAY_DATA_TYPE_DOUBLE = 0x140A
} vertex_array_data_type_t;

/**
 * @brief Creates a new vertex array object.
 * 
 * @return The ID of the created vertex array object.
 */
uint32_t create_vertex_array();

/**
 * @brief Destroys the specified vertex array object.
 * 
 * @param vertex_array Pointer to the ID of the vertex array object to be destroyed.
 */
void destroy_vertex_array(uint32_t *vertex_array);

/**
 * @brief Binds the specified vertex array object.
 * 
 * @param vertex_array The ID of the vertex array object to bind.
 */
void bind_vertex_array(uint32_t vertex_array);

/**
 * @brief Unbinds the currently bound vertex array object.
 */
void unbind_vertex_array();

/**
 * @brief Specifies the format of the vertex attribute data.
 * 
 * @param index The index of the vertex attribute.
 * @param size The number of components per vertex attribute.
 * @param type The data type of each component in the array.
 * @param stride The byte offset between consecutive vertex attributes.
 * @param pointer A pointer to the first component of the first vertex attribute in the array.
 */
void vertex_array_attrib(uint32_t index, int size, vertex_array_data_type_t type, int stride, const void *pointer);