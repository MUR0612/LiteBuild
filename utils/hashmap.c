#include "hashmap.h"

#include "memory.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t hash_string(const char* key)
{
    uint32_t hash = 2166136261u;

    while (*key != '\0') {
        hash ^= (unsigned char)(*key);
        hash *= 16777619u;
        key++;
    }
    return hash;
}

HashMap* hashmap_create(size_t capacity)
{
    HashMap* map = xmalloc(sizeof(HashMap));

    map->capacity = capacity > 0 ? capacity : 16;
    map->size = 0;
    map->buckets = xmalloc(sizeof(HashMapEntry*) * map->capacity);
    memset(map->buckets, 0, sizeof(HashMapEntry*) * map->capacity);
    return map;
}

void hashmap_put(HashMap* map, const char* key, void* value)
{
    uint32_t index;
    HashMapEntry* entry;
    HashMapEntry* newEntry;

    if (map == NULL || key == NULL) {
        return;
    }

    index = hash_string(key) % map->capacity;
    entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    newEntry = xmalloc(sizeof(HashMapEntry));
    newEntry->key = xstrdup(key);
    newEntry->value = value;
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
    map->size++;
}

void* hashmap_get(const HashMap* map, const char* key)
{
    uint32_t index;
    HashMapEntry* entry;

    if (map == NULL || key == NULL) {
        return NULL;
    }

    index = hash_string(key) % map->capacity;
    entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void hashmap_free(HashMap* map, void (*free_value_cb)(void*))
{
    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        HashMapEntry* entry = map->buckets[i];
        while (entry != NULL) {
            HashMapEntry* next = entry->next;

            if (free_value_cb != NULL && entry->value != NULL) {
                free_value_cb(entry->value);
            }
            free(entry->key);
            free(entry);
            entry = next;
        }
    }

    free(map->buckets);
    free(map);
}
