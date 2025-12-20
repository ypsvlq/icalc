#ifndef ICALC_ESCAPE_H
#define ICALC_ESCAPE_H

#include <stdbool.h>
#include <complex.h>
#include "hashmap.h"

typedef struct EscapeState EscapeState;
typedef struct Escape Escape;

#define ESCAPE_FLAGS_NONE 0
#define ESCAPE_FLAG_RESULT (1 << 0)
#define ESCAPE_FLAG_ARG_TOKEN (1 << 1)
#define ESCAPE_FLAG_ARG_NUMBER (1 << 2)

struct EscapeState {
    complex double *result;
    const char *arg_token;
    complex double arg_number;
};

struct Escape {
    const char *name;
    bool (*fn)(EscapeState *state);
    unsigned char flags;
};

extern bool escape_quiet;

extern HashMap *escape_map;

bool escape_init(void);
void escape_deinit(void);

#endif
