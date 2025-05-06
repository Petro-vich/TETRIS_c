#ifndef BRICKGAME_H
#define BRICKGAME_H

#define FIELD_ROWS 20
#define FIELD_COLS 10

#include <stdlib.h>
#include <stdio.h>
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
  int **next;  
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

#endif