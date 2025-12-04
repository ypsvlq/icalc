#ifndef ICALC_MENU_H
#define ICALC_MENU_H

#include <stdbool.h>
#include "hashmap.h"

#define MENU_COMMAND_MAX_ARGS 2

typedef struct MenuCommand MenuCommand;

struct MenuCommand {
    const char *name;
    double (*fn)(const double *args);
    unsigned char args;
};

// terminated with a zero element
extern const MenuCommand menu_commands[];

extern HashMap *menu_command_map;

bool menu_init(void);
void menu_deinit(void);

#endif
