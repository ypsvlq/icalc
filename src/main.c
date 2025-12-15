#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "menu.h"
#include "input.h"
#include "escape.h"

int main(void) {
    printf("- icalc -\n");

    if (!menu_init()) {
        return 1;
    }
    atexit(menu_deinit);
    if (!escape_init()) {
        return 1;
    }
    atexit(escape_deinit);

    while (true) {
        if (menu_folder_depth > 0) {
            printf("\n[");
            printf("%s", menu_folders[1]->name);
            for (int i = 2; i <= menu_folder_depth; i++) {
                printf("/%s", menu_folders[i]->name);
            }
            printf("]\n");
        }

        const MenuFolder *folder = menu_folders[menu_folder_depth];

        printf("\nfolders:\n");
        for (int i = 0; folder->folders[i].name != NULL; i++) {
            printf("  %c. %s\n", i + 'A', folder->folders[i].name);
        }
        if (menu_folder_depth > 0) {
            printf("  Z. (back)\n");
        }

        printf("\ncommands:\n");
        for (int i = 0; folder->commands[i].name != NULL; i++) {
            printf("  %d. %s\n", i + 1, folder->commands[i].name);
        }

        input_prompt("\n>>> ");

        if (input[1] == '\n') {
            if ('A' <= input[0] && input[0] < 'Z') {
                int i = input[0] - 'A';
                const MenuFolder *subfolder = folder->folders;
                while (i > 0 && subfolder->name != NULL) {
                    i--;
                    subfolder++;
                }
                if (i == 0 && subfolder->name != NULL) {
                    menu_folders[++menu_folder_depth] = subfolder;
                    continue;
                }
            } else if (input[0] == 'Z' && menu_folder_depth > 0) {
                menu_folder_depth--;
                continue;
            }
        }

        if (input_execute(&menu_ans, NULL)) {
            double real = creal(menu_ans);
            double imag = cimag(menu_ans);
            printf("\nresult = %g", real);
            if (imag != 0.0) {
                printf("%+gi", imag);
            }
            printf("\n");
        }
        input_prompt("\n(press enter to return to menu) ");
    }
}
