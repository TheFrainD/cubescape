#include "world/block.h"

#define BLOCK_TILE_UNIFORM(BLOCK_ID, TILE_ID) \
    case BLOCK_ID: \
        tiles.up = TILE_ID; \
        tiles.bottom = TILE_ID; \
        tiles.front = TILE_ID; \
        tiles.back = TILE_ID; \
        tiles.left = TILE_ID; \
        tiles.right = TILE_ID; \
        break;

int block_is_opaque(block_id_t block) {
    return block != BLOCK_ID_AIR;
}

block_tiles_t block_get_tiles(block_id_t block) {
    block_tiles_t tiles = {0};

    switch (block) {
        case BLOCK_ID_AIR:
            break;
        BLOCK_TILE_UNIFORM(BLOCK_ID_STONE, TILE_ID_STONE);
        BLOCK_TILE_UNIFORM(BLOCK_ID_COBBLESTONE, TILE_ID_COBBLESTONE);
        BLOCK_TILE_UNIFORM(BLOCK_ID_DIRT, TILE_ID_DIRT);
        case BLOCK_ID_GRASS:
            tiles.up = TILE_ID_GRASS_TOP;
            tiles.bottom = TILE_ID_GRASS_BOTTOM;
            tiles.front = TILE_ID_GRASS_SIDE;
            tiles.back = TILE_ID_GRASS_SIDE;
            tiles.left = TILE_ID_GRASS_SIDE;
            tiles.right = TILE_ID_GRASS_SIDE;
            break;
    }

    return tiles;
}
