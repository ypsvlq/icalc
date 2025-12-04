#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "input.h"

int main(void) {
    printf("- icalc -\n");

    menu_init();
    atexit(menu_deinit);

    while (true) {
        printf("\ncommands:\n");
        for (size_t i = 0; menu_commands[i].name != NULL; i++) {
            printf("  %zu. %s\n", i + 1, menu_commands[i].name);
        }

        input_prompt("\n>>> ");
        double result;
        if (input_execute(&result, NULL)) {
            printf("\nresult = %g\n", result);
        }
        input_prompt("\n(press enter to return to menu) ");
    }
}
