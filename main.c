#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include <Shlwapi.h>
#include <ctype.h>
#include <malloc.h>
#include <wchar.h>
#include <locale.h>
#include <shlwapi.h>
#include "Bin/nano/nano.h"
#include "Bin/portscanner/portscanner.h"
#include "Bin/chat/client.h"
#include "Bin/chat/server.h"
#include "Bin/color/color.h"
#include "Bin/stringFunc/stringFunc.h"
#include "Bin/menu/menu.h"
#include "Bin/simulatorCat/simulatorCat.h"
#include "Bin/doomLite/doomLite.h"
#include "Bin/spaceDown/spaceDown.h"
#include "Bin/myFlappyBird/myFlappyBird.h"

#define MAX_PATH_LEN 256
#define MAX_COMMAND_LEN 1024 // Define MAX_COMMAND_LEN here

// Функция для обработки команд
void RunCommand(char *command, char *currentDir) {
    char cmd[MAX_COMMAND_LEN];
    char param1[MAX_PATH_LEN];
    char param2[MAX_PATH_LEN];
    int ret;

    // Объявление переменных для портов
    int start_port, end_port, port;

    // Обработка команды cd
    if (sscanf(command, "cd %[^\n]", param1) == 1) {
        if (strcmp(param1, "..") == 0) {
            // Находим последнее вхождение символа '\\' в currentDir
            char *last_occurrence = strrchr(currentDir, '\\');
            if (last_occurrence != NULL) {
                // Обрезаем строку после последнего вхождения символа
                *last_occurrence = '\0';
                SetCurrentDirectory(currentDir); // Устанавливаем новый текущий каталог
            } else {
                // Если символа '\\' нет, значит, мы уже находимся в корневом каталоге
                strcpy(currentDir, "C:\\");
                SetCurrentDirectory(currentDir); // Устанавливаем корневой каталог
            }
        } else {
            char newDir[MAX_PATH_LEN];
            // Составляем полный путь к новому каталогу
            PathCombine(newDir, currentDir, param1);
            
            // Пытаемся установить новый текущий каталог
            if (SetCurrentDirectory(newDir) == 0) {
                printError1("Failed to change directory.");
                return;
            }
            
            // Получаем текущий каталог после изменения
            GetCurrentDirectory(MAX_PATH_LEN, currentDir);
        }
    } else if (strcmp(command, "ls") == 0) {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind;
        char searchPath[MAX_PATH_LEN];
        PathCombine(searchPath, currentDir, "*");
        hFind = FindFirstFile(searchPath, &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            printError1("Error listing directory contents.");
        } else {
            do {
                // Пропускаем "." и ".." каталоги
                if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
                    continue;
                }

                // Проверяем, является ли это каталогом
                BOOL isDirectory = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

                // Проверяем, является ли это исполняемым файлом (предполагаем расширение .exe для простоты)
                BOOL isExecutable = (strstr(findFileData.cFileName, ".exe") != NULL);

                // Печатаем имя файла с соответствующим цветом
                if (isDirectory) {
                    printf("\x1b[36m%s\x1b[0m ", findFileData.cFileName); // Cyan color for directories
                } else if (isExecutable) {
                    printf("\x1b[33m%s\x1b[0m ", findFileData.cFileName); // Orange color for executables
                } else {
                    printf("%s ", findFileData.cFileName);
                }
            } while (FindNextFile(hFind, &findFileData) != 0);
            printf("\n");
            FindClose(hFind);
        }
    } else if (sscanf(command, "mkdir %s", param1) == 1) {
        char newDir[MAX_PATH_LEN];
        PathCombine(newDir, currentDir, param1);
        ret = CreateDirectory(newDir, NULL);
        if (ret == 0) {
            printError1("Failed to create directory.");
        }
    } else if (sscanf(command, "rmdir %s", param1) == 1) {
        char targetDir[MAX_PATH_LEN];
        PathCombine(targetDir, currentDir, param1);
        ret = RemoveDirectory(targetDir);
        if (ret == 0) {
            printError1("Failed to remove directory.");
        }
    } else if (sscanf(command, "rm %s", param1) == 1) {
        char targetFile[MAX_PATH_LEN];
        PathCombine(targetFile, currentDir, param1);
        ret = DeleteFile(targetFile);
        if (ret == 0) {
            printError1("Failed to remove file.");
        }
    } else if (sscanf(command, "touch %s", param1) == 1) {
        char newFile[MAX_PATH_LEN];
        PathCombine(newFile, currentDir, param1);
        HANDLE hFile = CreateFile(newFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            printError1("Failed to create file.");
        } else {
            CloseHandle(hFile);
        }
    } else if (sscanf(command, "mv %s %s", param1, param2) == 2) {
        char source[MAX_PATH_LEN];
        char dest[MAX_PATH_LEN];
        PathCombine(source, currentDir, param1);
        PathCombine(dest, currentDir, param2);
        ret = MoveFile(source, dest);
        if (ret == 0) {
            printError1("Failed to move file or directory.");
        }
    } else if (sscanf(command, "cat %s", param1) == 1) {
        wchar_t targetFile[MAX_PATH_LEN]; // Используем широкие символы для работы с путями
        MultiByteToWideChar(CP_UTF8, 0, param1, -1, targetFile, MAX_PATH_LEN); // Преобразуем UTF-8 в широкие символы
        HANDLE hFile = CreateFileW(targetFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            printError1("Failed to open file.");
        } else {
            DWORD fileSize = GetFileSize(hFile, NULL);
            char *buffer = (char *)malloc(fileSize + 1);
            if (buffer == NULL) {
                printError1("Memory allocation error.");
            } else {
                DWORD bytesRead;
                if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
                    buffer[bytesRead] = '\0';  // Добавляем завершающий нуль для корректного вывода как строки
                    printf("Contents of file %s:\n%s\n", param1, buffer);
                } else {
                    printError1("Failed to read file.");
                }
                free(buffer);
            }
            CloseHandle(hFile);
        }
    } else if (sscanf(command, "print %[^\n]", param1) == 1) {
        setColor("bright_green");
        printf("%s\n", param1);
        resetColor();
    } else if (strcmp(command, "clear") == 0) {
        system("cls");
    } else if (strcmp(command, "help") == 0) {
        // Печать справочной информации
        setColor("bright_green");
        printf("+-------");
        setColor("bright_magenta");
        printf("<COMMANDS>");
        setColor("bright_green");
        printf("--------------------------------------------------------------------------------------------+\n");
        printf("|    help                                                  Print all commands                                 |\n");
        printf("|    cd <directory>                                        Change current directory within System directory   |\n");
        printf("|    ls                                                    List contents of current directory                 |\n");
        printf("|    mkdir <directory>                                     Create a new directory                             |\n");
        printf("|    rmdir <directory>                                     Remove a directory                                 |\n");
        printf("|    rm <file>                                             Remove a file                                      |\n");
        printf("|    touch <file>                                          Create a new empty file                            |\n");
        printf("|    mv <source> <dest>                                    Move a file or directory                           |\n");
        printf("|    cat <file>                                            Display contents of a file                         |\n");
        printf("|    print <message>                                       Print a message to the console                     |\n");
        printf("|    clear                                                 Clear the console screen                           |\n");
        printf("|    datetime                                              Print date and time                                |\n");
        printf("|    whoami                                                Print current user                                 |\n");
        printf("|    pwd                                                   Print current directory                            |\n");
        printf("|    systeminfo                                            Print information system                           |\n");
        printf("|    shutdown                                              Exit the program                                   |\n");
        printf("+-------");
        setColor("bright_magenta");
        printf("<UTILS>");
        setColor("bright_green");
        printf("-----------------------------------------------------------------------------------------------+\n");
        printf("|    nano <file>                                           Start text editor                                  |\n");
        printf("|    server <port>                                         Start a server on specified port                   |\n");
        printf("|        |-> Example: server 5555                                                                             |\n");
        printf("|    client <address> <port>                               Start a client to connect to a server              |\n");
        printf("|        |-> Example: client 192.168.0.103 5555                                                               |\n");
        printf("|    portscanner <port>                                    Start a port scanner                               |\n");
        printf("|        |-> Example: portscanner 192.168.0.103 1 200                                                         |\n");
        printf("+-------");
        setColor("bright_magenta");
        printf("<GAMES>");
        setColor("bright_green");
        printf("-----------------------------------------------------------------------------------------------+\n");
        printf("|    games                                                 Launches the game selection menu                   |\n");
        printf("|        |-> simulatorCat                                                                                     |\n");
        printf("|        |-> doomLite                                                                                         |\n");
        printf("|        |-> spaceDown                                                                                        |\n");
        printf("|        |-> myFlappyBird                                                                                     |\n");
        printf("+-------------------------------------------------------------------------------------------------------------+\n");
        resetColor();
    } else if (strcmp(command, "datetime") == 0) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        printf("%04d-%02d-%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    } else if (strncmp(command, "nano ", 5) == 0) {
        char filename[MAX_PATH_LEN];
        sscanf(command + 5, "%s", filename);
        RunNano(filename, currentDir);
    } else if (strcmp(command, "systeminfo") == 0) {
        setColor("white");
        printf("A system emulator called Amina (in honor of a person close to me) created by a programmer with the nickname Tailogs\n");
        printf("System updates and source code are located at this address: https://github.com/tailogs/Amina\n");
        resetColor();
    } else if (sscanf(command, "server %d", &port) == 1) {
        start_server(port);
    } else if (sscanf(command, "client %s %d", param1, &port) == 2) {
        start_client(param1, port);
    } else if (sscanf(command, "portscanner %s %d %d", param1, &start_port, &end_port) == 3) {
        scan_ports(param1, start_port, end_port);
    } else if (strcmp(command, "whoami") == 0) {
        char username[MAX_PATH_LEN];
        DWORD username_len = MAX_PATH_LEN;
        GetUserName(username, &username_len);
        printf("Current user: %s\n", username);
    } else if (strcmp(command, "pwd") == 0) {
        printf("Current directory: %s\n", currentDir);
    } else if (strcmp(command, "games") == 0) {
        printf("|>_Menu_games_<|\n");
        
        char *choices[] = {
            "simulatorCat",
            "doomLite",
            "spaceDown",
            "myFlappyBird"
        };
        int n_choices = sizeof(choices) / sizeof(char *);

        init_menu();
        char *selected_choice = show_menu(choices, n_choices);
        printf("You chose: %s\n", selected_choice);
        if (strcmp(selected_choice, "simulatorCat") == 0) {
            system("cls");
            Cat cat = {100, 100, 100, 100, 100, 100, 0, ""};
            birth(&cat);
            live(&cat);
        } else if (strcmp(selected_choice, "doomLite") == 0) {
            setupDoom();

            while (1) {
                if (updateDoom() == 27) break;
                renderDoom();
            }
        } else if (strcmp(selected_choice, "spaceDown") == 0) {
            system("cls");
            runSpaceDown();
        } else if (strcmp(selected_choice, "myFlappyBird") == 0) {
            system("cls");
            runMyFlappyBird();
        }
        free(selected_choice); // Освобождаем память, выделенную strdup
    } else {
        printError2("Unknown command: %s", command);
    }
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8"); // Устанавливаем локаль для работы с UTF-8

    srand(time(NULL));

    enableAnsiSupport();

    char command[MAX_COMMAND_LEN];
    char currentDir[MAX_PATH_LEN];

    // Получаем начальный текущий каталог
    GetCurrentDirectory(MAX_PATH_LEN, currentDir);
    PathCombine(currentDir, currentDir, "Home"); // Инициализируем начальный каталог

    // Основной цикл программы
    while (1) {
        // Обновляем текущий каталог
        GetCurrentDirectory(MAX_PATH_LEN, currentDir); 

        // Получаем путь относительно места запуска программы
        char exePath[MAX_PATH_LEN];
        GetModuleFileName(NULL, exePath, MAX_PATH_LEN);
        PathRemoveFileSpec(exePath);  // Удаляем имя файла программы из пути
        char relativePath[MAX_PATH_LEN];
        PathRelativePathTo(relativePath, exePath, FILE_ATTRIBUTE_DIRECTORY, currentDir, FILE_ATTRIBUTE_DIRECTORY);

        const char *username = getenv("USERNAME");  // Получаем имя пользователя

        if (username != NULL) {
            setBackground("purple");
            setColor("black");
            printf(" (%s) ", username);
            setColor("white");
            printf("%s", relativePath);
            setColor("black");
            printf(" > ", relativePath);
            resetColor();
            printf(" ");
        }

        // Считываем команду пользователя
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';  // Убираем символ новой строки

        // Выход из программы при вводе "exit"
        if (strcmp(command, "shutdown") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // Выполнение команды
        RunCommand(command, currentDir);
    }

    return 0;
}
