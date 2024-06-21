#include "myFlappyBird.h"

HANDLE __console_handle;
CHAR_INFO my_console_buffer[WIDTH * HEIGHT];

int bird_x, bird_y;
double bird_velocity;
int pipe_x, pipe_gap_y;
int next_pipe_x, next_pipe_gap_y;
int score;
int game_over;

void setupMFB() {
    __console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD buffer_size = {WIDTH, HEIGHT};
    SetConsoleScreenBufferSize(__console_handle, buffer_size);
    SMALL_RECT window_size = {0, 0, WIDTH - 1, HEIGHT - 1};
    SetConsoleWindowInfo(__console_handle, TRUE, &window_size);

    bird_x = 20;
    bird_y = HEIGHT / 2;
    bird_velocity = 0.0;
    pipe_x = WIDTH - PIPE_WIDTH;
    pipe_gap_y = rand() % (HEIGHT - PIPE_GAP_SIZE);
    next_pipe_x = 0;
    next_pipe_gap_y = 0;
    score = 0;
    game_over = 0;
}

void drawMFB() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x >= pipe_x && x < pipe_x + PIPE_WIDTH) {
                if (y >= pipe_gap_y && y < pipe_gap_y + PIPE_GAP_SIZE) {
                    my_console_buffer[y * WIDTH + x].Char.AsciiChar = ' ';
                    my_console_buffer[y * WIDTH + x].Attributes = 0;
                } else {
                    my_console_buffer[y * WIDTH + x].Char.AsciiChar = '#';
                    my_console_buffer[y * WIDTH + x].Attributes = FOREGROUND_GREEN;
                }
            } else if (x >= next_pipe_x && x < next_pipe_x + PIPE_WIDTH) {
                if (y >= next_pipe_gap_y && y < next_pipe_gap_y + PIPE_GAP_SIZE) {
                    my_console_buffer[y * WIDTH + x].Char.AsciiChar = ' ';
                    my_console_buffer[y * WIDTH + x].Attributes = 0;
                } else {
                    my_console_buffer[y * WIDTH + x].Char.AsciiChar = '#';
                    my_console_buffer[y * WIDTH + x].Attributes = FOREGROUND_GREEN;
                }
            } else if (x == bird_x && y == bird_y) {
                my_console_buffer[y * WIDTH + x].Char.AsciiChar = '@';
                my_console_buffer[y * WIDTH + x].Attributes = FOREGROUND_RED;
            } else {
                my_console_buffer[y * WIDTH + x].Char.AsciiChar = ' ';
                my_console_buffer[y * WIDTH + x].Attributes = 0;
            }
        }
    }

    SetConsoleCursorPosition(__console_handle, (COORD){0, 0});
    WriteConsoleOutputA(__console_handle, my_console_buffer, (COORD){WIDTH, HEIGHT}, (COORD){0, 0}, &(SMALL_RECT){0, 0, WIDTH, HEIGHT});
}

int updateMFB() {
    if (_kbhit()) {
        int key = _getch();
        switch (key) {
            case ' ':
                bird_velocity = -JUMP_FORCE;
                break;
            case 27: // ESC key
                system("cls");
                game_over = 1;
                return 27;
                break;
            default:
                break;
        }
    }

    bird_velocity += GRAVITY;
    bird_y += bird_velocity;

    if (bird_y < 0 || bird_y >= HEIGHT) {
        game_over = 1;
    }

    if ((bird_x == pipe_x && (bird_y < pipe_gap_y || bird_y >= pipe_gap_y + PIPE_GAP_SIZE)) ||
        (bird_x == next_pipe_x && (bird_y < next_pipe_gap_y || bird_y >= next_pipe_gap_y + PIPE_GAP_SIZE))) {
        game_over = 1;
    }

    if (!game_over) {
        pipe_x--;
        next_pipe_x--;

        if (pipe_x == 0) {
            pipe_x = WIDTH - PIPE_WIDTH;
            pipe_gap_y = rand() % (HEIGHT - PIPE_GAP_SIZE);
            score++;

            next_pipe_x = pipe_x - rand() % (WIDTH / 2) - 20;
            next_pipe_gap_y = rand() % (HEIGHT - PIPE_GAP_SIZE);
            while (next_pipe_gap_y == pipe_gap_y) {
                next_pipe_gap_y = rand() % (HEIGHT - PIPE_GAP_SIZE);
            }
        }
    }
}

void runMyFlappyBird() {
    srand(time(NULL));
    setupMFB();

    /*
    printf("Welcome to Flappy Berts!\n");
    printf("Press ENTER to start or ESC to exit...\n");
    while (_getch() != 13 && _getch() != 27);

    if (_getch() == 27) {
        return;
    }
    */

    short in = 1;

    while (in) {
        while (!game_over) {
            drawMFB();
            if (updateMFB() == 27) {in = 0; break;}
            Sleep(50);
        }

        displayEndMessage();
    }
}
