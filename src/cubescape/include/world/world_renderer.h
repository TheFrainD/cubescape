#pragma once

#include <cubegl/shader_program.h>

#include "graphics/tilemap.h"
#include "world/world.h"

typedef struct world_renderer_state world_renderer_state_t;

typedef struct {
    world_renderer_state_t *state;
} world_renderer_t;

typedef struct {
    tilemap_t *tilemap;
    shader_program_t *block_shader;
    int draw_distance;
} world_renderer_settings_t;

/**
 * @brief Creates a new world renderer.
 *
 * @param settings The settings to use for the world renderer.
 *
 * @return world_renderer_t* The created world renderer.
 */
world_renderer_t *world_renderer_create(world_renderer_settings_t settings);

/**
 * @brief Destroys the specified world renderer.
 *
 * This function frees the memory allocated for the specified world renderer object.
 *
 * @param renderer A pointer to the world renderer object to destroy.
 */
void world_renderer_destroy(world_renderer_t *renderer);

/**
 * @brief Prepares the specified world for rendering.
 *
 * This function prepares the specified world for rendering by generating the meshes for each chunk.
 *
 * @param renderer A pointer to the world renderer object.
 */
void world_renderer_prepare(world_renderer_t *renderer, world_t *world);

/**
 * @brief Renders the specified world.
 *
 * @param renderer A pointer to the world renderer object.
 * @param world A pointer to the world object to render.
 * @param camera_position The position of the camera.
 */
void world_renderer_render(world_renderer_t *renderer, world_t *world, vec3s camera_position);
