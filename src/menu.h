#ifndef ICALC_MENU_H
#define ICALC_MENU_H

typedef struct MenuCommand MenuCommand;

struct MenuCommand {
    const char *name;
    void (*fn)(void);
};

// terminated with a zero element
extern const MenuCommand menu_commands[];

#endif
