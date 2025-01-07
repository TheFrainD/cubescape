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

void logger(int level, const char *file, int line, const char *fmt, ...) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);

    fprintf(stderr, "\x1b[90m[%s]\x1b[0m [\x1b[1;%sm%s\x1b[0m] ", buf, level_fg[level], level_strings[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}
