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
} VertexArrayDataType;

uint32_t create_vertex_array();
void destroy_vertex_array(uint32_t *vertex_array);

void bind_vertex_array(uint32_t vertex_array);
void unbind_vertex_array();

void vertex_array_attrib(uint32_t index, int size, VertexArrayDataType type, int stride, const void *pointer);