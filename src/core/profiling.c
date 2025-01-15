#include "core/profiling.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/log.h"

#ifdef PROFILING_ENABLED
typedef struct {
    int id;
    clock_t start_time;
    char description[256];
} profile_t;

#define PROFILES_BASE_SIZE 100

static profile_t *profiles;
static int profile_count    = 0;
static int profile_capacity = PROFILES_BASE_SIZE;

static void profile_delete(int id) {
    --profile_count;
    if (id < profile_count - 1) {
        memcpy(&profiles[id], &profiles[id + 1], (profile_count - id) * sizeof(profile_t));
    }

    if (profile_capacity >= profile_count * 2) {
        profile_capacity /= 2;
        profiles = (profile_t *)realloc(profiles, profile_capacity * sizeof(profile_t));
    }
}

int profiling_init() {
    profiles = (profile_t *)malloc(PROFILES_BASE_SIZE * sizeof(profile_t));
    if (profiles == NULL) {
        LOG_ERROR("Failed to allocate memory for profiles.\n");
        return -1;
    }

    return 0;
}

void profiling_deinit() {
    free(profiles);
    profiles         = NULL;
    profile_count    = 0;
    profile_capacity = 0;
}

int profiling_begin(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(profiles[profile_count].description, sizeof(profiles[profile_count].description), format, args);
    va_end(args);

    profiles[profile_count].id         = profile_count;
    profiles[profile_count].start_time = clock();
    ++profile_count;

    if (profile_count >= profile_capacity) {
        profile_capacity *= 2;
        profiles = (profile_t *)realloc(profiles, profile_capacity * sizeof(profile_t));
    }

    return profiles[profile_count - 1].id;
}

void profiling_end(int id) {
    if (id < 0 || id >= profile_count) {
        LOG_ERROR("Invalid profile id: %d", id);
        return;
    }

    clock_t end_time    = clock();
    double elapsed_time = ((double)(end_time - profiles[id].start_time)) / CLOCKS_PER_SEC;

    LOG_DEBUG("Profile [%d]: %s - Elapsed time: %.6f seconds", id, profiles[id].description, elapsed_time);

    profile_delete(id);
}

void profiling_cancel(int id) {
    profile_delete(id);
}

#else
int profiling_init() { return 0; }

void profiling_deinit() {}

int profiling_begin(const char *format, ...) { return 0; }

void profiling_end(int id) {}

void profiling_cancel(int id) {}
#endif  // PROFILING_ENABLED
