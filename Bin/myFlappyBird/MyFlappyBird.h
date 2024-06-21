#ifndef FLAPPY_BERTS_H
#define FLAPPY_BERTS_H

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define WIDTH 240
#define HEIGHT 60
#define PIPE_WIDTH 10
#define BIRD_SIZE 1
#define GRAVITY 0.2
#define JUMP_FORCE 0.75
#define PIPE_GAP_SIZE 20

extern HANDLE console_handle;
extern CHAR_INFO my_console_buffer[WIDTH * HEIGHT];

extern int bird_x, bird_y;
extern double bird_velocity;
extern int pipe_x, pipe_gap_y;
extern int next_pipe_x, next_pipe_gap_y;
extern int score;
extern int game_over;

void setupMFB();
void drawMFB();
int updateMFB();

#endif // FLAPPY_BERTS_H
