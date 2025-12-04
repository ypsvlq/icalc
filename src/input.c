#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "menu.h"
#include "input.h"
#include "hashmap.h"

#define WHITESPACE " \t\n"

static bool input_strtod(double *d, const char *s, char expected_end);

static char input[1024];

void input_prompt(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    if (fgets(input, sizeof input, stdin) == NULL) {
        printf("\n\nbye!\n");
        exit(0);
    }

    va_end(args);
}

void input_execute(void) {
    char *name = strtok(input, WHITESPACE);
    if (name == NULL) {
        return;
    }

    const MenuCommand *command = hm_get(menu_command_map, name);
    if (command == NULL) {
        char *endptr;
        long n = strtol(name, &endptr, 10);
        if (n > 0 && *endptr == '\0') {
            command = menu_commands;
            while (n > 1 && command->name != NULL) {
                command++;
                n--;
            }
        }
    }
    if (command == NULL || command->name == NULL) {
        printf("error: invalid command %s\n", name);
        return;
    }

    double args[MENU_COMMAND_MAX_ARGS];
    char *token = strtok(NULL, WHITESPACE);
    if (token != NULL) {
        for (int i = 0; i < command->args; i++) {
            if (token == NULL) {
                printf("error: missing argument\n");
                return;
            }
            if (!input_strtod(&args[i], token, '\0')) {
                return;
            }
            token = strtok(NULL, WHITESPACE);
        }
        if (token != NULL) {
            printf("error: extra argument\n");
            return;
        }
    } else {
        printf("\n");
        for (int i = 0; i < command->args; i++) {
            input_prompt("enter number %d: ", i + 1);
            if (!input_strtod(&args[i], input, '\n')) {
                return;
            }
        }
    }
    printf("\nresult: %g\n", command->fn(args));
}

static bool input_strtod(double *d, const char *s, char expected_end) {
    char *endptr;
    *d = strtod(s, &endptr);
    if (endptr == s || *endptr != expected_end) {
        printf("error: invalid number %s", s);
        return false;
    }
    return true;
}
