#include <stdio.h>
#include "menu.h"

static void cmd_add(void) {
    printf("add\n");
}

const MenuCommand menu_commands[] = {
    {"add", cmd_add},
    {0},
};
