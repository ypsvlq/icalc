#ifndef ICALC_HASHMAP_H
#define ICALC_HASHMAP_H

#include <stdbool.h>

typedef struct HashMap HashMap; // opaque

#define HASHMAP_FLAGS_NONE 0
#define HASHMAP_FLAG_FREE_KEYS (1 << 0)
#define HASHMAP_FLAG_FREE_VALUES (1 << 1)

// returns NULL on failure
HashMap *hm_init(unsigned char flags);

void hm_deinit(HashMap *map);

bool hm_put(HashMap *map, const char *key, void *value);

// returns NULL if not found
void *hm_get(HashMap *map, const char *key);

#endif
