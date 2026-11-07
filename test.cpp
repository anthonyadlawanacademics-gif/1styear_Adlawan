#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

using namespace std;

struct Player {
    int x;
    int y;
    int vx;
    int vy;
    bool onGround;
    int dir;
};

struct Platform {
    int x;
    int y;
    int width;
    int height;
};

struct Coin {
    int x;
    int y;
    bool collected;
};

static bool kbhit() {
#ifdef _WIN32
    return _kbhit() != 0;
#else
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
#endif
}

static char readch() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

int main() {
    const int WIDTH = 40;
    const int HEIGHT = 18;
    const int GRAVITY = 1;
    const int MOVE_SPEED = 1;
    const int JUMP_SPEED = 3;

    vector<string> world(HEIGHT, string(WIDTH, ' '));
    vector<Platform> platforms = {
        {0, 16, 40, 1},
        {4, 12, 8, 1},
        {16, 10, 7, 1},
        {28, 13, 6, 1},
        {32, 8, 5, 1}
    };

    vector<Coin> coins = {
        {6, 11, false},
        {19, 9, false},
        {31, 12, false},
        {35, 7, false}
    };

    Player player{1, 14, 0, 0, true, 1};
    int score = 0;

    bool running = true;
    while (running) {
        if (kbhit()) {
            char key = readch();
            if (key == 'a' || key == 'A') {
                player.vx = -MOVE_SPEED;
                player.dir = -1;
            } else if (key == 'd' || key == 'D') {
                player.vx = MOVE_SPEED;
                player.dir = 1;
            } else if (key == 'w' || key == 'W' || key == ' ') {
                if (player.onGround) {
                    player.vy = -JUMP_SPEED;
                    player.onGround = false;
                }
            } else if (key == 'q' || key == 'Q') {
                running = false;
            }
        }

        if (!kbhit()) {
            player.vx = 0;
        }

        player.vy += GRAVITY;
        player.x += player.vx;
        player.y += player.vy;

        if (player.x < 0) player.x = 0;
        if (player.x >= WIDTH - 1) player.x = WIDTH - 1;

        player.onGround = false;
        for (const auto &p : platforms) {
            if (player.y + 1 >= p.y && player.y <= p.y + p.height &&
                player.x >= p.x && player.x <= p.x + p.width) {
                if (player.vy >= 0 && player.y + 1 <= p.y) {
                    player.y = p.y - 1;
                    player.vy = 0;
                    player.onGround = true;
                }
            }
        }

        if (player.y >= HEIGHT - 1) {
            player.y = HEIGHT - 2;
            player.vy = 0;
            player.onGround = true;
        }

        for (auto &coin : coins) {
            if (!coin.collected && coin.x == player.x && coin.y == player.y) {
                coin.collected = true;
                score++;
            }
        }

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                world[y][x] = ' ';
            }
        }

        for (const auto &p : platforms) {
            for (int y = p.y; y < p.y + p.height; ++y) {
                for (int x = p.x; x < p.x + p.width; ++x) {
                    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                        world[y][x] = '#';
                    }
                }
            }
        }

        for (const auto &coin : coins) {
            if (!coin.collected) {
                world[coin.y][coin.x] = 'o';
            }
        }

        world[player.y][player.x] = player.dir > 0 ? 'C' : 'c';

        cout << "Score: " << score << "\n";
        for (int y = 0; y < HEIGHT; ++y) {
            cout << world[y] << "\n";
        }
        cout << "Controls: A/D move, W/Space jump, Q quit\n\n";

        this_thread::sleep_for(chrono::milliseconds(80));

#ifdef _WIN32
        system("cls");
#else
        cout << "\033[H\033[J";
#endif
    }

    cout << "Game Over! Final score: " << score << "\n";
    return 0;
}
