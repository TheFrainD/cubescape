#pragma once

typedef enum {
    BLOCK_ID_AIR = 0,
    BLOCK_ID_STONE = 1,
    BLOCK_ID_COBBLESTONE = 2,
    BLOCK_ID_DIRT = 3,
    BLOCK_ID_GRASS = 4,
} block_id_t;

typedef enum {
    TILE_ID_NONE = -1,
    TILE_ID_STONE = 0,
    TILE_ID_COBBLESTONE = 1,
    TILE_ID_DIRT = 2,
    TILE_ID_GRASS_TOP = 3,
    TILE_ID_GRASS_SIDE = 4,
    TILE_ID_GRASS_BOTTOM = TILE_ID_DIRT,
} tile_id_t;

typedef struct {
    tile_id_t up;
    tile_id_t bottom;
    tile_id_t front;
    tile_id_t back;
    tile_id_t left;
    tile_id_t right;
} block_tiles_t;

int is_block_opaque(block_id_t block);

block_tiles_t get_block_tiles(block_id_t block);