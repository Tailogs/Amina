#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../color/color.h"
#include <conio.h>  // Для функции getch()

void init_menu() {
    // Инициализация может быть не нужна для простого текстового меню
}

void print_menu(char **choices, int n_choices, int highlight) {
    system("cls");  // Очистка экрана в Windows, используйте "clear" для Unix/Linux
    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i) {
            setColor("bright_magenta");
            printf("> %s\n", choices[i]);  // Выделение текущего пункта
            resetColor();
        } else {
            printf("  %s\n", choices[i]);
        }
    }
}

char* show_menu(char **choices, int n_choices) {
    int highlight = 0;
    int c;

    while (1) {
        print_menu(choices, n_choices, highlight);
        c = getch();

        if (c == 0 || c == 224) {
            // Это расширенная клавиша, считываем следующий байт
            switch (getch()) {
                case 72:  // Стрелка вверх
                    if (highlight == 0) {
                        highlight = n_choices - 1;
                    } else {
                        --highlight;
                    }
                    break;
                case 80:  // Стрелка вниз
                    if (highlight == n_choices - 1) {
                        highlight = 0;
                    } else {
                        ++highlight;
                    }
                    break;
                default:
                    break;
            }
        } else if (c == '\r') {
            // Нажата клавиша Enter
            return strdup(choices[highlight]);
        } else if (c == 27) {
            // Нажата клавиша Esc
            return NULL;  // Возвращаем NULL для обозначения выхода из меню
        } else if (c == 3) {
            // Нажата комбинация Ctrl+C
            return NULL;  // Возвращаем NULL для обозначения выхода из меню
        }
    }
}
