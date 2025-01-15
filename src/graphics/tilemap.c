#include "graphics/tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/log.h"
#include "graphics/image.h"
#include "graphics/texture.h"

tilemap_t *tilemap_load(const char *filename) {
    tilemap_t *tilemap = malloc(sizeof(tilemap_t));

    FILE *file = fopen(filename, "r");
    if (!file) {
        LOG_ERROR("Failed to open file: %s", filename);
        return NULL;
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

    tilemap->texture = texture_create();

    image_t *image = image_load(tilemap->path);
    texture_set_image(tilemap->texture, image);
    image_free(image);

    texture_set_filtering(tilemap->texture, TEXTURE_FILTERING_NEAREST, TEXTURE_FILTERING_NEAREST);
    texture_set_wrapping(tilemap->texture, TEXTURE_WRAPPING_REPEAT, TEXTURE_WRAPPING_REPEAT);

    return tilemap;
}

void tilemap_free(tilemap_t *tilemap) {
    if (tilemap == NULL) {
        LOG_ERROR("'tilemap_free' called with NULL tilemap");
        return;
    }

    free(tilemap->name);
    free(tilemap->path);
    texture_destroy(&tilemap->texture);
    free(tilemap);
}
