#pragma once

typedef struct {
    int width;
    int height;
    const char* title;
} WindowSettings;

/**
 * @brief Initializes the window with the given settings.
 * 
 * @param settings The settings to initialize the window with.
 */
void window_init(WindowSettings settings);

/**
 * @brief Destroys the window and releases any associated resources.
 */
void window_destroy();

/**
 * @brief Sets the title of the window.
 * 
 * @param title The new title for the window.
 */
void window_set_title(const char* title);

/**
 * @brief Retrieves the current size of the window.
 * 
 * @param width Pointer to an integer to store the width of the window.
 * @param height Pointer to an integer to store the height of the window.
 */
void get_window_size(int *width, int* height);

/**
 * @brief Retrieves the size of the framebuffer.
 * 
 * @param width Pointer to an integer to store the width of the framebuffer.
 * @param height Pointer to an integer to store the height of the framebuffer.
 */
void get_framebuffer_size(int *width, int* height);

/**
 * @brief Sets the size of the window.
 * 
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void set_window_size(int width, int height);

/**
 * @brief Checks if the window should close.
 * 
 * @return int Non-zero if the window should close, zero otherwise.
 */
int window_should_close();

/**
 * @brief Swaps the front and back buffers.
 */
void swap_buffers();

/**
 * @brief Polls for and processes events.
 */
void poll_events();

/**
 * @brief Sets the swap interval for buffer swapping.
 * 
 * @param interval The swap interval.
 */
void set_swap_interval(int interval);

/**
 * @brief Updates the delta time between frames.
 */
void update_delta_time();

/**
 * @brief Retrieves the delta time between frames.
 * 
 * @return float The delta time between frames.
 */
float get_delta_time();