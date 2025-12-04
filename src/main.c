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
            printf("  %c. %s\n", i + 'a', folder->folders[i].name);
        }
        if (menu_folder_depth > 0) {
            printf("  z. (back)\n");
        }

        printf("\ncommands:\n");
        for (int i = 0; folder->commands[i].name != NULL; i++) {
            printf("  %d. %s\n", i + 1, folder->commands[i].name);
        }

        input_prompt("\n>>> ");

        if (input[1] == '\n') {
            if ('a' <= input[0] && input[0] < 'z') {
                int i = input[0] - 'a';
                const MenuFolder *subfolder = folder->folders;
                while (i > 0 && subfolder->name != NULL) {
                    i--;
                    subfolder++;
                }
                if (i == 0 && subfolder->name != NULL) {
                    menu_folders[++menu_folder_depth] = subfolder;
                    continue;
                }
            } else if (input[0] == 'z' && menu_folder_depth > 0) {
                menu_folder_depth--;
                continue;
            }
        }

        double result;
        if (input_execute(&result, NULL)) {
            printf("\nresult = %g\n", result);
        }
        input_prompt("\n(press enter to return to menu) ");
    }
}
