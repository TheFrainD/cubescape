#include "world/noise/octave_noise.h"

#include <stdlib.h>

octave_noise_t *octave_noise_create(int octaves) {
    octave_noise_t *noise = malloc(sizeof(octave_noise_t));
    noise->octaves        = octaves;
    noise->noise          = malloc(octaves * sizeof(perlin_noise_t *));
    noise->base.compute   = octave_noise_compute;

    for (int i = 0; i < octaves; ++i) {
        noise->noise[i] = perlin_noise_create(rand());
    }
    return noise;
}

float octave_noise_compute(noise_t *noise, float x, float y) {
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
    free(noise->noise);
    free(noise);
}
