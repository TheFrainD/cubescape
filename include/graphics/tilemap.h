#pragma once

typedef struct {
    char *name;
    int tile_size;
    int map_size;
    char *path;
} TileMap;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_tilemap(const char *filename, TileMap *tilemap);

void destroy_tilemap(TileMap *tilemap);