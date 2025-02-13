#pragma once

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "core/concurrency/mutex.h"

typedef struct {
#ifdef _WIN32
#error "condition_variable_t is not supported on Windows"
#else
    pthread_cond_t cond;
#endif
} condition_variable_t;

/**
 * @brief Create a new condition variable
 *
 * @param cond Pointer to the condition_variable_t struct
 */
void condition_variable_create(condition_variable_t *cond);

void condition_variable_notify_all(condition_variable_t *cond);

void condition_variable_notify_one(condition_variable_t *cond);

void condition_variable_wait(condition_variable_t *cond, mutex_t *mutex);

void condition_variable_wait_for(condition_variable_t *cond, mutex_t *mutex, uint64_t timeout_ms);
