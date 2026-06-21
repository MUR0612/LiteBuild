#ifndef UTILS_MEMORY_H
#define UTILS_MEMORY_H

#include <stddef.h>

/* Allocate memory, or print an error and terminate safely. */
void* xmalloc(size_t size);

/* Reallocate memory, or print an error and terminate safely. */
void* xrealloc(void* ptr, size_t size);

/* Duplicate a string, or print an error and terminate safely. */
char* xstrdup(const char* s);

#endif /* UTILS_MEMORY_H */
