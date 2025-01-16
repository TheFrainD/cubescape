#include "world/block.h"

#include <cubelog/cubelog.h>

#define BLOCK_TILE_UNIFORM(BLOCK_ID, TILE_ID) \
    case BLOCK_ID:                            \
        for (int i = 0; i < 6; ++i) {         \
            tiles.values[i] = TILE_ID;        \
        }                                     \
        break;

#define U_MIN (((tile_id % tilemap->tile_size) / (float)tilemap->tile_size) + pixel_size)
#define V_MIN ((tile_id / row_size) / (float)tilemap->tile_size + pixel_size)
#define U_MAX ((((tile_id % tilemap->tile_size) + 1) / (float)tilemap->tile_size) - pixel_size)
#define V_MAX ((((tile_id / row_size) + 1) / (float)tilemap->tile_size) - pixel_size)

int block_is_opaque(block_id_t block) { return block != BLOCK_ID_AIR; }

block_tiles_t block_get_tiles(block_id_t block) {
    block_tiles_t tiles = {0};

    switch (block) {
        case BLOCK_ID_AIR:
            break;
            BLOCK_TILE_UNIFORM(BLOCK_ID_STONE, TILE_ID_STONE);
            BLOCK_TILE_UNIFORM(BLOCK_ID_COBBLESTONE, TILE_ID_COBBLESTONE);
            BLOCK_TILE_UNIFORM(BLOCK_ID_DIRT, TILE_ID_DIRT);
        case BLOCK_ID_GRASS:
            tiles.values[BLOCK_FACE_TOP]    = TILE_ID_GRASS_TOP;
            tiles.values[BLOCK_FACE_BOTTOM] = TILE_ID_GRASS_BOTTOM;
            for (int i = 2; i < 6; ++i) {
                tiles.values[i] = TILE_ID_GRASS_SIDE;
            }
            break;
    }

    return tiles;
}

void block_get_faces(block_id_t block, vec3s position, tilemap_t *tilemap, block_faces_t *faces) {
    if (tilemap == NULL) {
        CUBELOG_ERROR("'block_get_faces' called with NULL tilemap");
        return;
    }

    if (faces == NULL) {
        CUBELOG_ERROR("'block_get_faces' called with NULL faces");
        return;
    }

    block_tiles_t tiles = block_get_tiles(block);

    // Tilemap parameters
    float pixel_size = 1.0f / tilemap->map_size;
    int row_size     = tilemap->map_size / tilemap->tile_size;
    tile_id_t tile_id;

    // Top face
    tile_id = tiles.values[BLOCK_FACE_TOP];
    faces->values[BLOCK_FACE_TOP].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{1.0f, 1.0f, 1.0f}}};
    faces->values[BLOCK_FACE_TOP].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{1.0f, 1.0f, 1.0f}}};
    faces->values[BLOCK_FACE_TOP].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{1.0f, 1.0f, 1.0f}}};
    faces->values[BLOCK_FACE_TOP].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{1.0f, 1.0f, 1.0f}}};

    // Bottom face
    tile_id = tiles.values[BLOCK_FACE_BOTTOM];
    faces->values[BLOCK_FACE_BOTTOM].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{0.5f, 0.5f, 0.5f}}};
    faces->values[BLOCK_FACE_BOTTOM].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{0.5f, 0.5f, 0.5f}}};
    faces->values[BLOCK_FACE_BOTTOM].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{0.5f, 0.5f, 0.5f}}};
    faces->values[BLOCK_FACE_BOTTOM].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{0.5f, 0.5f, 0.5f}}};

    // Front face
    tile_id = tiles.values[BLOCK_FACE_FRONT];
    faces->values[BLOCK_FACE_FRONT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_FRONT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_FRONT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_FRONT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{0.65f, 0.65f, 0.65f}}};

    // Back face
    tile_id = tiles.values[BLOCK_FACE_BACK];
    faces->values[BLOCK_FACE_BACK].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_BACK].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_BACK].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{0.65f, 0.65f, 0.65f}}};
    faces->values[BLOCK_FACE_BACK].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{0.65f, 0.65f, 0.65f}}};

    // Left face
    tile_id = tiles.values[BLOCK_FACE_LEFT];
    faces->values[BLOCK_FACE_LEFT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_LEFT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_LEFT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_LEFT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{0.8f, 0.8f, 0.8f}}};

    // Right face
    tile_id = tiles.values[BLOCK_FACE_RIGHT];
    faces->values[BLOCK_FACE_RIGHT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MAX}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_RIGHT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), (vec2s) {{U_MIN, V_MIN}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_RIGHT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MIN}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
    faces->values[BLOCK_FACE_RIGHT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), (vec2s) {{U_MAX, V_MAX}}, (vec3s){{0.8f, 0.8f, 0.8f}}};
}