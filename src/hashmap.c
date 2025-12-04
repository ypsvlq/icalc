#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashmap.h"

typedef struct Entry {
    const char *key;
    const void *value;
} Entry;

struct HashMap {
    Entry *entries;
    unsigned size;
    unsigned used;
};

static unsigned long hm_hash(const char *key);
static Entry *hm_entry(HashMap *map, const char *key);
static bool hm_rehash(HashMap *map);

HashMap *hm_init(void) {
    HashMap *map = malloc(sizeof *map);
    if (map == NULL) {
        return NULL;
    }
    map->size = 16;
    map->used = 0;
    map->entries = calloc(map->size, sizeof *map->entries);
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }
    return map;
}

void hm_deinit(HashMap *map) {
    free(map->entries);
    free(map);
}

bool hm_put(HashMap *map, const char *key, const void *value) {
    Entry *entry = hm_entry(map, key);

    if (entry->key == NULL) {
        if (map->used >= map->size / 2) {
            if (!hm_rehash(map)) {
                return false;
            }
            entry = hm_entry(map, key);
        }
        map->used++;
    }

    entry->key = key;
    entry->value = value;

    return true;
}

const void *hm_get(HashMap *map, const char *key) {
    return hm_entry(map, key)->value;
}

// fnv-1a hash function (public domain)
static unsigned long hm_hash(const char *key) {
    unsigned long h = 0x811C9DC5;
    while (*key) {
        h ^= *key++;
        h *= 0x01000193;
    }
    return h;
}

static Entry *hm_entry(HashMap *map, const char *key) {
    size_t idx = hm_hash(key) & (map->size - 1);
    while (map->entries[idx].key && strcmp(map->entries[idx].key, key) != 0) {
        if (++idx == map->size) {
            idx = 0;
        }
    }
    return &map->entries[idx];
}

static bool hm_rehash(HashMap *map) {
    Entry *old_entries = map->entries;
    unsigned old_used = map->used;

    unsigned new_size = map->size * 2;
    Entry *new_entries = calloc(new_size, sizeof *map->entries);
    if (new_entries == NULL) {
        return false;
    }
    map->size = new_size;
    map->entries = new_entries;

    for (unsigned i = 0; old_used > 0; i++) {
        if (old_entries[i].key != NULL) {
            Entry *entry = hm_entry(map, old_entries[i].key);
            *entry = old_entries[i];
            old_used--;
        }
    }

    free(old_entries);
    return true;
}
