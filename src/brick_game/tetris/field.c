
#include "includes/field.h"

void updateField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int y = gs->y + i;
      int x = gs->x + j;
      if (gs->figure[i][j] == 1) {
        if (x >= 0 && x < FIELD_COLS && y >= 0 && y < FIELD_ROWS) {
          gi->field[y][x] = gs->figure[i][j];
        }
      }
    }
  }
}

void copyField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      gi->field[i][j] = gs->field[i][j];
    }
  }
}

int **createMatrix(int rows, int cols) {
  int **temp = (int **)malloc(sizeof(int *) * rows);
  for (int i = 0; i < rows; i++) {
    temp[i] = (int *)malloc(sizeof(int) * cols);
    for (int j = 0; j < cols; j++) {
      temp[i][j] = 0;
    }
  }
  return temp;
}

int clearFullLines(GameState_t *gs) {
  int lines_cleared = 0;
  for (int row = FIELD_ROWS - 1; row >= 0; row--) {
    int full = 1;
    for (int col = 0; col < FIELD_COLS; col++) {
      if (gs->field[row][col] == 0) {
        full = 0;
        break;
      }
    }
    if (full) {
      lines_cleared++;
      for (int r = row; r > 0; r--) {
        for (int c = 0; c < FIELD_COLS; c++) {
          gs->field[r][c] = gs->field[r - 1][c];
        }
      }
      for (int c = 0; c < FIELD_COLS; c++) {
        gs->field[0][c] = 0;
      }
      row++;
    }
  }
  return lines_cleared;
}

int checkGameOver(GameState_t const *gs) {
  bool isGameOver = false;

  if (gs->field != NULL) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (gs->figure[i][j] == 1) {
          int fy = gs->y + i;
          int fx = gs->x + j;
          if (fy >= 0 && fy < FIELD_ROWS && fx >= 0 && fx < FIELD_COLS) {
            if (gs->field[fy][fx] == 1) {
              isGameOver = true;
            }
          }
        }
      }
    }
  }
  return isGameOver;
}
void handlePause(GameState_t *gs, GameInfo_t *gi) {
  while (getch() != '\n') {
    sleep(1);
  }
  gs->pause = 0;
  gi->pause = 0;
}