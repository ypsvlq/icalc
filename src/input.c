#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "input.h"

#define WHITESPACE " \t\n"

static const MenuCommand *input_find(const char *name);

static char input[1024];

void input_prompt(const char *prompt) {
    printf("%s", prompt);

    if (fgets(input, sizeof input, stdin) == NULL) {
        printf("\n\nbye!\n");
        exit(0);
    }
}

void input_execute(void) {
    char *name = strtok(input, WHITESPACE);
    if (name == NULL) {
        return;
    }
    printf("\n");

    const MenuCommand *command = input_find(name);
    if (command == NULL) {
        printf("error: invalid command %s\n", name);
        return;
    }
    command->fn();
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
