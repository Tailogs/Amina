#include "doomLite.h"

// Global variables
CHAR_INFO *console_buffer;
HANDLE console_handle;
int WIDTH, HEIGHT;
double player_x = 8.0, player_y = 8.0, player_angle = 0.0;
int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void getConsoleSize(int *width, int *height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void setupDoom() {
    getConsoleSize(&WIDTH, &HEIGHT);
    console_buffer = (CHAR_INFO *)malloc(sizeof(CHAR_INFO) * WIDTH * HEIGHT);
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD buffer_size = {WIDTH, HEIGHT};
    SetConsoleScreenBufferSize(console_handle, buffer_size);
    SMALL_RECT window_size = {0, 0, WIDTH - 1, HEIGHT - 1};
    SetConsoleWindowInfo(console_handle, TRUE, &window_size);
}

void draw_wall(int x, int y, int height, char texture) {
    for (int i = 0; i < height; i++) {
        if (y + i >= 0 && y + i < HEIGHT) {
            console_buffer[(y + i) * WIDTH + x].Char.AsciiChar = texture;
            console_buffer[(y + i) * WIDTH + x].Attributes = FOREGROUND_GREEN;
        }
    }
}

void renderDoom() {
    double fov_start = player_angle - FOV_ANGLE / 2;
    double fov_end = player_angle + FOV_ANGLE / 2;

    for (int x = 0; x < WIDTH; x++) {
        double ray_angle = fov_start + ((double)x / WIDTH) * FOV_ANGLE;
        double sin_a = sin(ray_angle);
        double cos_a = cos(ray_angle);

        double distance_to_wall = 0;
        int hit_wall = 0;
        double eye_x = cos_a;
        double eye_y = sin_a;

        while (!hit_wall && distance_to_wall < MAP_WIDTH) {
            distance_to_wall += 0.1;
            int test_x = (int)(player_x + eye_x * distance_to_wall);
            int test_y = (int)(player_y + eye_y * distance_to_wall);

            if (test_x < 0 || test_x >= MAP_WIDTH || test_y < 0 || test_y >= MAP_HEIGHT) {
                hit_wall = 1;
                distance_to_wall = MAP_WIDTH;
            } else if (map[test_y][test_x] == 1) {
                hit_wall = 1;
            }
        }

        int ceiling = (HEIGHT / 2.0) - HEIGHT / ((double)distance_to_wall);
        int floor = HEIGHT - ceiling;

        for (int y = 0; y < HEIGHT; y++) {
            if (y < ceiling) {
                console_buffer[y * WIDTH + x].Char.AsciiChar = ' ';
                console_buffer[y * WIDTH + x].Attributes = 0;
            } else if (y > ceiling && y <= floor) {
                console_buffer[y * WIDTH + x].Char.AsciiChar = '#';
                console_buffer[y * WIDTH + x].Attributes = FOREGROUND_GREEN;
            } else {
                console_buffer[y * WIDTH + x].Char.AsciiChar = '.';
                console_buffer[y * WIDTH + x].Attributes = FOREGROUND_RED;
            }
        }
    }

    SetConsoleCursorPosition(console_handle, (COORD){0, 0});
    WriteConsoleOutputA(console_handle, console_buffer, (COORD){WIDTH, HEIGHT}, (COORD){0, 0}, &(SMALL_RECT){0, 0, WIDTH, HEIGHT});
}

int updateDoom() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': case 'A':
                player_angle -= 0.1;
                break;
            case 'd': case 'D':
                player_angle += 0.1;
                break;
            case 'w': case 'W':
                player_x += cos(player_angle) * 0.1;
                player_y += sin(player_angle) * 0.1;
                if (map[(int)player_y][(int)player_x] == 1) {
                    player_x -= cos(player_angle) * 0.1;
                    player_y -= sin(player_angle) * 0.1;
                }
                break;
            case 's': case 'S':
                player_x -= cos(player_angle) * 0.1;
                player_y -= sin(player_angle) * 0.1;
                if (map[(int)player_y][(int)player_x] == 1) {
                    player_x += cos(player_angle) * 0.1;
                    player_y += sin(player_angle) * 0.1;
                }
                break;
            case 27: // ESC key
                system("cls");
                return 27;
        }
    }
    return 0;
}