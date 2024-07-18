#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 24

struct Paddle {
    int height;
    char symbol;
    int x, y;
};

struct Ball {
    char symbol;
    int x, y, dx, dy;
};

struct Paddle left_paddle = {5, '#', 4, WINDOW_HEIGHT / 2};
struct Paddle right_paddle = {5, '#', WINDOW_WIDTH - 5, WINDOW_HEIGHT / 2};
struct Ball ball = {'*', WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, -1};


bool paddle_ball_collision(struct Paddle paddle, int fx, int fy);
void update_ball();
void update_right_paddle();
void keyboard(WINDOW *window, bool *running);
void render(WINDOW *window);

int main() {
    initscr();
    curs_set(0);
    cbreak();
    noecho();

    WINDOW *window = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    if (window != NULL) {
        wtimeout(window, 100);

        bool running = true;
        while (running) {
            keyboard(window, &running);
            update_ball();
            update_right_paddle();
            render(window);
        }

        delwin(window);

}
    endwin();
    return 0;
}

bool paddle_ball_collision(struct Paddle paddle, int fx, int fy) {
    return (fx == paddle.x) && (fy >= paddle.y) && (fy < (paddle.y + paddle.height));
}
void update_ball() {
    int fx = ball.x + ball.dx;
    int fy = ball.y + ball.dy;

    if (paddle_ball_collision(left_paddle, fx, fy)) {
        ball.dx = -ball.dx;
    } else if (paddle_ball_collision(right_paddle, fx, fy)) {
        ball.dx = -ball.dx;
    } else if (fx == WINDOW_WIDTH - 1) {
        ball.dx = -ball.dx;
    }

    if (fy == 0 || fy == WINDOW_HEIGHT - 1) {
        ball.dy = -ball.dy;
    }

    ball.x += ball.dx;
    ball.y += ball.dy;
}

void update_right_paddle() {
    if (ball.x > right_paddle.x) {
        return;
    }

    int fby = ball.y + (right_paddle.x - ball.x) * ball.dy;

    if (fby < 0 || fby >= WINDOW_HEIGHT) {
        return;
    }

    int half_paddle_height = right_paddle.height / 2;
    int fpy = right_paddle.y + ((ball.y > (right_paddle.y + half_paddle_height)) - 
                                (ball.y < (right_paddle.y + half_paddle_height)));

    if (fpy && (fpy + right_paddle.height != WINDOW_HEIGHT)) {
        right_paddle.y = fpy;
    }
}

void keyboard(WINDOW *window, bool *running) {
    int code = wgetch(window);

    switch (code) {
        case 'w':
            left_paddle.y = (left_paddle.y > 0) ? left_paddle.y - 1 : left_paddle.y;
            break;
        case 's':
            left_paddle.y = (left_paddle.y + left_paddle.height < WINDOW_HEIGHT) ? left_paddle.y + 1 : left_paddle.y;
            break;
        case 'q':
            *running = false;
            break;
    }
}

void render(WINDOW *window) {
    werase(window);
    box(window, 0, 0);

    mvwaddch(window, ball.y, ball.x, ball.symbol);

    for (int i = 0; i < left_paddle.height; ++i) {
        mvwaddch(window, left_paddle.y + i, left_paddle.x, left_paddle.symbol);
        mvwaddch(window, right_paddle.y + i, right_paddle.x, right_paddle.symbol);
    }

    wrefresh(window);
}

