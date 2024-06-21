#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <Shlwapi.h>
#include <ctype.h>
#include "nano.h"

#define MAX_PATH_LEN 256
#define MAX_LINES 1000
#define MAX_LINE_LEN 256

const char* keywords[] = {
    "int", "char", "if", "else", "for", "while", "return", "void", "define", "main",
    "float", "double", "struct", "typedef", "enum", "const", "static", "extern", "sizeof", "NULL",
    "break", "continue", "switch", "case", "default", "goto", "auto", "register", "signed", "unsigned",
    "short", "long", "volatile", "union", "restrict", "class", "public", "private", "protected", "template",
    "typename", "namespace", "using", "new", "delete", "virtual", "override", "friend", "this"
};
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

const char* functions[] = {
    "printf", "scanf", "cin", "cout", "gets", "puts", "fopen", "fclose", "fread", "fwrite",
    "memcpy", "memset", "strlen", "strcmp", "strcpy", "sprintf", "sscanf", "malloc", "free",
    "exit", "abort", "system", "time", "clock"
};
const int function_count = sizeof(functions) / sizeof(functions[0]);

void highlightKeywords(const char *line) {
    char buffer[MAX_LINE_LEN * 2];
    buffer[0] = '\0';
    const char *start = line;
    while (*start) {
        int found = 0;
        for (int i = 0; i < keyword_count; ++i) {
            size_t len = strlen(keywords[i]);
            if (strncmp(start, keywords[i], len) == 0 && (start[len] == ' ' || start[len] == '\0' || start[len] == '(')) {
                strcat(buffer, "\033[31m"); // Красный цвет для ключевых слов
                strncat(buffer, start, len);
                strcat(buffer, "\033[0m");
                start += len;
                found = 1;
                break;
            }
        }
        if (found) continue;

        for (int i = 0; i < function_count; ++i) {
            size_t len = strlen(functions[i]);
            if (strncmp(start, functions[i], len) == 0 && (start[len] == ' ' || start[len] == '\0' || start[len] == '(')) {
                strcat(buffer, "\033[34m"); // Синий цвет для функций
                strncat(buffer, start, len);
                strcat(buffer, "\033[0m");
                start += len;
                found = 1;
                break;
            }
        }
        if (found) continue;

        if (strncmp(start, "#include", 8) == 0 && (start[8] == ' ' || start[8] == '<' || start[8] == '"')) {
            strcat(buffer, "\033[35m"); // Магента цвет для препроцессорных директив
            strncat(buffer, start, 8);
            strcat(buffer, "\033[0m");
            start += 8;
            found = 1;
        }
        if (found) continue;

        if (*start == '"' || *start == '\'') {
            char quote = *start;
            strcat(buffer, "\033[32m"); // Зеленый цвет для строк и символов
            strncat(buffer, start, 1);
            start++;
            while (*start && *start != quote) {
                strncat(buffer, start, 1);
                start++;
            }
            if (*start == quote) {
                strncat(buffer, start, 1);
                start++;
            }
            strcat(buffer, "\033[0m");
            found = 1;
        }
        if (found) continue;

        if (isdigit(*start)) {
            strcat(buffer, "\033[33m"); // Желтый цвет для чисел
            while (isdigit(*start) || *start == '.') {
                strncat(buffer, start, 1);
                start++;
            }
            strcat(buffer, "\033[0m");
            found = 1;
        }
        if (found) continue;

        strncat(buffer, start, 1);
        start++;
    }
    printf("%s", buffer);
}

void RunNano(char *filename, char *currentDirectory) {
    SetConsoleOutputCP(CP_UTF8); // Устанавливаем кодировку UTF-8 для консоли

    char fullPath[MAX_PATH_LEN];
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", currentDirectory, filename);

    FILE *file = fopen(fullPath, "r+");
    if (file == NULL) {
        // Создаем файл, если он не существует
        file = fopen(fullPath, "w+");
        if (file == NULL) {
            printf("Error creating file: %s\n", fullPath);
            return;
        }
    }

    // Читаем содержимое файла
    char lines[MAX_LINES][MAX_LINE_LEN];
    int lineCount = 0;
    while (fgets(lines[lineCount], MAX_LINE_LEN, file) != NULL) {
        lines[lineCount][strcspn(lines[lineCount], "\r\n")] = 0; // Убираем символы новой строки
        lineCount++;
    }

    // Убедитесь, что есть хотя бы одна строка для редактирования
    if (lineCount == 0) {
        strcpy(lines[0], "");
        lineCount = 1;
    }

    int currentLineIndex = 0;
    int currentColumnIndex = 0;

    printf("Editing file: %s. Use Ctrl+S to save, Ctrl+C to cancel, and Esc to save and exit.\n", filename);

    while (1) {
        system("cls");

        // Отображаем строки с подсветкой ключевых слов
        for (int i = 0; i < lineCount; i++) {
            if (i == currentLineIndex) {
                printf("\033[36m%4d \033[33m", i + 1); // Подсвечиваем номер текущей строки
                highlightKeywords(lines[i]);
                printf("\033[0m\n");
            } else {
                printf("%4d ", i + 1);
                highlightKeywords(lines[i]);
                printf("\n");
            }
        }

        // Чтение ввода пользователя
        int key = _getch();

        if (key == 0 || key == 224) {
            switch (_getch()) {
                case 72: // Вверх
                    if (currentLineIndex > 0) currentLineIndex--;
                    if (currentColumnIndex >= strlen(lines[currentLineIndex])) currentColumnIndex = strlen(lines[currentLineIndex]);
                    break;
                case 80: // Вниз
                    if (currentLineIndex < lineCount - 1) currentLineIndex++;
                    if (currentColumnIndex >= strlen(lines[currentLineIndex])) currentColumnIndex = strlen(lines[currentLineIndex]);
                    break;
                case 75: // Влево
                    if (currentColumnIndex > 0) currentColumnIndex--;
                    break;
                case 77: // Вправо
                    if (currentColumnIndex < strlen(lines[currentLineIndex])) currentColumnIndex++;
                    break;
            }
        } else if (key == 3) { // Ctrl+C
            printf("Exiting nano editor without saving changes to '%s'.\n", filename);
            fclose(file);
            return;
        } else if (key == 19) { // Ctrl+S
            freopen(fullPath, "w", file);
            for (int i = 0; i < lineCount; i++) {
                fprintf(file, "%s\n", lines[i]);
            }
            fclose(file);
            printf("File '%s' saved.\n", filename);
            return;
        } else if (key == 27) { // Escape
            freopen(fullPath, "w", file);
            for (int i = 0; i < lineCount; i++) {
                fprintf(file, "%s\n", lines[i]);
            }
            fclose(file);
            printf("File '%s' saved.\n", filename);
            return;
        } else if (key == 8) { // Backspace
            if (currentColumnIndex > 0) {
                memmove(&lines[currentLineIndex][currentColumnIndex - 1], &lines[currentLineIndex][currentColumnIndex], strlen(lines[currentLineIndex]) - currentColumnIndex + 1);
                currentColumnIndex--;
            } else if (currentLineIndex > 0) {
                currentColumnIndex = strlen(lines[currentLineIndex - 1]);
                strcat(lines[currentLineIndex - 1], lines[currentLineIndex]);
                memmove(&lines[currentLineIndex], &lines[currentLineIndex + 1], (lineCount - currentLineIndex) * sizeof(lines[0]));
                lineCount--;
                currentLineIndex--;
            }
        } else if (key == 13) { // Enter
            memmove(&lines[currentLineIndex + 1], &lines[currentLineIndex], (lineCount - currentLineIndex) * sizeof(lines[0]));
            lineCount++;
            strncpy(lines[currentLineIndex + 1], &lines[currentLineIndex][currentColumnIndex], MAX_LINE_LEN - 1);
            lines[currentLineIndex + 1][MAX_LINE_LEN - 1] = '\0';
            lines[currentLineIndex][currentColumnIndex] = '\0';
            currentLineIndex++;
            currentColumnIndex = 0;
        } else if (isprint(key)) {
            memmove(&lines[currentLineIndex][currentColumnIndex + 1], &lines[currentLineIndex][currentColumnIndex], strlen(lines[currentLineIndex]) - currentColumnIndex + 1);
            lines[currentLineIndex][currentColumnIndex] = key;
            currentColumnIndex++;
        }
    }
}
