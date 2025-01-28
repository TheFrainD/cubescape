#include "world/noise/combined_noise.h"

#include <stdlib.h>

combined_noise_t *combined_noise_create(noise_t *noise1, noise_t *noise2) {
    combined_noise_t *noise = malloc(sizeof(combined_noise_t));
    noise->base.compute     = combined_noise_compute;
    noise->noise1           = noise1;
    noise->noise2           = noise2;
    return noise;
}

float combined_noise_compute(noise_t *noise, float x, float y) {
    noise_t *noise1 = ((combined_noise_t *)noise)->noise1;
    noise_t *noise2 = ((combined_noise_t *)noise)->noise2;
    return noise1->compute(noise1, x + noise2->compute(noise2, x, y), y);
}

void combined_noise_destroy(combined_noise_t *noise) { free(noise); }
