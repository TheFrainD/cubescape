#pragma once

#include <cglm/struct.h>

#include "graphics/camera.h"
#include "graphics/mesh.h"

#define RENDERER_DEFAULT_CLEAR_COLOR \
    (vec3s) {{0.0f, 0.0f, 0.0f}}

typedef struct {
    vec3s clear_color;
    float near_clip;
    float far_clip;
    camera_settings_t camera_settings;
} renderer_settings_t;

typedef struct {
    int wireframe;

    vec3s clear_color;

    camera_t *camera;

    float near_clip;
    float far_clip;

    float max_anisotropy;
} renderer_state_t;

/**
 * @brief Initializes the renderer.
 *
 * This function should be called before any other renderer functions.
 *
 * @param settings The settings to configure the renderer.
 *
 * @return int Zero if the renderer was initialized successfully, non-zero otherwise.
 */
int renderer_init(renderer_settings_t settings);

/**
 * @brief Deinitializes the renderer.
 *
 * This function should be called when the renderer is no longer needed.
 */
void renderer_deinit();

/**
 * @brief Begins a new frame.
 *
 * This function should be called at the beginning of each frame.
 */
void renderer_begin_frame();

/**
 * @brief Ends the current frame.
 *
 * This function should be called at the end of each frame.
 */
void renderer_end_frame();

/**
 * @brief Draws a mesh at the specified position, rotation, and scale.
 *
 * @param mesh The mesh to draw.
 * @param position The position to draw the mesh at.
 * @param rotation The rotation of the mesh.
 * @param scale The scale of the mesh.
 */
void renderer_draw_mesh(mesh_t *mesh, vec3s position, vec3s rotation, vec3s scale);

/**
 * @brief Returns the vertex array used by the renderer.
 *
 * @return uint32_t The ID of the vertex array used by the renderer.
 */
uint32_t renderer_get_vertex_array();

/**
 * @brief Binds the specified vertex array.
 *
 * @param vertex_array The ID of the vertex array to bind.
 */
void renderer_bind_vertex_buffer(uint32_t buffer);

/**
 * @brief Returns the current state of the renderer.
 *
 * @return renderer_state_t The current state of the renderer.
 */
renderer_state_t *renderer_get_state();

/**
 * @brief Returns the camera used by the renderer.
 *
 * @return camera_t* The camera used by the renderer.
 */
camera_t *renderer_get_camera();
