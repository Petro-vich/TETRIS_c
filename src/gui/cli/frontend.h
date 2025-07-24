#ifndef CLI_H
#define CLI_H

#define ENTER 10
#define LEFT 97
#define RIGHT 100
#define DOWN 115

#define GAME_HEIGHT 20
#define GAME_WIDTH 10
#define INFO_WIDTH 15
#define BORDER_WIDTH 2
#define BORDER_HEIGHT 2

#include "../../brick_game/tetris/includes/backend.h"
#include "../../brick_game/tetris/includes/common.h"
typedef struct {
  WINDOW *main;
  WINDOW *game;
  WINDOW *info;
  WINDOW *newtFigure;
  WINDOW *waitEnter;
} GameWindows_t;

#define WIN_INIT(time)                      \
  {                                         \
    initscr();                              \
    noecho();                               \
    curs_set(0);                            \
    keypad(stdscr, TRUE);                   \
    timeout(time);                          \
    start_color();                          \
    init_pair(1, COLOR_BLUE, COLOR_BLACK);  \
    init_pair(2, COLOR_GREEN, COLOR_BLACK); \
    init_pair(3, COLOR_CYAN, COLOR_BLACK);  \
    init_pair(10, COLOR_BLACK, COLOR_BLUE); \
  }

void gameLoop();
void getUserInput(GameState_t *gs);
void draw(GameInfo_t *gm, GameWindows_t *window);
void clearWinGame(GameWindows_t *window);
void renderWinGame(GameInfo_t const *gi, GameWindows_t *window);
void renderInfoWin(GameInfo_t *gi, GameState_t const *gs,
                   GameWindows_t *window);
// void renderHelpInfo(GameState_t *gs);
void initWindows(GameWindows_t *window);

#endif