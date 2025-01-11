#pragma once

typedef struct {
    char *name;
    int tile_size;
    int map_size;
    char *path;
} tilemap_t;

/**
 * @brief Loads a tilemap from a file.
 * 
 * This function reads a tilemap from the specified file and returns a tilemap_t object.
 * 
 * @param filename The path to the tilemap file to be loaded.
 * @param tilemap A pointer to the tilemap_t object to store the loaded tilemap.
 * @return 1 if the tilemap was loaded successfully, 0 otherwise.
 */
int tilemap_load(const char *filename, tilemap_t *tilemap);

/**
 * @brief Frees the memory allocated for the given tilemap_t object.
 * 
 * This function releases the memory allocated for the tilemap data.
 * The tilemap_t object itself is not freed, as it is not dynamically allocated.
 * 
 * @param tilemap A pointer to the tilemap_t object whose data is to be freed.
 */
void tilemap_free(tilemap_t *tilemap);
