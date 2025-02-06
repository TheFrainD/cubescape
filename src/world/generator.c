#include "world/generator.h"

#include <string.h>

#include "core/assert.h"
#include "core/bool.h"
#include "core/log.h"
#include "core/math.h"

struct world_gen_task {
    thread_t thread;
    world_generator_t *generator;
    chunk_t *chunk;
    bool_t busy;
};

static THREAD_FUNC(world_gen_thread, arg) {
    struct world_gen_task *task  = arg;
    chunk_t *chunk               = task->chunk;
    world_generator_t *generator = task->generator;

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

        if (chunk->blocks[i] != BLOCK_ID_STONE ||
            chunk->blocks[(dirt_position.z * CHUNK_SIZE * CHUNK_HEIGHT) + (dirt_position.y * CHUNK_SIZE) +
                          dirt_position.x] != BLOCK_ID_AIR) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            chunk->blocks[(dirt_position.z * CHUNK_SIZE * CHUNK_HEIGHT) + (dirt_position.y * CHUNK_SIZE) +
                          dirt_position.x] = BLOCK_ID_DIRT;
            dirt_position                  = glms_ivec3_add(dirt_position, (ivec3s) {{0, 1, 0}});
        }

        chunk
            ->blocks[(dirt_position.z * CHUNK_SIZE * CHUNK_HEIGHT) + (dirt_position.y * CHUNK_SIZE) + dirt_position.x] =
            BLOCK_ID_GRASS;
    }

    chunk->flags.generated  = CS_TRUE;
    chunk->flags.generating = CS_FALSE;
    task->busy              = CS_FALSE;
    return THREAD_OK;
}

world_generator_t *world_generator_create(world_generator_parameters_t parameters) {
    world_generator_t *generator = malloc(sizeof(world_generator_t));
    if (generator == NULL) {
        LOG_ERROR("Failed to allocate memory for world generator");
        return NULL;
    }
    generator->parameters  = parameters;
    generator->noise_scale = 1.3f;

    for (int i = 0; i < 4; ++i) {
        generator->octave_noise[i] = octave_noise_create(parameters.seed + i + 1, 8);
    }
    generator->combined_noise1 =
        combined_noise_create((noise_t *)generator->octave_noise[0], (noise_t *)generator->octave_noise[1]);
    generator->combined_noise2 =
        combined_noise_create((noise_t *)generator->octave_noise[1], (noise_t *)generator->octave_noise[2]);
    generator->octave_noise_misc = octave_noise_create(parameters.seed, 6);

    generator->task_pool = malloc(sizeof(struct world_gen_task) * parameters.thread_count);
    if (!generator->task_pool) {
        LOG_ERROR("Failed to allocate memory for world generator task pool");
        free(generator);
        return NULL;
    }
    memset(generator->task_pool, 0, sizeof(struct world_gen_task) * parameters.thread_count);

    return generator;
}

void world_generator_generate(world_generator_t *generator, chunk_t *chunk) {
    ASSERT(generator != NULL);
    ASSERT(chunk != NULL);

    for (size_t i = 0; i < generator->parameters.thread_count; ++i) {
        struct world_gen_task *task = &generator->task_pool[i];
        if (task->busy) {
            continue;
        }
        task->generator         = generator;
        task->chunk             = chunk;
        task->busy              = CS_TRUE;
        chunk->flags.generating = CS_TRUE;
        LOG_DEBUG("Generating chunk at index (%d, %d)", chunk->position.x, chunk->position.y);
        thread_create(&task->thread, world_gen_thread, task);
        return;
    }
}

void world_generator_destroy(world_generator_t *generator) {
    ASSERT(generator != NULL);

    for (size_t i = 0; i < generator->parameters.thread_count; ++i) {
        thread_join(&generator->task_pool[i].thread);
    }

    for (int i = 0; i < 4; ++i) {
        octave_noise_destroy(generator->octave_noise[i]);
    }
    combined_noise_destroy(generator->combined_noise1);
    combined_noise_destroy(generator->combined_noise2);
    octave_noise_destroy(generator->octave_noise_misc);
    free(generator);
}