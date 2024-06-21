// color.c

#include "color.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>

// ANSI escape sequences for colors
const char* ANSI_RESET = "\033[0m";
const char* ANSI_BLACK = "\033[30m";
const char* ANSI_RED = "\033[31m";
const char* ANSI_GREEN = "\033[32m";
const char* ANSI_YELLOW = "\033[33m";
const char* ANSI_BLUE = "\033[34m";
const char* ANSI_MAGENTA = "\033[35m";
const char* ANSI_CYAN = "\033[36m";
const char* ANSI_WHITE = "\033[37m";
const char* ANSI_PURPLE = "\033[35m"; 
const char* ANSI_BRIGHT_BLACK = "\033[90m";
const char* ANSI_BRIGHT_RED = "\033[91m";
const char* ANSI_BRIGHT_GREEN = "\033[92m";
const char* ANSI_BRIGHT_YELLOW = "\033[93m";
const char* ANSI_BRIGHT_BLUE = "\033[94m";
const char* ANSI_BRIGHT_MAGENTA = "\033[95m";
const char* ANSI_BRIGHT_CYAN = "\033[96m";
const char* ANSI_BRIGHT_WHITE = "\033[97m";

// ANSI escape sequences for background colors
const char* ANSI_BG_BLACK = "\033[40m";
const char* ANSI_BG_RED = "\033[41m";
const char* ANSI_BG_GREEN = "\033[42m";
const char* ANSI_BG_YELLOW = "\033[43m";
const char* ANSI_BG_BLUE = "\033[44m";
const char* ANSI_BG_MAGENTA = "\033[45m";
const char* ANSI_BG_CYAN = "\033[46m";
const char* ANSI_BG_WHITE = "\033[47m";
const char* ANSI_BG_PURPLE = "\033[45m";  
const char* ANSI_BG_BRIGHT_BLACK = "\033[100m";
const char* ANSI_BG_BRIGHT_RED = "\033[101m";
const char* ANSI_BG_BRIGHT_GREEN = "\033[102m";
const char* ANSI_BG_BRIGHT_YELLOW = "\033[103m";
const char* ANSI_BG_BRIGHT_BLUE = "\033[104m";
const char* ANSI_BG_BRIGHT_MAGENTA = "\033[105m";
const char* ANSI_BG_BRIGHT_CYAN = "\033[106m";
const char* ANSI_BG_BRIGHT_WHITE = "\033[107m";
const char* ANSI_BG_RESET = "\033[0m";

// Function to set the text color
void setColor(const char* color) {
    if (strcmp(color, "black") == 0) {
        printf("%s", ANSI_BLACK);
    } else if (strcmp(color, "red") == 0) {
        printf("%s", ANSI_RED);
    } else if (strcmp(color, "green") == 0) {
        printf("%s", ANSI_GREEN);
    } else if (strcmp(color, "yellow") == 0) {
        printf("%s", ANSI_YELLOW);
    } else if (strcmp(color, "blue") == 0) {
        printf("%s", ANSI_BLUE);
    } else if (strcmp(color, "magenta") == 0) {
        printf("%s", ANSI_MAGENTA);
    } else if (strcmp(color, "cyan") == 0) {
        printf("%s", ANSI_CYAN);
    } else if (strcmp(color, "white") == 0) {
        printf("%s", ANSI_WHITE);
    } else if (strcmp(color, "purple") == 0) {
        printf("%s", ANSI_PURPLE);
    } else if (strcmp(color, "bright_black") == 0) {
        printf("%s", ANSI_BRIGHT_BLACK);
    } else if (strcmp(color, "bright_red") == 0) {
        printf("%s", ANSI_BRIGHT_RED);
    } else if (strcmp(color, "bright_green") == 0) {
        printf("%s", ANSI_BRIGHT_GREEN);
    } else if (strcmp(color, "bright_yellow") == 0) {
        printf("%s", ANSI_BRIGHT_YELLOW);
    } else if (strcmp(color, "bright_blue") == 0) {
        printf("%s", ANSI_BRIGHT_BLUE);
    } else if (strcmp(color, "bright_magenta") == 0) {
        printf("%s", ANSI_BRIGHT_MAGENTA);
    } else if (strcmp(color, "bright_cyan") == 0) {
        printf("%s", ANSI_BRIGHT_CYAN);
    } else if (strcmp(color, "bright_white") == 0) {
        printf("%s", ANSI_BRIGHT_WHITE);
    } else {
        printf("%s", ANSI_RESET);
    }
}

// Function to set the background color
void setBackground(const char* color) {
    if (strcmp(color, "black") == 0) {
        printf("%s", ANSI_BG_BLACK);
    } else if (strcmp(color, "red") == 0) {
        printf("%s", ANSI_BG_RED);
    } else if (strcmp(color, "green") == 0) {
        printf("%s", ANSI_BG_GREEN);
    } else if (strcmp(color, "yellow") == 0) {
        printf("%s", ANSI_BG_YELLOW);
    } else if (strcmp(color, "blue") == 0) {
        printf("%s", ANSI_BG_BLUE);
    } else if (strcmp(color, "magenta") == 0) {
        printf("%s", ANSI_BG_MAGENTA);
    } else if (strcmp(color, "cyan") == 0) {
        printf("%s", ANSI_BG_CYAN);
    } else if (strcmp(color, "white") == 0) {
        printf("%s", ANSI_BG_WHITE);
    } else if (strcmp(color, "purple") == 0) {
        printf("%s", ANSI_BG_PURPLE);
    } else if (strcmp(color, "bright_black") == 0) {
        printf("%s", ANSI_BG_BRIGHT_BLACK);
    } else if (strcmp(color, "bright_red") == 0) {
        printf("%s", ANSI_BG_BRIGHT_RED);
    } else if (strcmp(color, "bright_green") == 0) {
        printf("%s", ANSI_BG_BRIGHT_GREEN);
    } else if (strcmp(color, "bright_yellow") == 0) {
        printf("%s", ANSI_BG_BRIGHT_YELLOW);
    } else if (strcmp(color, "bright_blue") == 0) {
        printf("%s", ANSI_BG_BRIGHT_BLUE);
    } else if (strcmp(color, "bright_magenta") == 0) {
        printf("%s", ANSI_BG_BRIGHT_MAGENTA);
    } else if (strcmp(color, "bright_cyan") == 0) {
        printf("%s", ANSI_BG_BRIGHT_CYAN);
    } else if (strcmp(color, "bright_white") == 0) {
        printf("%s", ANSI_BG_BRIGHT_WHITE);
    } else {
        printf("%s", ANSI_BG_RESET);
    }
}

// Function to reset the color to default
void resetColor() {
    printf("%s%s", ANSI_RESET, ANSI_BG_RESET); // Сбросить и цвет текста, и цвет фона
}

void enableAnsiSupport() {
    // Получаем дескриптор стандартного вывода (консоли)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Проверяем, что дескриптор валиден
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }
    
    // Получаем текущее состояние консоли
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }
    
    // Включаем режим обработки виртуальных терминалов (анси коды)
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return;
    }
}

void printError1(const char *text) {
    setBackground("white");
    printf("  ");
    setBackground("red");
    setColor("white");
    printf(" ");
    printf(text);
    printf(" \n");
    resetColor();
}


void printError2(const char *text1, const char *text2) {
    setBackground("white");
    printf("  ");
    setBackground("red");
    setColor("white");
    printf(" ");
    printf(text1);
    printf(" ");
    printf(text2);
    printf(" \n");
    resetColor();
}