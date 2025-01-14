#pragma once

#include <stdint.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "graphics/shader_program.h"

typedef struct camera camera_t;

typedef struct {
    float sensitivity;
    float fov;
} camera_settings_t;

/**
 * @brief Initializes the camera with given settings.
 * 
 * @param settings The settings to configure the camera.
 * 
 * @return camera_t* The initialized camera.
 */
camera_t *camera_create(camera_settings_t settings);

/**
 * @brief Destroys the camera.
 * 
 * @param camera The camera to destroy.
 */
void camera_destroy(camera_t *camera);

/**
 * @brief Sets the position of the camera.
 * 
 * @param camera The camera to set the position of.
 * @param position The new position of the camera.
 */
void camera_set_position(camera_t *camera, vec3s position);

/**
 * @brief Translates the camera by the given translation vector.
 * 
 * @param camera The camera to translate.
 * @param translation The translation vector.
 */
void camera_translate(camera_t *camera, vec3s translation);

/**
 * @brief Gets the current position of the camera.
 * 
 * @param camera The camera to get the position of.
 * 
 * @return vec3s The current position of the camera.
 */
vec3s camera_get_position(camera_t *camera);

/**
 * @brief Sets the settings of the camera.
 * 
 * @param camera The camera to set the settings of.
 * @param settings The new settings to configure the camera.
 */
void camera_apply_settings(camera_t *camera, camera_settings_t settings);

/**
 * @brief Gets the field of view of the camera.
 * 
 * @param camera The camera to get the field of view of.
 * 
 * @return float The field of view of the camera.
 */
float camera_get_fov(camera_t *camera);

/**
 * @brief Gets the sensitivity of the camera.
 * 
 * @param camera The camera to get the sensitivity of.
 * 
 * @return float The sensitivity of the camera.
 */
float camera_get_sensitivity(camera_t *camera);

/**
 * @brief Updates the view matrix of the camera based on the current mouse position.
 * 
 * @param camera The camera to update the view matrix of.
 * @param mouse_position The current mouse position.
 */
void camera_update_view(camera_t *camera, vec2s mouse_position);

/**
 * @brief Gets the view matrix of the camera.
 * 
 * @param camera The camera to get the view matrix of.
 * 
 * @return mat4s The view matrix of the camera.
 */
mat4s camera_get_view(camera_t *camera);

/**
 * @brief Checks if the camera view has changed.
 * 
 * @param camera The camera to check if the view has changed.
 * 
 * @return int Returns 1 if the camera view has changed, 0 otherwise.
 */
int camera_view_changed(camera_t *camera);

/**
 * @brief Resets the camera view changed flag.
 */
void camera_view_reset(camera_t *camera);

/**
 * @brief Gets the front vector of the camera.
 * 
 * @param camera The camera to get the front vector of.
 * 
 * @return vec3s The front vector of the camera.
 */
vec3s camera_get_front(camera_t *camera);

/**
 * @brief Gets the up vector of the camera.
 * 
 * @param camera The camera to get the up vector of.
 * 
 * @return vec3s The up vector of the camera.
 */
vec3s camera_get_up(camera_t *camera);

/**
 * @brief Gets the right vector of the camera.
 * 
 * @param camera The camera to get the right vector of.
 * 
 * @return vec3s The right vector of the camera.
 */
vec3s camera_get_right(camera_t *camera);
