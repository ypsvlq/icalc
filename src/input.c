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
static bool input_nested(double *result, char *token);

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

bool input_execute(double *result, const char *name) {
    if (name == NULL) {
        name = strtok(input, WHITESPACE);
        if (name == NULL) {
            return false;
        }
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
        return false;
    }

    double args[MENU_COMMAND_MAX_ARGS];
    char *token = strtok(NULL, WHITESPACE);
    if (token != NULL) {
        for (int i = 0; i < command->args; i++) {
            if (token == NULL) {
                printf("error: missing argument\n");
                return false;
            }
            if (token[0] == '(') {
                if (!input_nested(&args[i], token)) {
                    return false;
                }
            } else {
                if (!input_strtod(&args[i], token, '\0')) {
                    return false;
                }
            }
            token = strtok(NULL, WHITESPACE);
        }
        if (token != NULL) {
            printf("error: extra argument\n");
            return false;
        }
    } else {
        printf("\n");
        for (int i = 0; i < command->args; i++) {
            input_prompt("enter number %d: ", i + 1);
            if (!input_strtod(&args[i], input, '\n')) {
                return false;
            }
        }
    }
    *result = command->fn(args);
    return true;
}

static bool input_nested(double *result, char *token) {
    token[strlen(token)] = ' '; // remove '\0' inserted by strtok

    // find matching close bracket
    char *bracket = token;
    unsigned depth = 1;
    while (depth > 0) {
        bracket = strpbrk(bracket + 1, "()");
        if (bracket == NULL) {
            printf("error: missing close bracket\n");
            return false;
        } else if (*bracket == '(') {
            depth++;
        } else {
            depth--;
        }
    }

    token++; // move past open bracket
    *bracket = '\0'; // stop nested strtok on close bracket

    const char *name = strtok(token, WHITESPACE);
    if (name == NULL) {
        printf("error: missing command name\n");
        return false;
    }
    if (!input_execute(result, name)) {
        return false;
    }

    // check for extra characters after close bracket
    *bracket = ')';
    token = strtok(NULL, WHITESPACE);
    if (strcmp(token, ")") != 0) {
        printf("error: invalid command %s\n", token);
        return false;
    }

    return true;
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
