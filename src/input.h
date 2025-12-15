#ifndef ICALC_INPUT_H
#define ICALC_INPUT_H

#include <stdbool.h>
#include <complex.h>

#define INPUT_BUFFER_LEN 1024

extern char input[INPUT_BUFFER_LEN];

// uses global state: input buffer
void input_prompt(const char *format, ...);

// call input_prompt() then pass NULL for name
//
// uses global state: input buffer, strtok
bool input_execute(complex double *result, const char *name);

#endif
