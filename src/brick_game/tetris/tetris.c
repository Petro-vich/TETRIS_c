#include "tetris.h"

GameState_t* getGs() {
  static GameInfo_t gs;
  return &gs;
}

int** createMatrix(int rows, int cols) {
  int **temp = (int**)malloc(sizeof(int*) * rows);
  for (int i = 0; i < rows; i++) {
    temp[i] = (int*)malloc(sizeof(int) * cols);

  return temp; 
  }

}

GameInfo_t updateCurrentState(GameState_t *gs) {

}


int** generateFigure() {
  int temp = createMatrix(4, 4);
  

}

void initGameState() {
  GameState_t *gs = getGs();

  gs->field = createMatrix(FIELD_ROWS, FIELD_COLS);
  int **figure = createMatrix(4, 4);
  int **next = generateFigure();
  int score = 0;
  int high_score = 10000;
  int level = 1;
  int speed = 500000;
  int pause = 0;

}




// typedef struct {
//   int **field;
//   int **next;  
//   int score;
//   int high_score;
//   int level;
//   int speed;
//   int pause;
// } GameInfo_t;

// #endif