#include "vector.h"

#include "memory.h"

#include <stdlib.h>

void strvec_init(StringVector* vec)
{
    vec->capacity = 4;
    vec->count = 0;
    vec->data = xmalloc(sizeof(char*) * (vec->capacity + 1));
    vec->data[0] = NULL;
}

void strvec_take(StringVector* vec, char* str)
{
    if (vec->count >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = xrealloc(vec->data,
                sizeof(char*) * (vec->capacity + 1));
    }

    vec->data[vec->count] = str;
    vec->count++;
    vec->data[vec->count] = NULL;
}

void strvec_push(StringVector* vec, const char* str)
{
    if (str == NULL) {
        return;
    }
    strvec_take(vec, xstrdup(str));
}

void strvec_free(StringVector* vec)
{
    if (vec == NULL || vec->data == NULL) {
        return;
    }

    for (int i = 0; i < vec->count; i++) {
        free(vec->data[i]);
    }
    free(vec->data);

    vec->data = NULL;
    vec->count = 0;
    vec->capacity = 0;
}
