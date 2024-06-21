#include "spaceDown.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

// Global variables
float x, y, velocityX, velocityY, fuel, angle;
float thrust = 0.15;
float gravity = 0.05;
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];
HANDLE wHnd;

void delay(int milliseconds) {
    Sleep(milliseconds);
}

void initConsole() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, WIDTH - 1, HEIGHT - 1};
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    COORD bufferSize = {WIDTH, HEIGHT};
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

void resetGame() {
    x = (rand() % WIDTH); // Random horizontal position
    y = (rand() % (HEIGHT / 2)); // Random position from center to upper edge
    velocityX = 0;
    velocityY = 0;
    fuel = 100;

    // Random rotation angle: 0, -0.1, or 0.1
    int randomAngle = rand() % 3;
    if (randomAngle == 0) {
        angle = 0;
    } else if (randomAngle == 1) {
        angle = -0.1;
    } else {
        angle = 0.1;
    }
}

void draw() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 7;
    }

    for (int j = 0; j < WIDTH; j++) {
        consoleBuffer[(HEIGHT - 2) * WIDTH + j].Char.AsciiChar = '=';
    }

    // Coordinates of the spaceship relative to the base point (nose)
    int shipPoints[8][2] = {
        { 0, -3 },   // Nose of the spaceship
        { -2, 0 },   // Left wing
        { 2, 0 },    // Right wing
        { -1, 1 },   // Left engine
        { 1, 1 },    // Right engine
        { -1, 3 },   // Left lower wing
        { 1, 3 },    // Right lower wing
        { 0, 3 },    // Tail of the spaceship
    };

    int rx = (int)round(x);
    int ry = (int)round(y);
    if (ry >= 0 && ry < HEIGHT) {
        // Displaying the spaceship considering the rotation
        float cosA = cos(angle);
        float sinA = sin(angle);

        // Coordinates of the base point of the spaceship (nose)
        int baseX = rx;
        int baseY = ry;

        // Rotation and display of each point of the spaceship on the screen
        for (int i = 0; i < 8; i++) {
            int rotatedX = (int)(shipPoints[i][0] * cosA - shipPoints[i][1] * sinA + baseX);
            int rotatedY = (int)(shipPoints[i][0] * sinA + shipPoints[i][1] * cosA + baseY);

            if (rotatedX >= 0 && rotatedX < WIDTH && rotatedY >= 0 && rotatedY < HEIGHT) {
                consoleBuffer[rotatedY * WIDTH + rotatedX].Char.AsciiChar = 'A';
            }
        }
    }

    // Saving the state information of the rocket in a string
    char infoBuffer[100];
    sprintf_s(infoBuffer, sizeof(infoBuffer), "Fuel: %.2f\nVertical Velocity: %.2f\nHorizontal Velocity: %.2f\n", fuel, velocityY, velocityX);

    // Displaying the screen and state information of the rocket
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, WIDTH - 1, HEIGHT - 1};
    WriteConsoleOutputA(wHnd, consoleBuffer, (COORD) {WIDTH, HEIGHT}, bufferCoord, &writeRegion);
    SetConsoleCursorPosition(wHnd, bufferCoord);
    printf("%s", infoBuffer);
}

void displayCenteredText(const char *text, int yOffset) {
    int len = strlen(text);
    int startX = (WIDTH - len) / 2;
    int startY = (HEIGHT / 2) + yOffset;

    for (int i = 0; i < len; i++) {
        if (startX + i >= 0 && startX + i < WIDTH) {
            consoleBuffer[startY * WIDTH + startX + i].Char.AsciiChar = text[i];
        }
    }
}

void displayEndMessage(const char *message) {
    // Clear the screen
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 7;
    }

    // Display the win or lose message in the center of the screen in large letters
    displayCenteredText(message, -2);

    // Add restart or exit instructions
    displayCenteredText("Press ENTER to play again or ESC to exit", 2);

    // Display the updated screen
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, WIDTH - 1, HEIGHT - 1};
    WriteConsoleOutputA(wHnd, consoleBuffer, (COORD) {WIDTH, HEIGHT}, bufferCoord, &writeRegion);

    // Wait for key presses
    while (1) {
        int key = _getch();
        if (key == 13) { // ENTER key
            resetGame();
            break;
        } else if (key == 27) { // ESC key
            exit(0);
        }
    }
}

int processInput() {
    if (_kbhit()) {
        int key = _getch();
        switch (key) {
            case 'W': case 'w':
                if (fuel > 0) {
                    // Apply thrust in the direction of the current rotation angle
                    velocityY -= thrust * cos(angle);
                    velocityX += thrust * sin(angle);
                    fuel -= 0.5;
                }
                break;
            case 'A': case 'a':
                // Rotate the spaceship left
                angle -= 0.1;
                break;
            case 'D': case 'd':
                // Rotate the spaceship right
                angle += 0.1;
                break;
            case 27: // ESC key
                system("cls");
                return 27;
            default:
                break;
        }
    }
}

void runSpaceDown() {
    srand(time(NULL)); // Инициализация генератора случайных чисел
    initConsole();

    printf("Welcome to the Lunar Lander game!\n");
    printf("Your mission is to land the spacecraft softly on the designated landing zone.\n");
    printf("Controls:\n");
    printf("  W: Increase thrust (in direction of current angle)\n");
    printf("  A: Rotate left\n");
    printf("  D: Rotate right\n");
    printf("Press any key to start...\n");
    _getch();

    short in = 1;

    while (in) {
        resetGame();

        while (1) {
            if (processInput() == 27) { in = 0; break; }

            velocityY += gravity;
            x += velocityX;
            y += velocityY;

            if (x < 0) x = 0;
            if (x > WIDTH - 1) x = WIDTH - 1;
            if (y < 0) y = 0;
            if (y >= HEIGHT - 5) {
                y = HEIGHT - 5;
                break;
            }

            draw();

            delay(50);
        }

        // Проверка на успешную посадку с учетом угла поворота +-5 градусов
        if (velocityY > 1 || x < WIDTH / 3 || x > 2 * WIDTH / 3 || fabs(angle) > 5 * M_PI / 180) {
            displayEndMessage("CRASH! YOU LOSE.");
        } else {
            displayEndMessage("SUCCESS! YOU WIN.");
        }
    }
}
