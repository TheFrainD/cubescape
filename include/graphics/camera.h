#pragma once

#include <stdint.h>

#include <cglm/cglm.h>

#include "graphics/shader_program.h"

typedef struct {
    float speed;
    float sensitivity;
    float fov;
} CameraSettings;

// Intialize the camera with the given position, settings, and uniform buffer
void camera_init(vec3 position, CameraSettings settings, uint32_t uniform_buffer);

// Update the position of the camera
void camera_set_position(vec3 position);
void camera_get_position(vec3 position);

// Set the settings of the camera
void camera_set_settings(CameraSettings settings);

// Update the position of the camera
void camera_update_position();

// Get the projection matrix of the camera
void camera_get_perpective(mat4 projection);