// hashmap using open addressing and linear probing

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashmap.h"

typedef struct Entry {
    const char *key;
    void *value;
} Entry;

struct HashMap {
    Entry *entries;
    size_t size;
    size_t used;
    unsigned char flags;
};

static unsigned long hm_hash(const char *key);
static Entry *hm_entry(HashMap *map, const char *key);
static bool hm_rehash(HashMap *map);

HashMap *hm_init(unsigned char flags) {
    HashMap *map = malloc(sizeof *map);
    if (map == NULL) {
        return NULL;
    }
    map->size = 16;
    map->used = 0;
    map->flags = flags;
    map->entries = calloc(map->size, sizeof *map->entries);
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }
    return map;
}

void hm_deinit(HashMap *map) {
    bool free_keys = ((map->flags & HASHMAP_FLAG_FREE_KEYS) != 0);
    bool free_values = ((map->flags & HASHMAP_FLAG_FREE_VALUES) != 0);
    if (free_keys || free_values) {
        for (size_t i = 0; i < map->size; i++) {
            if (map->entries[i].key != NULL) {
                if (free_keys) {
                    free((void *)map->entries[i].key);
                }
                if (free_values) {
                    free(map->entries[i].value);
                }
            }
        }
    }
    free(map->entries);
    free(map);
}

bool hm_put(HashMap *map, const char *key, void *value) {
    Entry *entry = hm_entry(map, key);

    if (entry->key == NULL) {
        if (map->used >= map->size / 2) {
            if (!hm_rehash(map)) {
                return false;
            }
            entry = hm_entry(map, key);
        }
        map->used++;
    } else {
        if ((map->flags & HASHMAP_FLAG_FREE_KEYS) != 0) {
            free((void *)entry->key);
        }
        if ((map->flags & HASHMAP_FLAG_FREE_VALUES) != 0) {
            free(entry->value);
        }
    }

    entry->key = key;
    entry->value = value;

    return true;
}

void *hm_get(HashMap *map, const char *key) {
    // hm_entry returns an empty entry if key is not found
    return hm_entry(map, key)->value;
}

// fnv-1a hash function (public domain)
static unsigned long hm_hash(const char *key) {
    unsigned long hash = 0x811C9DC5;
    while (*key != '\0') {
        hash ^= *key++;
        hash *= 0x01000193;
    }
    return hash;
}

static Entry *hm_entry(HashMap *map, const char *key) {
    size_t i = hm_hash(key) & (map->size - 1); // equivalent to modulo (size is always a power of two)
    while (map->entries[i].key != NULL && strcmp(map->entries[i].key, key) != 0) {
        if (++i == map->size) {
            i = 0;
        }
    }
    return &map->entries[i];
}

static bool hm_rehash(HashMap *map) {
    size_t new_size = map->size * 2;
    if (new_size < map->size) {
        return false;
    }
    Entry *new_entries = calloc(new_size, sizeof *map->entries);
    if (new_entries == NULL) {
        return false;
    }
    Entry *old_entries = map->entries;
    map->size = new_size;
    map->entries = new_entries;

    size_t remaining = map->used;
    for (size_t i = 0; remaining > 0; i++) {
        if (old_entries[i].key != NULL) {
            Entry *entry = hm_entry(map, old_entries[i].key);
            *entry = old_entries[i];
            remaining--;
        }
    }

    free(old_entries);
    return true;
}
