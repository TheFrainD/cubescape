#pragma once 

#include "world/chunk.h"

#include "world/noise/combined_noise.h"
#include "world/noise/octave_noise.h"

typedef struct {
    int height;
    int water_level;
} world_generator_parameters_t;

typedef struct {
    octave_noise_t *octave_noise[4];
    combined_noise_t *combined_noise1;
    combined_noise_t *combined_noise2;
    octave_noise_t *octave_noise_misc;

    float noise_scale;

    world_generator_parameters_t parameters;
} world_generator_t;

world_generator_t *world_generator_create(world_generator_parameters_t parameters);

void world_generator_generate(world_generator_t *generator, chunk_t *chunk);

void world_generator_destroy(world_generator_t *generator);

