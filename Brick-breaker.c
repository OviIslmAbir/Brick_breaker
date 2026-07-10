
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ================= CONFIG ================= */
#define PLAY_LEFT      5
#define PLAY_TOP       3
#define PLAY_WIDTH     40
#define PLAY_HEIGHT    20
#define PLAY_RIGHT     (PLAY_LEFT + PLAY_WIDTH + 1)
#define PLAY_BOTTOM    (PLAY_TOP + PLAY_HEIGHT + 1)

#define BRICK_WIDTH    4
#define BRICK_COLS     (PLAY_WIDTH / BRICK_WIDTH)
#define MAX_BRICK_ROWS 6

#define PADDLE_WIDTH   8
#define PADDLE_ROW     (PLAY_BOTTOM - 2)

#define MAX_LIVES      3

/* colors (Windows console attributes) */
#define C_CYAN    11
#define C_MAGENTA 13
#define C_YELLOW  14
#define C_RED     12
#define C_GREEN   10
#define C_GREY     8
#define C_WHITE   15

int brickRowColor[MAX_BRICK_ROWS] = { C_RED, C_MAGENTA, C_YELLOW, C_GREEN, C_CYAN, C_WHITE };

int bricks[MAX_BRICK_ROWS][BRICK_COLS]; /* 1 = alive, 0 = destroyed */
int brickRows;                          /* active rows this level  */

int paddleX;
int ballX, ballY;
int dx, dy;
int ballLaunched;
int score;
int lives;
int level;
int delay;

/* ---------------- console helpers ---------------- */
void gotoxy(int x, int y) {
    COORD c;
    c.X = (SHORT)x;
    c.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor(int hide) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

/* ---------------- drawing ---------------- */
int brickTopRowY(void) {
    return PLAY_TOP + 2;
}

void drawBorders(void) {
    int x, y;
    setColor(C_CYAN);

    gotoxy(PLAY_LEFT, PLAY_TOP);
    printf("+");
    for (x = 0; x < PLAY_WIDTH; x++) printf("=");
    printf("+");

    gotoxy(PLAY_LEFT, PLAY_BOTTOM);
    printf("+");
    for (x = 0; x < PLAY_WIDTH; x++) printf("=");
    printf("+");

    for (y = PLAY_TOP + 1; y <= PLAY_BOTTOM - 1; y++) {
        gotoxy(PLAY_LEFT, y);  printf("|");
        gotoxy(PLAY_RIGHT, y); printf("|");
    }
}

void drawHUD(void) {
    setColor(C_MAGENTA);
    gotoxy(2, 0);
    printf("========= NEON BRICK BREAKER =========");
    setColor(C_YELLOW);
    gotoxy(2, 1);
    printf("SCORE: %d    ", score);
    gotoxy(20, 1);
    printf("LIVES: %d    ", lives);
    gotoxy(34, 1);
    printf("LEVEL: %d    ", level);
}

void setupLevel(void) {
    int r, c;
    brickRows = 3 + (level - 1);
    if (brickRows > MAX_BRICK_ROWS) brickRows = MAX_BRICK_ROWS;

    for (r = 0; r < MAX_BRICK_ROWS; r++)
        for (c = 0; c < BRICK_COLS; c++)
            bricks[r][c] = (r < brickRows) ? 1 : 0;

    delay = 70 - (level - 1) * 5;
    if (delay < 30) delay = 30;
}

void drawBricks(void) {
    int r, c;
    for (r = 0; r < brickRows; r++) {
        setColor(brickRowColor[r % MAX_BRICK_ROWS]);
        for (c = 0; c < BRICK_COLS; c++) {
            gotoxy(PLAY_LEFT + 1 + c * BRICK_WIDTH, brickTopRowY() + r);
            printf(bricks[r][c] ? "[==]" : "    ");
        }
    }
}

void eraseBrick(int r, int c) {
    gotoxy(PLAY_LEFT + 1 + c * BRICK_WIDTH, brickTopRowY() + r);
    printf("    ");
}

void drawPaddle(int oldX) {
    setColor(C_GREY);
    gotoxy(PLAY_LEFT + 1 + oldX, PADDLE_ROW);
    printf("        "); /* erase old (8 spaces) */
    setColor(C_GREEN);
    gotoxy(PLAY_LEFT + 1 + paddleX, PADDLE_ROW);
    printf("========");
}

void eraseBall(void) {
    gotoxy(PLAY_LEFT + 1 + ballX, ballY);
    printf(" ");
}

void drawBall(void) {
    setColor(C_WHITE);
    gotoxy(PLAY_LEFT + 1 + ballX, ballY);
    printf("O");
}

void resetBall(void) {
    ballLaunched = 0;
    ballX = paddleX + PADDLE_WIDTH / 2;
    ballY = PADDLE_ROW - 1;
    dx = (rand() % 2 == 0) ? 1 : -1;
    dy = -1;
}

int bricksRemaining(void) {
    int r, c, count = 0;
    for (r = 0; r < brickRows; r++)
        for (c = 0; c < BRICK_COLS; c++)
            if (bricks[r][c]) count++;
    return count;
}

void showMessage(const char *msg) {
    setColor(C_YELLOW);
    gotoxy(PLAY_LEFT + 6, PLAY_TOP + PLAY_HEIGHT / 2);
    printf("%s", msg);
    Sleep(1300);
    gotoxy(PLAY_LEFT + 6, PLAY_TOP + PLAY_HEIGHT / 2);
    printf("                                ");
}

/* ---------------- main game ---------------- */
int main(void) {
    int running = 1;

    srand((unsigned)time(NULL));
    hideCursor(1);
    system("cls");

    score = 0;
    lives = MAX_LIVES;
    level = 1;
    paddleX = (PLAY_WIDTH - PADDLE_WIDTH) / 2;

    setupLevel();
    drawBorders();
    drawHUD();
    drawBricks();

    resetBall();
    drawPaddle(paddleX);
    drawBall();

    while (running) {
        int oldPaddleX = paddleX;
        int moved = 0;

        /* ---- input ---- */
        if (_kbhit()) {
            int key = _getch();
            if (key == 224 || key == 0) {
                int arrow = _getch();
                if (arrow == 75 && paddleX > 0) { paddleX--; moved = 1; }               /* left  */
                if (arrow == 77 && paddleX < PLAY_WIDTH - PADDLE_WIDTH) { paddleX++; moved = 1; } /* right */
            } else if (key == ' ') {
                if (!ballLaunched) ballLaunched = 1;
            } else if (key == 27) { /* ESC */
                running = 0;
                break;
            }
        }

        if (moved) {
            drawPaddle(oldPaddleX);
            if (!ballLaunched) {
                eraseBall();
                ballX = paddleX + PADDLE_WIDTH / 2;
                drawBall();
            }
        }

        if (!ballLaunched) {
            Sleep(delay);
            continue;
        }

        /* ---- move ball ---- */
        eraseBall();
        ballX += dx;
        ballY += dy;

        /* wall bounces */
        if (ballX <= 0) { ballX = 0; dx = -dx; }
        if (ballX >= PLAY_WIDTH - 1) { ballX = PLAY_WIDTH - 1; dx = -dx; }
        if (ballY <= PLAY_TOP + 1) { ballY = PLAY_TOP + 1; dy = -dy; }

        /* brick collision */
        {
            int relRow = ballY - brickTopRowY();
            if (relRow >= 0 && relRow < brickRows) {
                int col = ballX / BRICK_WIDTH;
                if (col >= 0 && col < BRICK_COLS && bricks[relRow][col]) {
                    bricks[relRow][col] = 0;
                    eraseBrick(relRow, col);
                    dy = -dy;
                    score += 10;
                    drawHUD();
                }
            }
        }

        /* paddle bounce */
        if (ballY == PADDLE_ROW - 1 && dy > 0) {
            if (ballX >= paddleX && ballX < paddleX + PADDLE_WIDTH) {
                int offset = ballX - (paddleX + PADDLE_WIDTH / 2);
                dy = -dy;
                if (offset < -1) dx = -1;
                else if (offset > 1) dx = 1;
            }
        }

        /* ball fell below paddle -> lose life */
        if (ballY >= PLAY_BOTTOM - 1) {
            lives--;
            drawHUD();
            if (lives <= 0) {
                running = 0;
                break;
            }
            showMessage("Ball Lost! Press SPACE...");
            resetBall();
            drawPaddle(paddleX);
            drawBall();
            Sleep(delay);
            continue;
        }

        drawBall();

        /* level clear check */
        if (bricksRemaining() == 0) {
            showMessage("LEVEL CLEAR!");
            level++;
            setupLevel();
            system("cls");
            drawBorders();
            drawHUD();
            drawBricks();
            resetBall();
            drawPaddle(paddleX);
            drawBall();
        }

        Sleep(delay);
    }

    /* ---- game over screen ---- */
    system("cls");
    setColor(C_RED);
    gotoxy(10, 8);  printf("=====================================");
    gotoxy(10, 10); printf("         G A M E   O V E R");
    gotoxy(10, 12); printf("=====================================");
    setColor(C_YELLOW);
    gotoxy(14, 15); printf("Final Score  : %d", score);
    gotoxy(14, 16); printf("Level Reached: %d", level);
    setColor(C_CYAN);
    gotoxy(10, 19); printf("Press R to Retry or ESC to Quit...");
    setColor(C_WHITE);

    {
        int c;
        while (1) {
            c = _getch();
            if (c == 'r' || c == 'R') {
                hideCursor(1);
                return main(); /* restart */
            }
            if (c == 27) break;
        }
    }

    hideCursor(0);
    return 0;
}
