#include "includes/figure.h"

#include "includes/field.h"
#include "includes/lvl.h"

void freeFigure(int **figure) {
  if (figure != NULL) {
    for (int i = 0; i < 4; i++) {
      free(figure[i]);
    }
    free(figure);
  }
}

void rotateFigure(GameState_t *gs) {
  int is_square = 1;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      if (gs->figure[i][j] != 1) is_square = 0;
  for (int i = 0; i < 2; i++)
    for (int j = 2; j < 4; j++)
      if (gs->figure[i][j] != 0) is_square = 0;
  for (int i = 2; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (gs->figure[i][j] != 0) is_square = 0;
  if (is_square) return;

  int temp[4][4] = {0};
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) temp[i][j] = gs->figure[i][j];

  int rotated[4][4] = {0};
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) rotated[j][3 - i] = temp[i][j];

  int can_rotate = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rotated[i][j]) {
        int y = gs->y + i;
        int x = gs->x + j;
        if (x < 0 || x >= FIELD_COLS || y < 0 || y >= FIELD_ROWS)
          can_rotate = 0;
        if (gs->field[y][x]) can_rotate = 0;
      }
    }
  }

  if (can_rotate) {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++) gs->figure[i][j] = rotated[i][j];
  }
}

int canMove(GameState_t *gs) {
  int dx = 0, dy = 0;

  switch (gs->button) {
    case Left:
      dx = -1;
      break;
    case Right:
      dx = 1;
      break;
    case Down:
    case ERR:
      dy = 1;
      break;
    default:
      return 1;
  }

  bool isMove = true;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gs->figure[i][j] == 1) {
        int fy = gs->y + i + dy;
        int fx = gs->x + j + dx;

        if (fx < 0 || fx >= FIELD_COLS || fy < 0 || fy >= FIELD_ROWS) {
          if (dy == 1) gs->status = Attaching;
          isMove = false;
          break;
        }

        if (gs->field[fy][fx]) {
          if (dy == 1) gs->status = Attaching;
          isMove = false;
          break;
        }
      }
    }
  }

  return isMove;
}

long long get_time_ms() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

int isShifting(GameState_t *gs) {
  bool isSh = false;
  gs->time.timeEnd = get_time_ms();
  long result = gs->time.timeEnd - gs->time.timeStart;
  if (result > gs->speed / 1000) {
    isSh = true;
  }
  return isSh;
}

int get_random_number() { return get_time_ms() % 7; }

void moveFigure(GameState_t *gs) {
  switch (gs->button) {
    case Left:
      if (canMove(gs)) {
        gs->x--;
      }
      break;
    case Right:
      if (canMove(gs)) {
        gs->x++;
      }
      break;
    case Down:
    case ERR:
      if (canMove(gs)) {
        gs->y++;
      }
      break;
    case Up:
      break;
    default:
      break;
  }
}

int **generateFigure() {
  int random_number = get_random_number();
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

void handleAttaching(GameState_t *gs, GameInfo_t *gi) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gs->figure[i][j] == 1) {
        int fy = gs->y + i;
        int fx = gs->x + j;
        gs->field[fy][fx] = 1;
      }
    }
  }
  int lines_cleared = clearFullLines(gs);
  addScore(gs, lines_cleared);
  gs->status = Spawn;
  copyField(gi, gs);
}

void handleSpawn(GameState_t *gs) {
  freeFigure(gs->figure);
  gs->figure = gs->next;
  gs->next = generateFigure();
  gs->x = SPAWN_X;
  gs->y = SPAWN_Y;
  gs->is_play = true;
  gs->status = Moving;
}