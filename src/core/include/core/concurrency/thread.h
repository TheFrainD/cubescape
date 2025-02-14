
#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "core/bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
#ifdef _WIN32
    HANDLE handle;
#else
    pthread_t handle;
#endif
} thread_t;

#ifdef _WIN32
typedef DWORD(WINAPI *thread_func_t)(LPVOID);
#define THREAD_RETURN_TYPE        DWORD
#define THREAD_CALLING_CONVENTION WINAPI
#define THREAD_PARAM_TYPE         LPVOID
#define THREAD_OK                 0
#else
#define THREAD_RETURN_TYPE void *
#define THREAD_CALLING_CONVENTION
#define THREAD_PARAM_TYPE void *
#define THREAD_OK         NULL
#endif

/**
 * @brief Function signature for a thread function
 *
 * @param arg Argument to pass to the function
 *
 * Usage example:
 * THREAD_FUNC(my_thread_function, arg) {
 *     // Do something
 *     return THREAD_OK;
 * }
 */
#define THREAD_FUNC(func, arg) THREAD_RETURN_TYPE THREAD_CALLING_CONVENTION func(THREAD_PARAM_TYPE arg)

typedef THREAD_RETURN_TYPE(THREAD_CALLING_CONVENTION *thread_func_t)(THREAD_PARAM_TYPE);

/**
 * @brief Create a new thread
 *
 * @param thread Pointer to the thread_t struct
 * @param func Function to run in the thread
 * @param arg Argument to pass to the function
 * @return int 0 on success, 1 on failure
 */
int thread_create(thread_t *thread, thread_func_t func, void *arg);

/**
 * @brief Wait for a thread to finish
 *
 * @param thread Pointer to the thread_t struct
 */
void thread_join(thread_t *thread);

/**
 * @brief Detach a thread
 *
 * @param thread Pointer to the thread_t struct
 */
void thread_detach(thread_t *thread);

/**
 * @brief Get the number of hardware threads
 *
 * @return int The number of hardware threads
 */
int thread_hardware_concurrency();

/**
 * @brief Sleep for the specified number of milliseconds
 *
 * @param ms The number of milliseconds to sleep
 */
void thread_sleep_for(uint64_t ms);

/**
 * @brief Yield the current thread
 *
 * This function yields the current thread to allow other threads to run
 */
void thread_yield();

/**
 * @brief Get the ID of the current thread
 *
 * @return uint64_t The ID of the current thread
 */
uint64_t thread_get_id();

#ifdef __cplusplus
}
#endif
