#include "world/renderer.h"

#include <string.h>

#include <cubelog/cubelog.h>

#include "core/profiling.h"
#include "core/thread.h"

#include "graphics/renderer.h"

struct mesh_gen_task {
    thread_t thread;
    chunk_t *chunk;
    shader_program_t *shader_program;
    tilemap_t *tilemap;
    bool busy;
};

THREAD_FUNC(mesh_gen_thread, arg) {
    struct mesh_gen_task *task = arg;
    chunk_t *chunk             = task->chunk;

    chunk_generate_mesh(chunk, task->shader_program, task->tilemap);
    chunk->flags.mesh_generating = false;

    task->busy = false;
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
        task->busy                   = true;
        chunk->flags.mesh_generating = true;
        thread_create(&task->thread, mesh_gen_thread, task);
        return;
    }
}

world_renderer_t *world_renderer_create(world_renderer_settings_t settings) {
    world_renderer_t *renderer     = malloc(sizeof(world_renderer_t));
    renderer->state                = malloc(sizeof(world_renderer_state_t));
    renderer->state->tilemap       = settings.tilemap;
    renderer->state->block_shader  = settings.block_shader;
    renderer->state->draw_distance = settings.draw_distance;

    renderer->state->mesh_generation_thread_count = settings.mesh_generation_thread_count;
    renderer->state->mesh_gen_task_pool = malloc(sizeof(struct mesh_gen_task) * settings.mesh_generation_thread_count);
    memset(renderer->state->mesh_gen_task_pool, 0,
           sizeof(struct mesh_gen_task) * settings.mesh_generation_thread_count);
    return renderer;
}

void world_renderer_destroy(world_renderer_t *renderer) {
    if (renderer == NULL) {
        CUBELOG_ERROR("'world_renderer_destroy' called with NULL renderer");
        return;
    }

    for (size_t i = 0; i < renderer->state->mesh_generation_thread_count; ++i) {
        thread_join(&renderer->state->mesh_gen_task_pool[i].thread);
    }

    free(renderer->state);
    free(renderer);
}

void world_renderer_prepare(world_renderer_t *renderer, world_t *world) {
    if (renderer == NULL) {
        CUBELOG_ERROR("'world_renderer_prepare' called with NULL renderer");
        return;
    }

    if (world == NULL) {
        CUBELOG_ERROR("'world_renderer_prepare' called with NULL world");
        return;
    }
}

void world_renderer_render(world_renderer_t *renderer, world_t *world, vec3s camera_position) {
    if (renderer == NULL) {
        CUBELOG_ERROR("'world_renderer_render' called with NULL renderer");
        return;
    }

    if (world == NULL) {
        CUBELOG_ERROR("'world_renderer_render' called with NULL world");
        return;
    }

    ivec2s index =
        (ivec2s) {{camera_position.x >= 0 ? (camera_position.x / CHUNK_SIZE) : (camera_position.x / CHUNK_SIZE - 1),
                   camera_position.z >= 0 ? (camera_position.z / CHUNK_SIZE) : (camera_position.z / CHUNK_SIZE - 1)}};

    for (int x = index.x - renderer->state->draw_distance; x <= index.x + renderer->state->draw_distance; ++x) {
        for (int z = index.y - renderer->state->draw_distance; z <= index.y + renderer->state->draw_distance; ++z) {
            ivec2s chunk_index = (ivec2s) {{x, z}};
            chunk_t *chunk     = world_get_chunk(world, chunk_index, true);

            if (!chunk->flags.generated && !chunk->flags.generating) {
                world_generate_chunk(world, chunk);
            }

            if (chunk->flags.dirty && !chunk->flags.mesh_generating && chunk->flags.generated) {
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