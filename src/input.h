#ifndef ICALC_INPUT_H
#define ICALC_INPUT_H

// uses global state: input buffer
void input_prompt(const char *format, ...);

// uses global state: input buffer, strtok
void input_execute(void);

#endif
