#include "core/concurrency/mutex.h"

void mutex_create(mutex_t *mutex) {
#ifdef _WIN32
    mutex->mutex = CreateMutex(NULL, FALSE, NULL);
#else
    pthread_mutex_init(&mutex->mutex, NULL);
#endif
}

void mutex_destroy(mutex_t *mutex) {
#ifdef _WIN32
    CloseHandle(mutex->mutex);
#else
    pthread_mutex_destroy(&mutex->mutex);
#endif
}

void mutex_lock(mutex_t *mutex) {
#ifdef _WIN32
    WaitForSingleObject(mutex->mutex, INFINITE);
#else
    pthread_mutex_lock(&mutex->mutex);
#endif
}

void mutex_unlock(mutex_t *mutex) {
#ifdef _WIN32
    ReleaseMutex(mutex->mutex);
#else
    pthread_mutex_unlock(&mutex->mutex);
#endif
}
