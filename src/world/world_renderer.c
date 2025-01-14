#include "world/world_renderer.h"

#include "core/log.h"
#include "graphics/renderer.h"

struct world_renderer_state {
    tilemap_t *tilemap;
    shader_program_t *block_shader;
    int draw_distance;
};

world_renderer_t *world_renderer_create(world_renderer_settings_t settings) {
    world_renderer_t *renderer     = malloc(sizeof(world_renderer_t));
    renderer->state                = malloc(sizeof(world_renderer_state_t));
    renderer->state->tilemap       = settings.tilemap;
    renderer->state->block_shader  = settings.block_shader;
    renderer->state->draw_distance = settings.draw_distance;
    return renderer;
}

void world_renderer_destroy(world_renderer_t *renderer) {
    if (renderer == NULL) {
        LOG_ERROR("'world_renderer_destroy' called with NULL renderer");
        return;
    }

    free(renderer->state);
    free(renderer);
}

void world_renderer_prepare(world_renderer_t *renderer, world_t *world) {
    if (renderer == NULL) {
        LOG_ERROR("'world_renderer_prepare' called with NULL renderer");
        return;
    }

    if (world == NULL) {
        LOG_ERROR("'world_renderer_prepare' called with NULL world");
        return;
    }

    for (size_t i = 0; i < world->size * world->size; ++i) {
        chunk_t *chunk = world->chunks[i];
        if (chunk->dirty) {
            chunk_generate_mesh(chunk, renderer->state->block_shader, renderer->state->tilemap);
        }
    }
}

void world_renderer_render(world_renderer_t *renderer, world_t *world, vec3s camera_position) {
    if (renderer == NULL) {
        LOG_ERROR("'world_renderer_render' called with NULL renderer");
        return;
    }

    if (world == NULL) {
        LOG_ERROR("'world_renderer_render' called with NULL world");
        return;
    }

    for (size_t i = 0; i < world->size * world->size; ++i) {
        chunk_t *chunk = world->chunks[i];

        vec3s position = (vec3s) {{chunk->position.x * CHUNK_SIZE, 0.0f, chunk->position.y * CHUNK_SIZE}};
        float closest_x =
            fmaxf(chunk->position.x * CHUNK_SIZE, fminf(camera_position.x, (chunk->position.x + 1) * CHUNK_SIZE));
        float closest_y = fmaxf(0.0f, fminf(camera_position.y, CHUNK_HEIGHT));
        float closest_z =
            fmaxf(chunk->position.y * CHUNK_SIZE, fminf(camera_position.z, (chunk->position.y + 1) * CHUNK_SIZE));
        float distance = sqrtf(powf(camera_position.x - closest_x, 2) + powf(camera_position.y - closest_y, 2) +
                               powf(camera_position.z - closest_z, 2));
        if (distance < renderer->state->draw_distance * CHUNK_SIZE) {
            renderer_draw_mesh(chunk->mesh, position, GLMS_VEC3_ZERO, GLMS_VEC3_ONE);
        }
    }
}