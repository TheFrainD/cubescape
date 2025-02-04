#include "world/noise/combined_noise.h"

#include <stdlib.h>

#include "core/assert.h"
#include "core/log.h"

combined_noise_t *combined_noise_create(noise_t *noise1, noise_t *noise2) {
    combined_noise_t *noise = malloc(sizeof(combined_noise_t));
    if (noise == NULL) {
        LOG_ERROR("Failed to allocate memory for combined noise");
        return NULL;
    }

    noise->base.compute = combined_noise_compute;
    noise->noise1       = noise1;
    noise->noise2       = noise2;
    return noise;
}

float combined_noise_compute(noise_t *noise, float x, float y) {
    ASSERT(noise != NULL);
    noise_t *noise1 = ((combined_noise_t *)noise)->noise1;
    noise_t *noise2 = ((combined_noise_t *)noise)->noise2;

    ASSERT(noise1 != NULL && noise2 != NULL);
    return noise1->compute(noise1, x + noise2->compute(noise2, x, y), y);
}

void combined_noise_destroy(combined_noise_t *noise) {
    ASSERT(noise != NULL);
    free(noise);
}
