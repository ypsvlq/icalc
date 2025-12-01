#include "menu.h"

static double cmd_add(const double *args) {
    return args[0] + args[1];
}

const MenuCommand menu_commands[] = {
    {"add", cmd_add, 2},
    {0},
};
