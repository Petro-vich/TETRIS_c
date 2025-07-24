#ifndef FIELD_H
#define FIELD_H

#include "backend.h"
#include "common.h"

void updateField(GameInfo_t *gi, GameState_t *gs);
void copyField(GameInfo_t *gi, GameState_t *gs);
int **createMatrix(int rows, int cols);
int clearFullLines(GameState_t *gs);
int checkGameOver(GameState_t const *gs);
void handlePause(GameState_t *gs, GameInfo_t *gi);
#endif
