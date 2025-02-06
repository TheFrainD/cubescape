#include "graphics/tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cJSON.h>

#include "core/file.h"
#include "core/log.h"

#include "gl/image.h"
#include "gl/texture.h"

tilemap_t *tilemap_load(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        LOG_ERROR("Failed to open file: %s", filename);
        return NULL;
    }

    size_t file_size = get_file_size(file);
    uint8_t *data    = (uint8_t *)malloc(file_size + 1);
    if (data == NULL) {
        LOG_ERROR("Failed to allocate memory for file data");
        fclose(file);
        return NULL;
    }

    read_file_content(file, (char *)data, file_size);

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (json == NULL) {
        LOG_ERROR("Failed to parse JSON data");
        return NULL;
    }

    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *tile = cJSON_GetObjectItem(json, "tile");
    cJSON *size = cJSON_GetObjectItem(json, "size");
    cJSON *path = cJSON_GetObjectItem(json, "path");

    tilemap_t *tilemap = malloc(sizeof(tilemap_t));
    if (tilemap == NULL) {
        LOG_ERROR("Failed to allocate memory for tilemap");
        cJSON_Delete(json);
        return NULL;
    }

    if (cJSON_IsString(name) && name->valuestring) {
        tilemap->name = strdup(name->valuestring);
    } else {
        LOG_ERROR("Failed to parse tilemap name");
        free(tilemap);
        cJSON_Delete(json);
        return NULL;
    }

    if (cJSON_IsNumber(tile)) {
        tilemap->tile_size = tile->valueint;
    } else {
        LOG_ERROR("Failed to parse tilemap tile size");
        free(tilemap);
        cJSON_Delete(json);
        return NULL;
    }

    if (cJSON_IsNumber(size)) {
        tilemap->map_size = size->valueint;
    } else {
        LOG_ERROR("Failed to parse tilemap size");
        free(tilemap);
        cJSON_Delete(json);
        return NULL;
    }

    if (cJSON_IsString(path) && path->valuestring) {
        tilemap->path = strdup(path->valuestring);
    } else {
        LOG_ERROR("Failed to parse tilemap path");
        free(tilemap->name);
        free(tilemap);
        cJSON_Delete(json);
        return NULL;
    }

    cJSON_Delete(json);

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
