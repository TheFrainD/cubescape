#include "core/thread.h"

int thread_create(thread_t *thread, thread_func_t func, void *arg) {
#ifdef _WIN32
    thread->handle = CreateThread(NULL, 0, func, arg, 0, NULL);
    if (!thread->handle) {
        return 1;
    }
#else
    int result = pthread_create(&thread->handle, NULL, func, arg);
    if (result) {
        return 1;
    }
#endif
    return 0;
}

void thread_join(thread_t *thread) {
#ifdef _WIN32
    WaitForSingleObject(thread->handle, INFINITE);
#else
    pthread_join(thread->handle, NULL);
#endif
}

void thread_detach(thread_t *thread) {
#ifdef _WIN32
    CloseHandle(thread->handle);
#else
    pthread_detach(thread->handle);
#endif
}

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
