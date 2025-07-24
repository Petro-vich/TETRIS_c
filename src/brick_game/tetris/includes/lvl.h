#ifndef LVL_H
#define LVL_H

#include "backend.h"
#include "common.h"

void addScore(GameState_t *gs, int lines_cleared);
void updateLevel(GameState_t *gs);
int loadHighScore();
void saveHighScore(int score);
void updateHighScore(GameState_t *gs);

#endif
