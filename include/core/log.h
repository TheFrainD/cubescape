#pragma once

// Logging levels
enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) logger(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) logger(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) logger(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) logger(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) logger(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) logger(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void logger(int level, const char *file, int line, const char *fmt, ...);

void set_log_level(int level);
void set_log_file(const char *file);