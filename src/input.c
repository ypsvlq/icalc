#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <complex.h>
#include "menu.h"
#include "input.h"
#include "hashmap.h"
#include "escape.h"

#define WHITESPACE " \t\n"

static bool input_strtod(complex double *d, const char *s, char expected_end);
static bool input_nested(complex double *result, char *token);
static bool input_arg(complex double *result, char *token);
static bool input_escape(complex double *result, const char *name, bool nested);

char input[INPUT_BUFFER_LEN];

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

bool input_execute(complex double *result, const char *name) {
    bool nested = (name != NULL);

    if (name == NULL) {
        name = strtok(input, WHITESPACE);
        if (name == NULL) {
            return false;
        }
    }

    if (name[0] == '\\') {
        return input_escape(result, name, nested);
    }

    const MenuCommand *command = hm_get(menu_command_map, name);
    if (command == NULL) {
        char *endptr;
        long n = strtol(name, &endptr, 10);
        if (n > 0 && *endptr == '\0') {
            command = menu_folders[menu_folder_depth]->commands;
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

    complex double args[MENU_COMMAND_MAX_ARGS];
    char *token = strtok(NULL, WHITESPACE);
    if (token != NULL || nested) {
        for (int i = 0; i < command->args; i++) {
            if (!input_arg(&args[i], token)) {
                return false;
            }
            token = strtok(NULL, WHITESPACE);
        }
        if (token != NULL) {
            printf("error: extra argument\n");
            return false;
        }
    } else if (command->args > 0) {
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

static bool input_escape(complex double *result, const char *name, bool nested) {
    const Escape *escape = hm_get(escape_map, &name[1]);
    if (escape == NULL) {
        printf("error: invalid command %s\n", name);
        return false;
    }

    EscapeState state;

    if ((escape->flags & ESCAPE_FLAG_RESULT) != 0) {
        state.result = result;
    } else {
        if (nested) {
            printf("error: invalid use of escape\n");
            return false;
        }
    }

    if ((escape->flags & ESCAPE_FLAG_ARG_TOKEN) != 0) {
        state.arg_token = strtok(NULL, WHITESPACE);
        if (state.arg_token == NULL) {
            printf("error: missing argument\n");
            return false;
        }
    }

    if ((escape->flags & ESCAPE_FLAG_ARG_NUMBER) != 0) {
        if (!input_arg(&state.arg_number, strtok(NULL, WHITESPACE))) {
            return false;
        }
    }

    if (strtok(NULL, WHITESPACE) != NULL) {
        printf("error: extra argument\n");
        return false;
    }

    return escape->fn(&state);
}

static bool input_arg(complex double *result, char *token) {
    if (token == NULL) {
        printf("error: missing argument\n");
        return false;
    }
    if (token[0] == '(') {
        return input_nested(result, token);
    } else {
        return input_strtod(result, token, '\0');
    }
}

static bool input_nested(complex double *result, char *token) {
    // remove null terminator if it was inserted by strtok
    char *next = strtok(NULL, WHITESPACE);
    if (next != NULL) {
        next[strlen(next)] = ' ';
        input[INPUT_BUFFER_LEN - 1] = '\0';
        token[strlen(token)] = ' ';
    }

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
    token = strtok(bracket, WHITESPACE);
    if (strcmp(token, ")") != 0) {
        printf("error: invalid command %s\n", token);
        return false;
    }

    return true;
}

static bool input_strtod(complex double *d, const char *s, char expected_end) {
    char *endptr;
    *d = strtod(s, &endptr);
    if (endptr == s || *endptr != expected_end) {
        printf("error: invalid number %s", s);
        if (expected_end != '\n') {
            printf("\n");
        }
        return false;
    }
    return true;
}
