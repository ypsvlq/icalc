#include <stddef.h>
#include <stdbool.h>
#include "menu.h"
#include "hashmap.h"

static double cmd_add(const double *args) {
    return args[0] + args[1];
}

const MenuCommand menu_commands[] = {
    {"add", cmd_add, 2},
    {0},
};

HashMap *menu_command_map;

bool menu_init(void) {
    menu_command_map = hm_init();
    if (menu_command_map == NULL) {
        return false;
    }

    for (const MenuCommand *command = menu_commands; command->name; command++) {
        if (!hm_put(menu_command_map, command->name, (void *)command)) {
            return false;
        }
    }

    return true;
}

void menu_deinit(void) {
    hm_deinit(menu_command_map);
}
