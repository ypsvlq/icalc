#ifndef ICALC_MENU_H
#define ICALC_MENU_H

#include <stdbool.h>
#include "hashmap.h"

#define MENU_COMMAND_MAX_ARGS 3
#define MENU_FOLDER_MAX_DEPTH 4

typedef struct MenuCommand MenuCommand;
typedef struct MenuFolder MenuFolder;

struct MenuCommand {
    const char *name;
    double (*fn)(const double *args);
    unsigned char args;
};

struct MenuFolder {
    const char *name;
    const MenuFolder *folders; // terminated with a zero element
    const MenuCommand *commands; // terminated with a zero element
};

extern const MenuFolder *menu_folders[MENU_FOLDER_MAX_DEPTH];
extern unsigned char menu_folder_depth;

extern HashMap *menu_command_map;

bool menu_init(void);
void menu_deinit(void);

#endif
