#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "menu.h"
#include "hashmap.h"

#define PI 3.14159265358979323846

// commands: /

static double cmd_add(const double *args) {
    return args[0] + args[1];
}

static double cmd_sub(const double *args) {
    return args[0] - args[1];
}

static double cmd_mul(const double *args) {
    return args[0] * args[1];
}

static double cmd_div(const double *args) {
    return args[0] / args[1];
}

static double cmd_exit(const double *args) {
    (void)args;
    printf("\nbye!\n");
    exit(0);
}

static const MenuCommand cmds_root[] = {
    {"add", cmd_add, 2},
    {"sub", cmd_sub, 2},
    {"mul", cmd_mul, 2},
    {"div", cmd_div, 2},
    {"exit", cmd_exit, 0},
    {0},
};

// commands: /constants

static double cmd_pi(const double *args) {
    (void)args;
    return PI;
}

static double cmd_phi(const double *args) {
    (void)args;
    return 1.618033988749894;
}

static const MenuCommand cmds_constants[] = {
    {"pi", cmd_pi, 0},
    {"phi", cmd_phi, 0},
    {0},
};

// commands: /constants/physics

static double cmd_c(const double *args) {
    (void)args;
    return 299792458;
}

static double cmd_h(const double *args) {
    (void)args;
    return 6.62607015e-34;
}

static double cmd_hbar(const double *args) {
    (void)args;
    return 1.054571817e-34;
}

static double cmd_k(const double *args) {
    (void)args;
    return 1.380649e-23;
}

static double cmd_e(const double *args) {
    (void)args;
    return 1.602176634e-19;
}

static double cmd_m_e(const double *args) {
    (void)args;
    return 9.109383713928e-31;
}

static double cmd_mu_0(const double *args) {
    (void)args;
    return 1.2566370612720e-6;
}

static double cmd_z_0(const double *args) {
    (void)args;
    return 376.73031341259;
}

static double cmd_epsilon_0(const double *args) {
    (void)args;
    return 8.854187818814e-12;
}

static const MenuCommand cmds_constants_physics[] = {
    {"c", cmd_c, 0},
    {"h", cmd_h, 0},
    {"hbar", cmd_hbar, 0},
    {"k", cmd_k, 0},
    {"e", cmd_e, 0},
    {"m_e", cmd_m_e, 0},
    {"mu_0", cmd_mu_0, 0},
    {"z_0", cmd_z_0, 0},
    {"epsilon_0", cmd_epsilon_0, 0},
    {0},
};

// commands: /scientific

static double cmd_power(const double *args) {
    return pow(args[0], args[1]);
}

static double cmd_nthroot(const double *args) {
    return pow(args[0], 1.0 / args[1]);
}

static double cmd_sqrt(const double *args) {
    return sqrt(args[0]);
}

static double cmd_exp(const double *args) {
    return pow(2.7182818284590452354, args[0]);
}

static double cmd_log(const double *args) {
    return log(args[0]);
}

static double cmd_logb(const double *args) {
    return log(args[1]) / log(args[0]);
}

static double cmd_abs(const double *args) {
    return fabs(args[0]);
}

static double cmd_factorial(const double *args) {
    double result = 1.0;
    for (int i = 2; i <= args[0]; i++) {
        result *= i;
        if (isinf(result)) {
            break;
        }
    }
    return result;
}

static const MenuCommand cmds_scientific[] = {
    {"power", cmd_power, 2},
    {"nthroot", cmd_nthroot, 2},
    {"sqrt", cmd_sqrt, 1},
    {"exp", cmd_exp, 1},
    {"log", cmd_log, 1},
    {"logb", cmd_logb, 2},
    {"abs", cmd_abs, 1},
    {"factorial", cmd_factorial, 1},
    {0},
};

// commands: /scientific/rounding

static double cmd_round(const double *args) {
    return round(args[0]);
}

static double cmd_trunc(const double *args) {
    return trunc(args[0]);
}

static double cmd_floor(const double *args) {
    return floor(args[0]);
}

static double cmd_ceil(const double *args) {
    return ceil(args[0]);
}

static const MenuCommand cmds_scientific_rounding[] = {
    {"round", cmd_round, 1},
    {"trunc", cmd_trunc, 1},
    {"floor", cmd_floor, 1},
    {"ceil", cmd_ceil, 1},
    {0},
};

// commands: /scientific/sets

static double cmd_permutations(const double *args) {
    double d = args[0] - args[1];
    return cmd_factorial(&args[0]) / cmd_factorial(&d);
}

static double cmd_combinations(const double *args) {
    return cmd_permutations(args) / cmd_factorial(&args[1]);
}

static const MenuCommand cmds_scientific_sets[] = {
    {"permutations", cmd_permutations, 2},
    {"combinations", cmd_combinations, 2},
    {0},
};

// commands: /trigonometry

static double cmd_deg_rad(const double *args) {
    return args[0] * PI / 180.0;
}

static double cmd_rad_deg(const double *args) {
    return args[0] * 180.0 / PI;
}

static double cmd_sin(const double *args) {
    return sin(args[0]);
}

static double cmd_cos(const double *args) {
    return cos(args[0]);
}

static double cmd_tan(const double *args) {
    return tan(args[0]);
}

static const MenuCommand cmds_trigonometry[] = {
    {"deg>rad", cmd_deg_rad, 1},
    {"rad>deg", cmd_rad_deg, 1},
    {"sin", cmd_sin, 1},
    {"cos", cmd_cos, 1},
    {"tan", cmd_tan, 1},
    {0},
};

// commands: /trigonometry/inverse

static double cmd_arcsin(const double *args) {
    return asin(args[0]);
}

static double cmd_arccos(const double *args) {
    return acos(args[0]);
}

static double cmd_arctan(const double *args) {
    return atan(args[0]);
}

static const MenuCommand cmds_trigonometry_inverse[] = {
    {"arcsin", cmd_arcsin, 1},
    {"arccos", cmd_arccos, 1},
    {"arctan", cmd_arctan, 1},
    {0},
};

// commands: /trigonometry/hyperbolic

static double cmd_sinh(const double *args) {
    return sinh(args[0]);
}

static double cmd_cosh(const double *args) {
    return cosh(args[0]);
}

static double cmd_tanh(const double *args) {
    return tanh(args[0]);
}

static const MenuCommand cmds_trigonometry_hyperbolic[] = {
    {"sinh", cmd_sinh, 1},
    {"cosh", cmd_cosh, 1},
    {"tanh", cmd_tanh, 1},
    {0},
};

// commands: /trigonometry/hyperbolic/inverse

static double cmd_arsinh(const double *args) {
    return asinh(args[0]);
}

static double cmd_arcosh(const double *args) {
    return acosh(args[0]);
}

static double cmd_artanh(const double *args) {
    return atanh(args[0]);
}

static const MenuCommand cmds_trigonometry_hyperbolic_inverse[] = {
    {"arsinh", cmd_arsinh, 1},
    {"arcosh", cmd_arcosh, 1},
    {"artanh", cmd_artanh, 1},
    {0},
};

// commands: /equations

static double cmd_hypot(const double *args) {
    return sqrt((args[0] * args[0]) + (args[1] * args[1]));
}

static double cmd_quadratic_p(const double *args) {
    double a = args[0];
    double b = args[1];
    double c = args[2];
    return (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
}

static double cmd_quadratic_m(const double *args) {
    double a = args[0];
    double b = args[1];
    double c = args[2];
    return (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
}

static const MenuCommand cmds_equations[] = {
    {"hypot", cmd_hypot, 2},
    {"quadratic+", cmd_quadratic_p, 3},
    {"quadratic-", cmd_quadratic_m, 3},
    {0},
};

// commands: /bitwise

static double cmd_and(const double *args) {
    return (unsigned long long)args[0] & (unsigned long long)args[1];
}

static double cmd_or(const double *args) {
    return (unsigned long long)args[0] | (unsigned long long)args[1];
}

static double cmd_xor(const double *args) {
    return (unsigned long long)args[0] ^ (unsigned long long)args[1];
}

static const MenuCommand cmds_bitwise[] = {
    {"and", cmd_and, 2},
    {"or", cmd_or, 2},
    {"xor", cmd_xor, 2},
    {0},
};

// folders

static const MenuFolder folders_none[] = {{0}};

static const MenuFolder folders_trigonometry_hyperbolic[] = {
    {"inverse", folders_none, cmds_trigonometry_hyperbolic_inverse},
    {0},
};

static const MenuFolder folders_trigonometry[] = {
    {"inverse", folders_none, cmds_trigonometry_inverse},
    {"hyperbolic", folders_trigonometry_hyperbolic, cmds_trigonometry_hyperbolic},
    {0},
};

static const MenuFolder folders_scientific[] = {
    {"rounding", folders_none, cmds_scientific_rounding},
    {"sets", folders_none, cmds_scientific_sets},
    {0},
};

static const MenuFolder folders_constants[] = {
    {"physics", folders_none, cmds_constants_physics},
    {0},
};

static const MenuFolder folders_root[] = {
    {"constants", folders_constants, cmds_constants},
    {"scientific", folders_scientific, cmds_scientific},
    {"trigonometry", folders_trigonometry, cmds_trigonometry},
    {"equations", folders_none, cmds_equations},
    {"bitwise", folders_none, cmds_bitwise},
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
