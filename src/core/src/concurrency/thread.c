#include "core/concurrency/thread.h"

#ifndef _WIN32
#include <unistd.h>
#endif

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

int thread_hardware_concurrency() {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void thread_sleep_for(uint64_t ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void thread_yield() {
#ifdef _WIN32
    SwitchToThread();
#else
    sched_yield();
#endif
}

uint64_t thread_get_id() {
#ifdef _WIN32
    return GetCurrentThreadId();
#else
    return (uint64_t)(uintptr_t)pthread_self();
#endif
}
