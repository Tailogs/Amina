// stringFunc.c

#include <stdio.h>
#include <ctype.h>

// Function to convert a string to lowercase
void toLowerCase(char* str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// Function to convert a string to uppercase
void toUpperCase(char* str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

// Function to convert a character to lowercase
char charToLower(char c) {
    return tolower(c);
}

// Function to convert a character to uppercase
char charToUpper(char c) {
    return toupper(c);
}

