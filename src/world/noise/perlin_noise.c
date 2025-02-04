#include "world/noise/perlin_noise.h"

#include <stdlib.h>

#define FNL_IMPL
#include <FastNoiseLite.h>

#include "core/assert.h"
#include "core/log.h"

perlin_noise_t *perlin_noise_create(int seed) {
    perlin_noise_t *noise = malloc(sizeof(perlin_noise_t));
    if (noise == NULL) {
        LOG_ERROR("Failed to allocate memory for perlin noise");
        return NULL;
    }
    noise->base.compute = perlin_noise_compute;
    noise->noise        = malloc(sizeof(fnl_state));
    if (noise->noise == NULL) {
        LOG_ERROR("Failed to allocate memory for FastNoiseLite state");
        free(noise);
        return NULL;
    }
    *((fnl_state *)noise->noise)            = fnlCreateState();
    ((fnl_state *)noise->noise)->seed       = seed;
    ((fnl_state *)noise->noise)->noise_type = FNL_NOISE_PERLIN;
    return noise;
}

float perlin_noise_compute(noise_t *noise, float x, float y) {
    ASSERT(noise != NULL);
    float raw_noise = fnlGetNoise2D(((fnl_state *)((perlin_noise_t *)noise)->noise), x, y);
    return (raw_noise + 1.0f) * 128.0f;
}

void perlin_noise_destroy(perlin_noise_t *noise) {
    ASSERT(noise != NULL);
    free(noise->noise);
    free(noise);
}
