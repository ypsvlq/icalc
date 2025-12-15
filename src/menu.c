#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "menu.h"
#include "hashmap.h"

#define PI 3.14159265358979323846

// commands: /

static complex double cmd_add(const complex double *args) {
    return args[0] + args[1];
}

static complex double cmd_sub(const complex double *args) {
    return args[0] - args[1];
}

static complex double cmd_mul(const complex double *args) {
    return args[0] * args[1];
}

static complex double cmd_div(const complex double *args) {
    return args[0] / args[1];
}

static complex double cmd_exit(const complex double *args) {
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

static complex double cmd_pi(const complex double *args) {
    (void)args;
    return PI;
}

static complex double cmd_phi(const complex double *args) {
    (void)args;
    return 1.618033988749894;
}

static const MenuCommand cmds_constants[] = {
    {"pi", cmd_pi, 0},
    {"phi", cmd_phi, 0},
    {0},
};

// commands: /constants/physics

static complex double cmd_c(const complex double *args) {
    (void)args;
    return 299792458;
}

static complex double cmd_h(const complex double *args) {
    (void)args;
    return 6.62607015e-34;
}

static complex double cmd_hbar(const complex double *args) {
    (void)args;
    return 1.054571817e-34;
}

static complex double cmd_k(const complex double *args) {
    (void)args;
    return 1.380649e-23;
}

static complex double cmd_e(const complex double *args) {
    (void)args;
    return 1.602176634e-19;
}

static complex double cmd_m_e(const complex double *args) {
    (void)args;
    return 9.109383713928e-31;
}

static complex double cmd_mu_0(const complex double *args) {
    (void)args;
    return 1.2566370612720e-6;
}

static complex double cmd_z_0(const complex double *args) {
    (void)args;
    return 376.73031341259;
}

static complex double cmd_epsilon_0(const complex double *args) {
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

static complex double cmd_power(const complex double *args) {
    return cpow(args[0], args[1]);
}

static complex double cmd_nthroot(const complex double *args) {
    return cpow(args[0], 1.0 / args[1]);
}

static complex double cmd_sqrt(const complex double *args) {
    return csqrt(args[0]);
}

static complex double cmd_exp(const complex double *args) {
    return cexp(args[0]);
}

static complex double cmd_log(const complex double *args) {
    return clog(args[0]);
}

static complex double cmd_logb(const complex double *args) {
    return clog(args[1]) / clog(args[0]);
}

static complex double cmd_abs(const complex double *args) {
    return cabs(args[0]);
}

static complex double cmd_factorial(const complex double *args) {
    double result = 1.0;
    for (double i = 2; i <= creal(args[0]); i++) {
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

static complex double cmd_round(const complex double *args) {
    return round(creal(args[0]));
}

static complex double cmd_trunc(const complex double *args) {
    return trunc(creal(args[0]));
}

static complex double cmd_floor(const complex double *args) {
    return floor(creal(args[0]));
}

static complex double cmd_ceil(const complex double *args) {
    return ceil(creal(args[0]));
}

static const MenuCommand cmds_scientific_rounding[] = {
    {"round", cmd_round, 1},
    {"trunc", cmd_trunc, 1},
    {"floor", cmd_floor, 1},
    {"ceil", cmd_ceil, 1},
    {0},
};

// commands: /scientific/sets

static complex double cmd_permutations(const complex double *args) {
    complex double d = args[0] - args[1];
    return cmd_factorial(&args[0]) / cmd_factorial(&d);
}

static complex double cmd_combinations(const complex double *args) {
    return cmd_permutations(args) / cmd_factorial(&args[1]);
}

static const MenuCommand cmds_scientific_sets[] = {
    {"permutations", cmd_permutations, 2},
    {"combinations", cmd_combinations, 2},
    {0},
};

// commands: /trigonometry

static complex double cmd_deg_rad(const complex double *args) {
    return args[0] * PI / 180.0;
}

static complex double cmd_rad_deg(const complex double *args) {
    return args[0] * 180.0 / PI;
}

static complex double cmd_sin(const complex double *args) {
    return csin(args[0]);
}

static complex double cmd_cos(const complex double *args) {
    return ccos(args[0]);
}

static complex double cmd_tan(const complex double *args) {
    return ctan(args[0]);
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

static complex double cmd_arcsin(const complex double *args) {
    return casin(args[0]);
}

static complex double cmd_arccos(const complex double *args) {
    return cacos(args[0]);
}

static complex double cmd_arctan(const complex double *args) {
    return catan(args[0]);
}

static const MenuCommand cmds_trigonometry_inverse[] = {
    {"arcsin", cmd_arcsin, 1},
    {"arccos", cmd_arccos, 1},
    {"arctan", cmd_arctan, 1},
    {0},
};

// commands: /trigonometry/hyperbolic

static complex double cmd_sinh(const complex double *args) {
    return csinh(args[0]);
}

static complex double cmd_cosh(const complex double *args) {
    return ccosh(args[0]);
}

static complex double cmd_tanh(const complex double *args) {
    return ctanh(args[0]);
}

static const MenuCommand cmds_trigonometry_hyperbolic[] = {
    {"sinh", cmd_sinh, 1},
    {"cosh", cmd_cosh, 1},
    {"tanh", cmd_tanh, 1},
    {0},
};

// commands: /trigonometry/hyperbolic/inverse

static complex double cmd_arsinh(const complex double *args) {
    return casinh(args[0]);
}

static complex double cmd_arcosh(const complex double *args) {
    return cacosh(args[0]);
}

static complex double cmd_artanh(const complex double *args) {
    return catanh(args[0]);
}

static const MenuCommand cmds_trigonometry_hyperbolic_inverse[] = {
    {"arsinh", cmd_arsinh, 1},
    {"arcosh", cmd_arcosh, 1},
    {"artanh", cmd_artanh, 1},
    {0},
};

// commands: /equations

static complex double cmd_hypot(const complex double *args) {
    return csqrt((args[0] * args[0]) + (args[1] * args[1]));
}

static complex double cmd_quadratic_p(const complex double *args) {
    complex double a = args[0];
    complex double b = args[1];
    complex double c = args[2];
    return (-b + csqrt((b * b) - (4 * a * c))) / (2 * a);
}

static complex double cmd_quadratic_m(const complex double *args) {
    complex double a = args[0];
    complex double b = args[1];
    complex double c = args[2];
    return (-b - csqrt((b * b) - (4 * a * c))) / (2 * a);
}

static const MenuCommand cmds_equations[] = {
    {"hypot", cmd_hypot, 2},
    {"quadratic+", cmd_quadratic_p, 3},
    {"quadratic-", cmd_quadratic_m, 3},
    {0},
};

// commands: /bitwise

static complex double cmd_and(const complex double *args) {
    return (unsigned long long)creal(args[0]) & (unsigned long long)creal(args[1]);
}

static complex double cmd_or(const complex double *args) {
    return (unsigned long long)creal(args[0]) | (unsigned long long)creal(args[1]);
}

static complex double cmd_xor(const complex double *args) {
    return (unsigned long long)creal(args[0]) ^ (unsigned long long)creal(args[1]);
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
    menu_command_map = hm_init(HASHMAP_FLAGS_NONE);
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
