#include "world/renderer.h"

#include <string.h>

#include "core/assert.h"
#include "core/bool.h"
#include "core/log.h"
#include "core/concurrency/thread.h"

#include "graphics/renderer.h"

struct mesh_gen_task {
    thread_t thread;
    chunk_t *chunk;
    shader_program_t *shader_program;
    tilemap_t *tilemap;
    bool_t busy;
};

static THREAD_FUNC(mesh_gen_thread, arg) {
    struct mesh_gen_task *task = arg;
    chunk_t *chunk             = task->chunk;

    chunk_generate_mesh(chunk, task->shader_program, task->tilemap);
    chunk->flags.mesh_generating = CS_FALSE;

    task->busy = CS_FALSE;
    return THREAD_OK;
}

struct world_renderer_state {
    tilemap_t *tilemap;
    shader_program_t *block_shader;
    int draw_distance;

    size_t mesh_generation_thread_count;
    struct mesh_gen_task *mesh_gen_task_pool;
};

static void world_renderer_generate_mesh(world_renderer_t *renderer, chunk_t *chunk) {
    for (size_t i = 0; i < renderer->state->mesh_generation_thread_count; ++i) {
        struct mesh_gen_task *task = &renderer->state->mesh_gen_task_pool[i];
        if (task->busy) {
            continue;
        }
        task->chunk                  = chunk;
        task->shader_program         = renderer->state->block_shader;
        task->tilemap                = renderer->state->tilemap;
        task->busy                   = CS_TRUE;
        chunk->flags.mesh_generating = CS_TRUE;
        thread_create(&task->thread, mesh_gen_thread, task);
        return;
    }
}

world_renderer_t *world_renderer_create(world_renderer_settings_t settings) {
    world_renderer_t *renderer = malloc(sizeof(world_renderer_t));
    if (renderer == NULL) {
        LOG_ERROR("Failed to allocate memory for world renderer");
        return NULL;
    }
    renderer->state = malloc(sizeof(world_renderer_state_t));
    if (renderer->state == NULL) {
        LOG_ERROR("Failed to allocate memory for world renderer state");
        free(renderer);
        return NULL;
    }
    renderer->state->tilemap       = settings.tilemap;
    renderer->state->block_shader  = settings.block_shader;
    renderer->state->draw_distance = settings.draw_distance;

    renderer->state->mesh_generation_thread_count = settings.mesh_generation_thread_count;
    renderer->state->mesh_gen_task_pool = malloc(sizeof(struct mesh_gen_task) * settings.mesh_generation_thread_count);
    if (renderer->state->mesh_gen_task_pool == NULL) {
        LOG_ERROR("Failed to allocate memory for mesh generation task pool");
        free(renderer->state);
        free(renderer);
        return NULL;
    }
    memset(renderer->state->mesh_gen_task_pool, 0,
           sizeof(struct mesh_gen_task) * settings.mesh_generation_thread_count);
    return renderer;
}

void world_renderer_destroy(world_renderer_t *renderer) {
    ASSERT(renderer != NULL);

    for (size_t i = 0; i < renderer->state->mesh_generation_thread_count; ++i) {
        thread_join(&renderer->state->mesh_gen_task_pool[i].thread);
    }

    free(renderer->state);
    free(renderer);
}

void world_renderer_render(world_renderer_t *renderer, world_t *world, vec3s camera_position) {
    ASSERT(renderer != NULL);
    ASSERT(world != NULL);

    ivec2s index =
        (ivec2s) {{camera_position.x >= 0 ? (camera_position.x / CHUNK_SIZE) : (camera_position.x / CHUNK_SIZE - 1),
                   camera_position.z >= 0 ? (camera_position.z / CHUNK_SIZE) : (camera_position.z / CHUNK_SIZE - 1)}};

    for (int x = index.x - renderer->state->draw_distance; x <= index.x + renderer->state->draw_distance; ++x) {
        for (int z = index.y - renderer->state->draw_distance; z <= index.y + renderer->state->draw_distance; ++z) {
            ivec2s chunk_index = (ivec2s) {{x, z}};
            chunk_t *chunk     = world_get_chunk(world, chunk_index);
            if (!chunk) {
                chunk = world_add_chunk(world, chunk_index);
                continue;
            }

            if (!chunk->flags.generated && !chunk->flags.generating) {
                world_generate_chunk(world, chunk);
            }

            bool_t neighboring_chunks_generated = CS_TRUE;
            for (int i = 0; i < 4; ++i) {
                ivec2s neighbor_index =
                    (ivec2s) {{chunk_index.x + (i % 2 == 0 ? 1 : -1), chunk_index.y + (i / 2 == 0 ? 1 : -1)}};
                chunk_t *neighbor = world_get_chunk(world, neighbor_index);
                if (!neighbor) {
                    continue;
                }
                if (!neighbor->flags.generated) {
                    neighboring_chunks_generated = CS_FALSE;
                    break;
                }
            }

            if (chunk->flags.dirty && !chunk->flags.mesh_generating && chunk->flags.generated &&
                neighboring_chunks_generated) {
                world_renderer_generate_mesh(renderer, chunk);
            }

            if (chunk->mesh) {
                if (chunk->mesh->flags.ready_to_upload) {
                    mesh_upload(chunk->mesh);
                }

                if (chunk->mesh->flags.uploaded) {
                    vec3s position = (vec3s) {{chunk->position.x * CHUNK_SIZE, 0.0f, chunk->position.y * CHUNK_SIZE}};
                    renderer_draw_mesh(chunk->mesh, position, (vec3s) {{0.0f, 0.0f, 0.0f}},
                                       (vec3s) {{1.0f, 1.0f, 1.0f}});
                }
            }
        }
    }
}