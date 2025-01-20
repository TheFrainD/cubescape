
#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef struct {
#ifdef _WIN32
    HANDLE handle;
#else
    pthread_t handle;
#endif
} thread_t;

typedef struct {
#ifdef _WIN32
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif
} mutex_t;

#ifdef _WIN32
typedef DWORD(WINAPI *thread_func_t)(LPVOID);
#define THREAD_RETURN_TYPE        DWORD
#define THREAD_CALLING_CONVENTION WINAPI
#define THREAD_PARAM_TYPE         LPVOID
#define THREAD_OK                 0
#else
typedef void *(*thread_func_t)(void *);
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
 * THREAD_FUNC(my_thread_function, void *arg) {
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
 * @brief Create a new mutex
 *
 * @param mutex Pointer to the mutex_t struct
 */
void mutex_create(mutex_t *mutex);

/**
 * @brief Destroy a mutex
 *
 * @param mutex Pointer to the mutex_t struct
 */
void mutex_destroy(mutex_t *mutex);

/**
 * @brief Lock a mutex
 *
 * @param mutex Pointer to the mutex_t struct
 */
void mutex_lock(mutex_t *mutex);

/**
 * @brief Unlock a mutex
 *
 * @param mutex Pointer to the mutex_t struct
 */
void mutex_unlock(mutex_t *mutex);
