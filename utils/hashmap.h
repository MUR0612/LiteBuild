#ifndef UTILS_HASHMAP_H
#define UTILS_HASHMAP_H

#include <stddef.h>

typedef struct HashMapEntry {
    char* key;
    void* value;
    struct HashMapEntry* next;
} HashMapEntry;

typedef struct {
    HashMapEntry** buckets;
    size_t capacity;
    size_t size;
} HashMap;

HashMap* hashmap_create(size_t capacity);
void hashmap_put(HashMap* map, const char* key, void* value);
void* hashmap_get(const HashMap* map, const char* key);
void hashmap_free(HashMap* map, void (*free_value_cb)(void*));

#endif /* UTILS_HASHMAP_H */
