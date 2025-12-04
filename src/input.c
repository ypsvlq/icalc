#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "menu.h"
#include "input.h"

#define WHITESPACE " \t\n"

static const MenuCommand *input_find(const char *name);
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

    const MenuCommand *command = input_find(name);
    if (command == NULL) {
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

static const MenuCommand *input_find(const char *name) {
    char *endptr;
    long n = strtol(name, &endptr, 10);
    long i = n;
    for (const MenuCommand *command = menu_commands; command->name != NULL; command++) {
        if (n != 0 && *endptr == '\0') {
            if (--i == 0) {
                return command;
            }
        } else {
            if (strcmp(command->name, name) == 0) {
                return command;
            }
        }
    }
    return NULL;
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
