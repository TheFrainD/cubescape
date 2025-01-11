#pragma once

typedef struct {
    char *name;
    int tile_size;
    int map_size;
    char *path;
} tilemap_t;

int load_tilemap(const char *filename, tilemap_t *tilemap);

void destroy_tilemap(tilemap_t *tilemap);