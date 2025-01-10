#pragma once

typedef enum {
    BLOCK_ID_AIR = 0,
    BLOCK_ID_STONE = 1,
    BLOCK_ID_COBBLESTONE = 2,
    BLOCK_ID_DIRT = 3,
    BLOCK_ID_GRASS = 4,
} BlockId;

typedef enum {
    TILE_ID_NONE = -1,
    TILE_ID_STONE = 0,
    TILE_ID_COBBLESTONE = 1,
    TILE_ID_DIRT = 2,
    TILE_ID_GRASS_TOP = 3,
    TILE_ID_GRASS_SIDE = 4,
    TILE_ID_GRASS_BOTTOM = TILE_ID_DIRT,
} TileId;

typedef struct {
    TileId up;
    TileId bottom;
    TileId front;
    TileId back;
    TileId left;
    TileId right;
} BlockTiles;

int is_block_opaque(BlockId block);

BlockTiles get_block_tiles(BlockId block);