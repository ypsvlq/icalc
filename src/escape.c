#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>
#include "escape.h"
#include "hashmap.h"

bool escape_quiet = false;

static HashMap *vars;

static bool esc_exit(EscapeState *state) {
    (void)state;
    printf("\nbye!\n");
    exit(0);
}

static bool esc_quiet(EscapeState *state) {
    (void)state;
    escape_quiet = !escape_quiet;
    return false;
}

static bool esc_set(EscapeState *state) {
    size_t len = strlen(state->arg_token) + 1;
    char *key = malloc(len);
    if (!key) {
        return false;
    }
    memcpy(key, state->arg_token, len);

    complex double *value = malloc(sizeof *value);
    if (!value) {
        free(key);
        return false;
    }
    *value = state->arg_number;

    if (!hm_put(vars, key, value)) {
        free(value);
        free(key);
        return false;
    }

    *state->result = state->arg_number;
    return true;
}

static bool esc_get(EscapeState *state) {
    complex double *value = hm_get(vars, state->arg_token);
    if (value == NULL) {
        printf("error: undefined var %s\n", state->arg_token);
        return false;
    }
    *state->result = *value;
    return true;
}

static bool esc_hex(EscapeState *state) {
    printf("\nresult = 0x%llx\n", (unsigned long long)creal(state->arg_number));
    return false;
}

static const Escape escapes[] = {
    {"exit", esc_exit, ESCAPE_FLAGS_NONE},
    {"quiet", esc_quiet, ESCAPE_FLAGS_NONE},
    {"set", esc_set, ESCAPE_FLAG_RESULT | ESCAPE_FLAG_ARG_TOKEN | ESCAPE_FLAG_ARG_NUMBER},
    {"get", esc_get, ESCAPE_FLAG_RESULT | ESCAPE_FLAG_ARG_TOKEN},
    {"hex", esc_hex, ESCAPE_FLAG_ARG_NUMBER},
    {0},
};

HashMap *escape_map;

bool escape_init(void) {
    escape_map = hm_init(HASHMAP_FLAGS_NONE);
    if (escape_map == NULL) {
        return false;
    }

    for (const Escape *escape = escapes; escape->name != NULL; escape++) {
        if (!hm_put(escape_map, escape->name, (void *)escape)) {
            return false;
        }
    }

    vars = hm_init(HASHMAP_FLAG_FREE_KEYS | HASHMAP_FLAG_FREE_VALUES);
    if (vars == NULL) {
        return false;
    }

    return true;
}

void escape_deinit(void) {
    hm_deinit(vars);
    hm_deinit(escape_map);
}
