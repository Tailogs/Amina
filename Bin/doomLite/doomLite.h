#ifndef DOOM_LITE_H
#define DOOM_LITE_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

// Constants
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define FOV_ANGLE (60 * (M_PI / 180))
#define TEXTURE_SIZE 16

// Global variables
extern CHAR_INFO *console_buffer;
extern HANDLE console_handle;
extern int WIDTH, HEIGHT;
extern double player_x, player_y, player_angle;
extern int map[MAP_WIDTH][MAP_HEIGHT];

// Function declarations
void setupDoom();
void draw_wall(int x, int y, int height, char texture);
void renderDoom();
int updateDoom();
void getConsoleSize(int *width, int *height);

#endif // DOOM_LITE_H
