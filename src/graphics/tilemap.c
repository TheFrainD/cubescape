#include "graphics/tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tilemap_load(const char *filename, tilemap_t *tilemap) {
    FILE *file = fopen(filename, "r");
    if (!file) {
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
