#pragma once

#include <stdio.h>

enum { LOG_LEVEL_TRACE, LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_WARN, LOG_LEVEL_ERROR, LOG_LEVEL_FATAL };

#define LOG_TRACE(...) logger_log(LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) logger_log(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) logger_log(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) logger_log(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logger_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) logger_log(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a message with a specified log level.
 *
 * @param level The log level (e.g., DEBUG, INFO, WARN, ERROR).
 * @param file The name of the source file where the log message is generated.
 * @param line The line number in the source file where the log message is generated.
 * @param fmt The format string for the log message (similar to printf).
 * @param ... Additional arguments for the format string.
 */
void logger_log(int level, const char *file, int line, const char *fmt, ...);

/**
 * @brief Sets the logging level. Messages with a level lower than the specified level will not be logged.
 *
 * @param level The log level to set (e.g., DEBUG, INFO, WARN, ERROR).
 */
void logger_set_level(int level);

/**
 * @brief Sets the file pointer for logging output and the minimum log level for that file pointer.
 *
 * @param fp The file pointer where log messages will be written.
 * @param level The minimum log level for messages to be written to the file pointer.
 */
void logger_set_fp(FILE *fp, int level);