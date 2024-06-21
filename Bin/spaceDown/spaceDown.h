#ifndef SPACEDOWN_H
#define SPACEDOWN_H

#include <windows.h>

// Constants
#define WIDTH 240
#define HEIGHT 60

// Global variables
extern float x, y, velocityX, velocityY, fuel, angle;
extern float thrust;
extern float gravity;
extern CHAR_INFO consoleBuffer[WIDTH * HEIGHT];
extern HANDLE wHnd;

// Function declarations
void delay(int milliseconds);
void initConsole();
void resetGame();
void draw();
void displayCenteredText(const char *text, int yOffset);
void displayEndMessage(const char *message);
int processInput();
void runSpaceDown();

#endif // SPACEDOWN_H








