#include <stdbool.h>
#include <stdio.h>
#include "menu.h"
#include "input.h"

int main(void) {
    printf("- icalc -\n");

    while (true) {
        printf("\ncommands:\n");
        for (size_t i = 0; menu_commands[i].name != NULL; i++) {
            printf("  %zu. %s\n", i + 1, menu_commands[i].name);
        }

        input_prompt("\n>>> ");
        input_execute();
        input_prompt("\n(press enter to return to menu) ");
    }
}
