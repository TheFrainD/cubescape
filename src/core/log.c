#include "core/log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static const char *level_strings[] = {
    "trace", "debug", "info", "warn", "error", "fatal"
};

static const char *level_fg[] = {
    "37", "36", "32", "33", "31", "35"
};

static int current_log_level = LOG_INFO;

void set_log_level(int level) {
    if (level >= 0 && level <= 5) {
        current_log_level = level;
    }
}

static FILE *log_file = NULL;

void set_log_file(const char *file) {
    if (log_file) {
        fclose(log_file);
    }
    
    log_file = fopen(file, "w");
    if (!log_file) {
        fprintf(stderr, "Could not open log file: %s\n", file);
    }
}

void logger(int level, const char *file, int line, const char *fmt, ...) {
    if (level < current_log_level) {
        return;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);

    FILE *outputs[] = {stderr, log_file};
    size_t num_outputs = log_file ? 2 : 1;

    for (size_t i = 0; i < num_outputs; ++i) {
        FILE *output = outputs[i];

        if (output == stderr) {
            fprintf(output, "\x1b[90m[%s]\x1b[0m [\x1b[1;%sm%s\x1b[0m] ", buf, level_fg[level], level_strings[level]);
        } else {
            fprintf(output, "[%s] [%s] ", buf, level_strings[level]);
        }

        va_list args;
        va_start(args, fmt);
        vfprintf(output, fmt, args);
        va_end(args);

        fprintf(output, "\n");
        fflush(output);
    }
}
