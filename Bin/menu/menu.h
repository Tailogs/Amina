#ifndef MENU_H
#define MENU_H

void init_menu();
void print_menu(char **choices, int n_choices, int highlight);
char* show_menu(char **choices, int n_choices);

#endif // MENU_H
