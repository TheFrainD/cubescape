#pragma once

typedef struct {
    int width;
    int height;
    const char* title;
} window_settings_t;

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
 * @param width Pointer to an integer to store the width of the framebuffer.
 * @param height Pointer to an integer to store the height of the framebuffer.
 */
void window_get_framebuffer_size(int *width, int* height);

/**
 * @brief Sets the size of the window.
 * 
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void window_set_size(int width, int height);

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
