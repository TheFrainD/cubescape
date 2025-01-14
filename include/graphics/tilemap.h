#pragma once

#include <stdint.h>

#include <cglm/struct.h>

typedef struct {
    char *name;
    char *path;

    int tile_size;
    int map_size;

    uint32_t texture;
} tilemap_t;

typedef struct {
    vec2s value[4];
} tile_uv_t;

/**
 * @brief Loads a tilemap from a file.
 * 
 * This function reads a tilemap from the specified file and returns a tilemap_t object.
 * 
 * @param filename The path to the tilemap file to be loaded.
 * @return tilemap_t* The loaded tilemap.
 */
tilemap_t *tilemap_load(const char *filename);

/**
 * @brief Frees the memory allocated for the given tilemap_t object.
 * 
 * This function releases the memory allocated for the tilemap data.
 * The tilemap_t object itself is not freed, as it is not dynamically allocated.
 * 
 * @param tilemap A pointer to the tilemap_t object whose data is to be freed.
 */
void tilemap_free(tilemap_t *tilemap);

/**
 * @brief Retrieves the UV coordinates of the specified tile.
 * 
 * This function calculates the UV coordinates of the specified tile in the tilemap.
 * 
 * @param tilemap A pointer to the tilemap_t object containing the tilemap data.
 * @param tile_id The ID of the tile to retrieve the UV coordinates for.
 * 
 * @return tile_uv_t The UV coordinates of the specified tile.
 */
tile_uv_t tilemap_get_tile_uv(tilemap_t *tilemap, int tile_id);
