#include "cubelog.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static const char *level_strings[] = {
    "trace", "debug", "info", "warn", "error", "fatal"
};

static const char *level_fg[] = {
    "37", "36", "32", "33", "31", "35"
};

static int stdout_log_level = CUBELOG_LEVEL_INFO;

void cubelog_set_level(int level) {
    if (level >= 0 && level <= 5) {
        stdout_log_level = level;
    }
}

static FILE *log_fp = NULL;
static int fp_log_level = 0;

void cubelog_set_fp(FILE *fp, int level) {
    log_fp = fp;
    fp_log_level = level;
}

void cubelog_log(int level, const char *file, int line, const char *fmt, ...) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);

    FILE *outputs[] = {stdout, log_fp};
    size_t num_outputs = log_fp ? 2 : 1;

    for (size_t i = 0; i < num_outputs; ++i) {
        FILE *output = outputs[i];

        if (output == stdout) {
            if (level < stdout_log_level) {
                continue;
            }
            fprintf(output, "\x1b[90m[%s]\x1b[0m [\x1b[1;%sm%s\x1b[0m] ", buf, level_fg[level], level_strings[level]);
        } else {
            if (level < fp_log_level) {
                continue;
            }
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
