#pragma once

#include "world/noise/noise.h"
#include "world/noise/perlin_noise.h"

typedef struct {
    noise_t base;
    int octaves;
    perlin_noise_t **noise;
} octave_noise_t;

/**
 * @brief Creates a new octave noise generator.
 *
 * This function creates a new octave noise generator with the specified number of octaves.
 *
 * @param octaves The number of octaves to use.
 * @return octave_noise_t* The created octave noise generator.
 */
octave_noise_t *octave_noise_create(int octaves);

/**
 * @brief Computes the octave noise value at the given coordinates.
 *
 * This function calculates the noise value for the specified coordinates (x, y)
 * using the provided octave noise structure.
 *
 * @param noise A pointer to the octave noise structure.
 * @param x The x-coordinate for which to compute the noise.
 * @param y The y-coordinate for which to compute the noise.
 * @return The computed noise value as a float.
 */
float octave_noise_compute(noise_t *noise, float x, float y);

/**
 * @brief Destroys the specified octave noise generator.
 *
 * This function frees the memory allocated for the specified octave noise generator object.
 *
 * @param noise A pointer to the octave noise generator object to destroy.
 */
void octave_noise_destroy(octave_noise_t *noise);
