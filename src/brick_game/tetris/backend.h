#ifndef TETRIS_H
#define TETRIS_H

#include "../brickgame.h"
#include <string.h>
#define SPAWN_X 4
#define SPAWN_Y 0 

typedef enum {
  Initial,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  Gameover
}GameStatus_t;


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
}GameState_t;



GameState_t* getGs();
GameInfo_t updateCurrentState();

int** createMatrix(int rows, int cols);
void initGameState();
void gameLoop();
void moveFigure(GameState_t *gs, GameInfo_t *gi);
int** generateFigure();
int canMove(GameState_t *gs);
void updateField(GameInfo_t *gi, GameState_t *gs);
void freeGameInfo(GameInfo_t *gi);
void copyField(GameInfo_t *gi, GameState_t *gs);

// void cleanupWindows();

#endif