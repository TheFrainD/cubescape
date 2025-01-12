#pragma once

#include <cglm/struct.h>

typedef struct {
    int width;
    int height;
    const char* title;
} window_settings_t;

typedef void(*window_framebuffersize_callback_t)(ivec2s size);

/**
 * @brief Initializes the window with the given settings.
 * 
 * @param settings The settings to initialize the window with.
 */
void window_init(window_settings_t settings);

/**
 * @brief Destroys the window and releases any associated resources.
 */
void window_deinit();

/**
 * @brief Sets the settings of the window.
 * 
 * @param settings The new settings of the window.
 */
void window_set_settings(window_settings_t settings);

/**
 * @brief Retrieves the settings of the window.
 * 
 * @return window_settings_t The settings of the window.
 */
window_settings_t window_get_settings();

/**
 * @brief Retrieves the size of the framebuffer.
 * 
 * @return ivec2s The size of the framebuffer.
 */
ivec2s window_get_framebuffer_size();

/**
 * @brief Sets the size of the window.
 * 
 * @param size The new size of the window.
 */
void window_set_size(ivec2s size);

/**
 * @brief Retrieves the size of the window.
 * 
 * @return ivec2s The size of the window.
 */
ivec2s window_get_size();

/**
 * @brief Checks if the window should close.
 * 
 * @return int Non-zero if the window should close, zero otherwise.
 */
int window_should_close();

/**
 * @brief Swaps the front and back buffers.
 */
void window_swap_buffers();

/**
 * @brief Polls for and processes events.
 */
void window_poll_events();

/**
 * @brief Sets the swap interval for buffer swapping.
 * 
 * @param interval The swap interval.
 */
void window_set_swap_interval(int interval);

/**
 * @brief Updates the delta time between frames.
 */
void window_update_delta_time();

/**
 * @brief Retrieves the delta time between frames.
 * 
 * @return float The delta time between frames.
 */
float window_get_delta_time();

/**
 * @brief Adds a callback function for framebuffer size changes.
 * 
 * @param callback The callback function to be called when the framebuffer size changes.
 */
void window_add_framebuffersize_callback(window_framebuffersize_callback_t callback);
