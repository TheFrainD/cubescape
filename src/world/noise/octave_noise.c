#include "world/noise/octave_noise.h"

#include <stdlib.h>

#include "core/assert.h"
#include "core/log.h"

octave_noise_t *octave_noise_create(int octaves) {
    octave_noise_t *noise = malloc(sizeof(octave_noise_t));
    if (noise == NULL) {
        LOG_ERROR("Failed to allocate memory for octave noise");
        return NULL;
    }

    noise->octaves = octaves;
    noise->noise   = malloc(octaves * sizeof(perlin_noise_t *));
    if (noise->noise == NULL) {
        LOG_ERROR("Failed to allocate memory for perlin noise array");
        free(noise);
        return NULL;
    }
    noise->base.compute = octave_noise_compute;

    for (int i = 0; i < octaves; ++i) {
        noise->noise[i] = perlin_noise_create(rand());
    }
    return noise;
}

float octave_noise_compute(noise_t *noise, float x, float y) {
    ASSERT(noise != NULL);
    octave_noise_t *octave_noise = (octave_noise_t *)noise;

    float value      = 0.0f;
    float wavelength = 1.0f;

    for (int i = 0; i < octave_noise->octaves; ++i) {
        noise_t *perlin_noise = (noise_t *)octave_noise->noise[i];
        value += perlin_noise->compute(perlin_noise, x * wavelength, y * wavelength) / wavelength;
        wavelength *= 2.0f;
    }

    // Normalize the value to be between 0 and 1
    return (value + 1.0f) / 2.0f;
}

void octave_noise_destroy(octave_noise_t *noise) {
    ASSERT(noise != NULL);
    free(noise->noise);
    free(noise);
}
