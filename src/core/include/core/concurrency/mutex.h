#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef struct {
#ifdef _WIN32
    HANDLE mutex;
#else
    pthread_mutex_t mutex;
#endif
} mutex_t;

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
