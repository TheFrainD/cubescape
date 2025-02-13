#pragma once

#include "world/noise/noise.h"

typedef struct {
    noise_t base;
    noise_t *noise1;
    noise_t *noise2;
} combined_noise_t;

/**
 * @brief Creates a combined noise object from two noise sources.
 *
 * @param noise1 Pointer to the first noise source.
 * @param noise2 Pointer to the second noise source.
 * @return Pointer to the created combined noise object.
 */
combined_noise_t *combined_noise_create(noise_t *noise1, noise_t *noise2);

/**
 * @brief Computes the combined noise value at the given coordinates.
 *
 * @param noise Pointer to the combined noise object.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return The computed noise value.
 */
float combined_noise_compute(noise_t *noise, float x, float y);

/**
 * @brief Destroys the combined noise object and frees associated resources.
 *
 * @param noise Pointer to the combined noise object to be destroyed.
 */
void combined_noise_destroy(combined_noise_t *noise);
