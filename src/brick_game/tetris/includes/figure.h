#ifndef FIGURE_H
#define FIGURE_H

#include "backend.h"
#include "common.h"

void moveFigure(GameState_t *gs);
int **generateFigure();
void freeFigure(int **figure);
void rotateFigure(GameState_t *gs);
int canMove(GameState_t *gs);
long long get_time_ms();
int isShifting(GameState_t *gs);
int getRandomNumber();
void handleAttaching(GameState_t *gs, GameInfo_t *gi);
void handleSpawn(GameState_t *gs);

#endif
