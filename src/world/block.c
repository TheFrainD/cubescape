#include "world/block.h"

#define BLOCK_TILE_UNIFORM(BLOCK_ID, TILE_ID) \
    case BLOCK_ID:                            \
        for (int i = 0; i < 6; ++i) {         \
            tiles.values[i] = TILE_ID;        \
        }                                     \
        break;

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

block_faces_t block_get_faces(block_id_t block, vec3s position, tilemap_t *tilemap) {
    block_faces_t faces = {0};
    block_tiles_t tiles = block_get_tiles(block);

    // Top face
    tile_uv_t uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_TOP]);
    faces.values[BLOCK_FACE_TOP].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_TOP].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_TOP].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_TOP].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), uv.value[3]};

    // Bottom face
    uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_BOTTOM]);
    faces.values[BLOCK_FACE_BOTTOM].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_BOTTOM].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_BOTTOM].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_BOTTOM].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), uv.value[3]};

    // Front face
    uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_FRONT]);
    faces.values[BLOCK_FACE_FRONT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_FRONT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_FRONT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_FRONT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), uv.value[3]};

    // Back face
    uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_BACK]);
    faces.values[BLOCK_FACE_BACK].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_BACK].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_BACK].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_BACK].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), uv.value[3]};

    // Left face
    uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_LEFT]);
    faces.values[BLOCK_FACE_LEFT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 1.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_LEFT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 1.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_LEFT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 1.0f, 0.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_LEFT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{0.0f, 0.0f, 0.0f}}, position), uv.value[3]};

    // Right face
    uv = tilemap_get_tile_uv(tilemap, tiles.values[BLOCK_FACE_RIGHT]);
    faces.values[BLOCK_FACE_RIGHT].vertices[0] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 0.0f}}, position), uv.value[0]};
    faces.values[BLOCK_FACE_RIGHT].vertices[1] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 0.0f}}, position), uv.value[1]};
    faces.values[BLOCK_FACE_RIGHT].vertices[2] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 1.0f, 1.0f}}, position), uv.value[2]};
    faces.values[BLOCK_FACE_RIGHT].vertices[3] =
        (vertex_t) {glms_vec3_add((vec3s) {{1.0f, 0.0f, 1.0f}}, position), uv.value[3]};

    return faces;
}