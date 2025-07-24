#ifndef TETRIS_H
#define TETRIS_H

#include "common.h"
#define SPAWN_X 4
#define SPAWN_Y 0

typedef enum {
  Initial,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  Gameover
} GameStatus_t;

typedef struct {
  long long timeStart;
  long long timeEnd;
} Time_t;

typedef struct {
  int **field;
  int **figure;
  int **next;
  int x;
  int y;
  int score;
  int highScore;
  int level;
  int speed;
  int pause;
  int button;
  int is_play;
  GameStatus_t status;
  Time_t time;
} GameState_t;

GameState_t *getGs();
GameInfo_t updateCurrentState();
void initGameState();
void freeGameInfo(GameInfo_t *gi);
void freeGameState(GameState_t *gs);
// void cleanupWindows();

#endif