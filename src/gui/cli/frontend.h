#ifndef CLI_H
#define CLI_H

#define ENTER 10
#define LEFT 97
#define RIGHT 100
#define DOWN 115

#define WIN_INIT(time)     \
  {                        \
    initscr();             \
    noecho();              \
    curs_set(0);           \
    keypad(stdscr, TRUE);  \
    timeout(time);         \
    nodelay(stdscr, TRUE); \
    start_color();         \
  }

#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include "../../brick_game/brickgame.h"
#include "../../brick_game/tetris/backend.h"

void gameLoop();
void handleUserInput(GameState_t *gs);


#endif