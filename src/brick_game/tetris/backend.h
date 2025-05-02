#ifndef TETRIS_H
#define TETRIS_H

#include "../brickgame.h"
#include <string.h>
#define SPAWN_X 4
#define SPAWN_Y 0 


typedef struct {
  int **field;
  int **figure;
  int **next;  
  int x;
  int y;
  int status;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
}GameState_t;

typedef enum {
  Initial,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  Gameover
}GameStatus_t;


GameState_t* getGs();
GameInfo_t updateCurrentState(GameState_t *gs);

int** createMatrix(int rows, int cols);
void initGameState();
void gameLoop();
void moveFigure(GameState_t *gs, GameInfo_t *gi);
int** generateFigure();
int canMove(GameState_t *gs);
void updateField(GameInfo_t *gi, GameState_t *gs);
void freeGameInfo(GameInfo_t *gi);
void copyField(GameInfo_t *gi, GameState_t *gs);
void Draw(GameInfo_t *gm);

void initWindows();
void cleanupWindows();

#endif