#include "backend.h"
#include "../../gui/cli/frontend.h"



GameState_t* getGs() {
  static GameState_t gs;
  return &gs;
}

int** createMatrix(int rows, int cols) {
  int **temp = (int**)malloc(sizeof(int*) * rows);
  for (int i = 0; i < rows; i++) {
    temp[i] = (int*)malloc(sizeof(int) * cols);
    for (int j = 0; j < cols; j++) {
      temp[i][j] = 0;
    }
  }
  return temp; 
  }

GameInfo_t 
  updateCurrentState(GameState_t *gs) {
  GameInfo_t gi;
  gi.field = createMatrix(FIELD_ROWS, FIELD_COLS);
  gi.next = gs->next;
  gi.score = gs->score;
  gi.high_score = gs->high_score;
  gi.level = gs->level;
  gi.speed = gs->speed;
  gi.pause = gs->pause;

  switch (gs->status) {
    case Initial:
      initGameState();
      gs->status = Spawn;

      break;
    case Spawn:
      gs->figure = gs->next;
      gs->next = generateFigure();
      gs->x = SPAWN_X;
      gs->y = SPAWN_Y;
      gs->status = Moving;
      break;
    case Left:
    case Right:
    case Down:
      moveFigure(gs, &gi);
      break;
    default:
      break;
  }
  
  return gi;
}

void moveFigure(GameState_t *gs, GameInfo_t *gi) {
  switch (gs->status) {
    case Left:  
      if (canMove(gs)) {
        gs->x--;
        updateField(gi, gs);
      }
      break;
    case Right:
      if (canMove(gs)) {
        gs->x++;
        updateField(gi, gs);
      }
      break;
    case Down:
      if (canMove(gs)) {
        gs->y++;
        updateField(gi, gs);
      }
      break;
  }
}

void updateField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int y = gs->y + i;
      int x = gs->x + j;
      if (gs->figure[i][j] == 1) {
        gi->field[y][x] = gs->figure[i][j];
      }
    }
  }
}

int canMove(GameState_t *gs) {
  int direction = gs->status;
  int check = 1;

  if (direction == Left) {
    if (gs->x - 1 < 0 || gs->x - 1 > FIELD_COLS - 4) {
      check = 0;
    }
  }
  
  if (direction == Right) {
    if (gs->x + 1 < 0 || gs->x > FIELD_COLS - 4) {
      check = 0;
    }
  }

  if (direction == Down) {
    
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        int x = gs->x + j;
        int y = gs->y + i;
        

        if (gs->figure[i][j] == 1 && y + 1 == FIELD_ROWS) {
          printf("ATTACHING_FIELD");
          check = 0;
          gs->status = Spawn;
          break;

        }
        
        if (check == 1) {
          if (gs->figure[i][j] == 1 && gs->field[y+1][x] == 1) {
          printf("ATTACHING_FIGURE");
          check = 0;
          }
        }
      }
    }
  }
  return check;
}

void printField(int **field) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      if (field[i][j] == 1) {
        printf("*");
      } else {
        printf("0");
      }
    }
    printf("\n");
  }
}

void freeGameInfo(GameInfo_t *gi) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    free(gi->field[i]);
  }
  free(gi->field);
}

void copyField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      gi->field[i][j] = gs->field[i][j];
    }
  }
}

int** generateFigure() {
  int random_number = rand() % 7;
  int **figure = createMatrix(4, 4);

  switch (random_number) {
    case 0:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[0][2] = 1;
      figure[0][3] = 1;
      break;
    case 1:
      figure[0][0] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 2:
      figure[0][2] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 3:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      break;
    case 4:
      figure[0][1] = 1;
      figure[0][2] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      break;
    case 5:
      figure[0][1] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 6:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    default:
      break;
  }
  return figure;
}

void initGameState() {
  GameState_t *gs = getGs();
  gs->field = createMatrix(FIELD_ROWS, FIELD_COLS);
  gs->figure = generateFigure();
  gs->next = generateFigure();
  // gs->x = SPAWN_X;
  // gs->y = SPAWN_Y;
  gs->status = 0;
  gs->score = 0;
  gs->high_score = 10000;
  gs->level = 1;
  gs->speed = 500000;
  gs->pause = 0;
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