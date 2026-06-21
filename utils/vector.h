#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

typedef struct {
    char** data;
    int count;
    int capacity;
} StringVector;

void strvec_init(StringVector* vec);
void strvec_push(StringVector* vec, const char* str);
void strvec_take(StringVector* vec, char* str);
void strvec_free(StringVector* vec);

#endif /* UTILS_VECTOR_H */
