#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE 20
#define TOWER_WIDTH 60
#define TOWER_HEIGHT 10

typedef struct {
    int x, y;
    int active;
} Block;

int towerX = SCREEN_WIDTH / 2 - TOWER_WIDTH / 2;
int towerY = SCREEN_HEIGHT - TOWER_HEIGHT - 10;
int towerHeight = TOWER_HEIGHT;

Block block;
int gameOver = 0;

void interrupt far(*oldKeyHandler)(...);
void interrupt far(*oldTimerHandler)(...);

void interrupt far newKeyHandler(...) {
    unsigned char key;
    key = inportb(0x60);
    if (key == 0x4B) {
        block.x -= 10;
        if (block.x < 0) block.x = 0;
    }
    else if (key == 0x4D) {
        block.x += 10;
        if (block.x > SCREEN_WIDTH - BLOCK_SIZE) block.x = SCREEN_WIDTH - BLOCK_SIZE;
    }
    outportb(0x20, 0x20);
}

void interrupt far newTimerHandler(...) {
    static int timerCount = 0;
    timerCount++;
    if (timerCount >= 2) {
        timerCount = 0;

        if (block.active) {
            block.y += 5;
        }

        if (block.y + BLOCK_SIZE >= towerY &&
            block.x >= towerX &&
            block.x + BLOCK_SIZE <= towerX + TOWER_WIDTH) {
            sound(500);
            delay(100);
            nosound();
            towerHeight += BLOCK_SIZE;
            towerY -= BLOCK_SIZE;
            block.active = 0;
        }

        if (block.y > SCREEN_HEIGHT) {
            gameOver = 1;
        }

        setfillstyle(SOLID_FILL, BLUE);
        bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(towerX, towerY, towerX + TOWER_WIDTH, towerY + towerHeight);

        if (block.active) {
            setfillstyle(SOLID_FILL, RED);
            bar(block.x, block.y, block.x + BLOCK_SIZE, block.y + BLOCK_SIZE);
        }

        if (!block.active) {
            block.x = rand() % (SCREEN_WIDTH - BLOCK_SIZE);
            block.y = 0;
            block.active = 1;
        }
    }
    outportb(0x20, 0x20);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    if (graphresult() != grOk) {
        printf("Error initializing graphics!\n");
        printf("Error code: %d\n", graphresult());
        getch();
        exit(1);
    }
    srand(time(NULL));

    oldKeyHandler = getvect(0x09);
    setvect(0x09, newKeyHandler);
    oldTimerHandler = getvect(0x1C);
    setvect(0x1C, newTimerHandler);

    block.x = rand() % (SCREEN_WIDTH - BLOCK_SIZE);
    block.y = 0;
    block.active = 1;

    while (!gameOver && !kbhit()) {}

    setvect(0x09, oldKeyHandler);
    setvect(0x1C, oldTimerHandler);

    closegraph();
    printf("Game Over!\n");
    getch();
    return 0;
}

