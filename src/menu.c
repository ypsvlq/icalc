#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "menu.h"
#include "hashmap.h"

// commands: /

static double cmd_add(const double *args) {
    return args[0] + args[1];
}

static const MenuCommand cmds_root[] = {
    {"add", cmd_add, 2},
    {0},
};

// commands: /trigonometry

static double cmd_sin(const double *args) {
    return sin(args[0]);
}

static const MenuCommand cmds_trigonometry[] = {
    {"sin", cmd_sin, 1},
    {0},
};

// commands: /trigonometry/inverse

static double cmd_arcsin(const double *args) {
    return asin(args[0]);
}

static const MenuCommand cmds_trigonometry_inverse[] = {
    {"arcsin", cmd_arcsin, 1},
    {0},
};

// folders

static const MenuFolder folders_none[] = {{0}};

static const MenuFolder folders_trigonometry[] = {
    {"inverse", folders_none, cmds_trigonometry_inverse},
    {0},
};

static const MenuFolder folders_root[] = {
    {"trigonometry", folders_trigonometry, cmds_trigonometry},
    {0},
};

static const MenuFolder root = {NULL, folders_root, cmds_root};

// api

const MenuFolder *menu_folders[MENU_FOLDER_MAX_DEPTH] = {&root};
unsigned char menu_folder_depth = 0;

HashMap *menu_command_map;

static bool menu_init_folder(const MenuFolder *folder);

bool menu_init(void) {
    menu_command_map = hm_init();
    if (menu_command_map == NULL) {
        return false;
    }
    if (!menu_init_folder(&root)) {
        return false;
    }
    return true;
}

void menu_deinit(void) {
    hm_deinit(menu_command_map);
}

static bool menu_init_folder(const MenuFolder *folder) {
    for (const MenuCommand *command = folder->commands; command->name != NULL; command++) {
        if (!hm_put(menu_command_map, command->name, (void *)command)) {
            return false;
        }
    }

    for (const MenuFolder *subfolder = folder->folders; subfolder->name != NULL; subfolder++) {
        if (!menu_init_folder(subfolder)) {
            return false;
        }
    }

    return true;
}
