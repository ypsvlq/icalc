#ifndef ICALC_INPUT_H
#define ICALC_INPUT_H

#include <stdbool.h>

// uses global state: input buffer
void input_prompt(const char *format, ...);

// call input_prompt() then pass NULL for name
//
// uses global state: input buffer, strtok
bool input_execute(double *result, const char *name);

#endif
