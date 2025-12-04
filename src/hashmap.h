#ifndef ICALC_HASHMAP_H
#define ICALC_HASHMAP_H

#include <stdbool.h>

typedef struct HashMap HashMap; // opaque

// returns NULL on failure
HashMap *hm_init(void);

void hm_deinit(HashMap *map);

bool hm_put(HashMap *map, const char *key, void *value);

// returns NULL if not found
void *hm_get(HashMap *map, const char *key);

#endif
