#ifndef TETRIS_H
#define TETRIS_H

#include "../brickgame.h"
#include <stdlib.h>

#define SPAWN_X 4
#define SPAWN Y 0 

typedef enum {
  Start,      // f1
  Pause,      // f2
  Terminate,  // f3
  Left,       // 3
  Right,      // 4
  Up,         // none
  Down,       // падение
  Action      // rotate  // 7
} UserAction_t;

typedef struct {
  int **field;
  int **figure;
  int **next;  
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
}GameState_t;


void initGameinfo();
GameState_t* getGs();
GameInfo_t updateCurrentState(GameState_t *gs);

int** createMatrix(int rows, int cols);

#endif