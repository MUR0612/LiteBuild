#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include <stdarg.h>

/* Write informational output to stdout. */
void log_info(const char* format, ...);

/* Write a warning to stderr with the project prefix. */
void log_warn(const char* format, ...);

/* Write an error to stderr with the project prefix. */
void log_error(const char* format, ...);

#endif /* UTILS_LOGGER_H */
