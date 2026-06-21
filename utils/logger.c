#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

#include "../core/types.h"

void log_info(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void log_warn(const char* format, ...)
{
    va_list args;

    fprintf(stderr, "%s: warning: ", PROJECT_NAME);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void log_error(const char* format, ...)
{
    va_list args;

    fprintf(stderr, "%s: ", PROJECT_NAME);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
