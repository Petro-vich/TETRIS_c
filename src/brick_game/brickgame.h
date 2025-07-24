#ifndef BRICKGAME_H
#define BRICKGAME_H

#define FIELD_ROWS 20
#define FIELD_COLS 10

#include <stdio.h>
#include <stdlib.h>
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Space
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int highScore;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif