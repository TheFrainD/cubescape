#pragma once

#include <cglm/struct.h>

#include "graphics/tilemap.h"
#include "graphics/vertex.h"

typedef enum {
    BLOCK_ID_AIR         = 0,
    BLOCK_ID_STONE       = 1,
    BLOCK_ID_COBBLESTONE = 2,
    BLOCK_ID_DIRT        = 3,
    BLOCK_ID_GRASS       = 4,
} block_id_t;

typedef enum {
    TILE_ID_NONE         = -1,
    TILE_ID_STONE        = 0,
    TILE_ID_COBBLESTONE  = 1,
    TILE_ID_DIRT         = 2,
    TILE_ID_GRASS_TOP    = 3,
    TILE_ID_GRASS_SIDE   = 4,
    TILE_ID_GRASS_BOTTOM = TILE_ID_DIRT,
} tile_id_t;

typedef enum {
    BLOCK_FACE_TOP    = 0,
    BLOCK_FACE_BOTTOM = 1,
    BLOCK_FACE_FRONT  = 2,
    BLOCK_FACE_BACK   = 3,
    BLOCK_FACE_LEFT   = 4,
    BLOCK_FACE_RIGHT  = 5,
} block_face_t;

typedef struct {
    vertex_t vertices[4];
} block_face_data_t;

typedef struct {
    block_face_data_t values[6];
} block_faces_t;

typedef struct {
    tile_id_t values[6];
} block_tiles_t;

/**
 * @brief Checks if a block is opaque.
 *
 * @param block The block to check.
 *
 * @return int Returns 1 if the block is opaque, 0 otherwise.
 */
int block_is_opaque(block_id_t block);

/**
 * @brief Retrieves the tiles for a block.
 *
 * @param block The block to retrieve the tiles for.
 *
 * @return block_tiles_t The tiles for the block.
 */
block_tiles_t block_get_tiles(block_id_t block);

/**
 * @brief Retrieves the faces for a block.
 * 
 * This function calculates the faces for the specified block at the given position.
 * 
 * @param block The block to retrieve the faces for.
 * 
 * @return block_faces_t The faces for the block.
 */
block_faces_t block_get_faces(block_id_t block, vec3s position, tilemap_t *tilemap);
