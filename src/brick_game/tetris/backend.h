#ifndef TETRIS_H
#define TETRIS_H

#include <string.h>

#include "../brickgame.h"
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
  int high_score;
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
void moveFigure(GameState_t *gs, GameInfo_t *gi);
int canMove(GameState_t *gs);
void updateField(GameInfo_t *gi, GameState_t *gs);
int **generateFigure();
int **createMatrix(int rows, int cols);
void copyField(GameInfo_t *gi, GameState_t *gs);
void freeGameInfo(GameInfo_t *gi);
long long get_time_ms();
void rotateFigure(GameState_t *gs);

// void cleanupWindows();

#endif