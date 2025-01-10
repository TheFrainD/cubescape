#pragma once

#include <stdint.h>
#include <stddef.h>

#include "world/block.h"
#include "graphics/shader_program.h"
#include "graphics/tilemap.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT)

typedef struct {
    int x;
    int y;
    BlockId *blocks;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
    uint32_t index_buffer;
    size_t vertex_count;
    TileMap *tilemap;
} Chunk;

Chunk *create_chunk(int x, int y, TileMap *tilemap);

BlockId get_block(Chunk *chunk, int x, int y, int z);

void chunk_set_block(Chunk *chunk, int x, int y, int z, BlockId block);

void chunk_generate_mesh(Chunk *chunk);

void chunk_render(Chunk *chunk, ShaderProgram *shader_program);

void destroy_chunk(Chunk *chunk);