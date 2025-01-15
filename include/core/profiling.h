#pragma once

/**
 * @brief Initializes the profiling system.
 * 
 * This function initializes the profiling system.
 * 
 * @return int Zero if the profiling system was initialized successfully, non-zero otherwise.
 */
int profiling_init();

/**
 * @brief Deinitializes the profiling system.
 * 
 * This function deinitializes the profiling system.
 */
void profiling_deinit();

/**
 * @brief Starts profiling with the specified format string.
 *
 * This function starts profiling with the specified format string.
 *
 * @param format The format string to use for the profile description.
 *
 * @return int The ID of the profile.
 */
int profiling_begin(const char *format, ...);

/**
 * @brief Stops profiling with the specified ID.
 *
 * This function stops profiling with the specified ID.
 *
 * @param id The ID of the profile to stop.
 */
void profiling_end(int id);

/**
 * @brief Cancels profiling with the specified ID.
 * 
 * This function cancels profiling with the specified ID.
 * 
 * @param id The ID of the profile to cancel.
 */
void profiling_cancel(int id);
