#pragma once

#include <stdint.h>

#include <cglm/cglm.h>

#include "graphics/shader_program.h"

typedef struct {
    float speed;
    float sensitivity;
    float fov;
} camera_settings_t;

/**
 * @brief Initializes the camera with a given position and settings.
 * 
 * @param position The initial position of the camera.
 * @param settings The settings to configure the camera.
 * @param uniform_buffer The uniform buffer to use for the camera.
 */
void camera_init(vec3 position, camera_settings_t settings, uint32_t uniform_buffer);

/**
 * @brief Sets the position of the camera.
 * 
 * @param position The new position of the camera.
 */
void camera_set_position(vec3 position);

/**
 * @brief Gets the current position of the camera.
 * 
 * @param position A vector to store the current position of the camera.
 */
void camera_get_position(vec3 position);

/**
 * @brief Sets the settings of the camera.
 * 
 * @param settings The new settings to configure the camera.
 */
void camera_set_settings(camera_settings_t settings);

/**
 * @brief Gets the current settings of the camera.
 * 
 * @return camera_settings_t The current settings of the camera.
 */
camera_settings_t camera_get_settings();

/**
 * @brief Updates the camera's position based on its current settings.
 */
void camera_update_position();

/**
 * @brief Gets the perspective projection matrix of the camera.
 * 
 * @param projection A matrix to store the perspective projection of the camera.
 */
void camera_get_perpective(mat4 projection);