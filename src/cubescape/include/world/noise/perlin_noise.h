#pragma once

#include "world/noise/noise.h"

typedef struct {
    noise_t base;
    void *noise;
} perlin_noise_t;

/**
 * @brief Creates a new Perlin noise generator.
 *
 * This function creates a new Perlin noise generator with the specified seed.
 *
 * @param seed The seed to use for the noise generator.
 * @return perlin_noise_t* The created Perlin noise generator.
 */
perlin_noise_t *perlin_noise_create(int seed);

/**
 * @brief Computes the Perlin noise value at the given coordinates.
 *
 * This function calculates the noise value for the specified coordinates (x, y)
 *
 * @param noise A pointer to the Perlin noise structure.
 * @param x The x-coordinate for which to compute the noise.
 * @param y The y-coordinate for which to compute the noise.
 * @return The computed noise value as a float.
 */
float perlin_noise_compute(noise_t *noise, float x, float y);

/**
 * @brief Destroys the specified Perlin noise generator.
 *
 * This function frees the memory allocated for the specified Perlin noise generator object.
 *
 * @param noise A pointer to the Perlin noise structure.
 */
void perlin_noise_destroy(perlin_noise_t *noise);
