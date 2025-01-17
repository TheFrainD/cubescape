#include "world/generator.h"

#include <cubelog/cubelog.h>

#include "core/math.h"

world_generator_t *world_generator_create(world_generator_parameters_t parameters) {
    world_generator_t *generator = malloc(sizeof(world_generator_t));
    generator->parameters        = parameters;
    generator->noise_scale       = 1.3f;

    for (int i = 0; i < 4; ++i) {
        generator->octave_noise[i] = octave_noise_create(8);
    }
    generator->combined_noise1 =
        combined_noise_create((noise_t *)generator->octave_noise[0], (noise_t *)generator->octave_noise[1]);
    generator->combined_noise2 =
        combined_noise_create((noise_t *)generator->octave_noise[1], (noise_t *)generator->octave_noise[2]);
    generator->octave_noise_misc = octave_noise_create(6);

    return generator;
}

void world_generator_generate(world_generator_t *generator, chunk_t *chunk) {
    if (generator == NULL) {
        CUBELOG_ERROR("'world_generator_generate' called with NULL generator");
        return;
    }

    if (chunk == NULL) {
        CUBELOG_ERROR("'world_generator_generate' called with NULL chunk");
        return;
    }

    for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
        ivec3s block_position =
            (ivec3s) {{i % CHUNK_SIZE, (i / CHUNK_SIZE) % CHUNK_HEIGHT, i / (CHUNK_SIZE * CHUNK_HEIGHT)}};
        ivec3s world_position = (ivec3s) {{chunk->position.x * CHUNK_SIZE + block_position.x, block_position.y,
                                           chunk->position.y * CHUNK_SIZE + block_position.z}};

        vec2s noise_sample = (vec2s) {{world_position.x, world_position.z}};

        float height1 =
            combined_noise_compute((noise_t *)generator->combined_noise1, noise_sample.x * generator->noise_scale,
                                   noise_sample.y * generator->noise_scale) /
                6.0f -
            4.0f;

        float height2 =
            combined_noise_compute((noise_t *)generator->combined_noise2, noise_sample.x * generator->noise_scale,
                                   noise_sample.y * generator->noise_scale) /
                5.0f +
            6.0f;
        if (octave_noise_compute((noise_t *)generator->octave_noise_misc, noise_sample.x * generator->noise_scale,
                                 noise_sample.y * generator->noise_scale) /
                8.0f >
            0.0f) {
            height2 = height1;
        }

        float height = MAX(height1, height2) / 2.0f;

        if (block_position.y > height) {
            chunk->blocks[i] = BLOCK_ID_AIR;
            continue;
        }
        chunk->blocks[i] = BLOCK_ID_STONE;
    }

    for (size_t i = 0; i < CHUNK_VOLUME; ++i) {
        ivec3s block_position =
            (ivec3s) {{i % CHUNK_SIZE, (i / CHUNK_SIZE) % CHUNK_HEIGHT, i / (CHUNK_SIZE * CHUNK_HEIGHT)}};
        ivec3s dirt_position = glms_ivec3_add(block_position, (ivec3s) {{0, 1, 0}});

        if (chunk->blocks[i] != BLOCK_ID_STONE || chunk_get_block(chunk, dirt_position) != BLOCK_ID_AIR) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            chunk_set_block(chunk, dirt_position, BLOCK_ID_DIRT);
            dirt_position = glms_ivec3_add(dirt_position, (ivec3s) {{0, 1, 0}});
        }

        chunk_set_block(chunk, dirt_position, BLOCK_ID_GRASS);
    }
}

void world_generator_destroy(world_generator_t *generator) {
    for (int i = 0; i < 4; ++i) {
        octave_noise_destroy(generator->octave_noise[i]);
    }
    combined_noise_destroy(generator->combined_noise1);
    combined_noise_destroy(generator->combined_noise2);
    octave_noise_destroy(generator->octave_noise_misc);
    free(generator);
}