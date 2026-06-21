#include "memory.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>

void* xmalloc(size_t size)
{
    void* ptr = malloc(size);

    if (ptr == NULL && size != 0) {
        log_error("memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* xrealloc(void* ptr, size_t size)
{
    void* newPtr = realloc(ptr, size);

    if (newPtr == NULL && size != 0) {
        log_error("memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return newPtr;
}

char* xstrdup(const char* s)
{
    char* copy;

    if (s == NULL) {
        return NULL;
    }

    copy = strdup(s);
    if (copy == NULL) {
        log_error("memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return copy;
}
