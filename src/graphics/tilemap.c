#include "graphics/tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/log.h"

int tilemap_load(const char *filename, tilemap_t *tilemap) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        LOG_ERROR("Failed to open file: %s", filename);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[16];
        if (sscanf(line, "%15s", key) != 1) {
            continue;
        }

        if (strcmp(key, "name") == 0) {
            char name[256];
            sscanf(line + strlen(key) + 1, "%255[^\n]", name);
            tilemap->name = strdup(name);
        } else if (strcmp(key, "tile") == 0) {
            sscanf(line + strlen(key) + 1, "%d", &tilemap->tile_size);
        } else if (strcmp(key, "size") == 0) {
            sscanf(line + strlen(key) + 1, "%d", &tilemap->map_size);
        } else if (strcmp(key, "path") == 0) {
            char path[256];
            sscanf(line + strlen(key) + 1, "%255[^\n]", path);
            tilemap->path = strdup(path);
        }
    }

    fclose(file);
    return 1;
}

void tilemap_free(tilemap_t *tilemap) {
    free(tilemap->name);
    free(tilemap->path);
}

tile_uv_t tilemap_get_tile_uv(tilemap_t *tilemap, int tile_id) {
    if (tilemap == NULL) {
        LOG_ERROR("'tilemap_get_tile_uv' called with NULL tilemap");
        return (tile_uv_t) {{0}};
    }

    if (tile_id < 0 || tile_id >= tilemap->map_size * tilemap->map_size) {
        LOG_ERROR("Invalid tile ID: %d", tile_id);
        return (tile_uv_t) {{0}};
    }

    float pixel_size = 1.0f / tilemap->map_size;
    int row_size     = tilemap->map_size / tilemap->tile_size;

    float u_min = ((tile_id % tilemap->tile_size) / (float)tilemap->tile_size) + pixel_size;
    float v_min = ((tile_id / row_size) / (float)tilemap->tile_size + pixel_size);
    float u_max = (((tile_id % tilemap->tile_size) + 1) / (float)tilemap->tile_size) - pixel_size;
    float v_max = (((tile_id / row_size) + 1) / (float)tilemap->tile_size) - pixel_size;

    tile_uv_t uv = {0};
    uv.value[0]  = (vec2s) {{u_min, v_max}};
    uv.value[1]  = (vec2s) {{u_max, v_max}};
    uv.value[2]  = (vec2s) {{u_max, v_min}};
    uv.value[3]  = (vec2s) {{u_min, v_min}};

    return uv;
}