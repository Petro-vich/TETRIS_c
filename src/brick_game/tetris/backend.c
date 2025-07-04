#include "backend.h"

#include "../../gui/cli/frontend.h"

long long get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Монотонное время (не зависит от системных часов)
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}


GameState_t *getGs() {
  static GameState_t gs;
  return &gs;
}

GameInfo_t updateCurrentState() {
  GameState_t *gs = getGs();
  GameInfo_t gi;
  gi.field = createMatrix(FIELD_ROWS, FIELD_COLS);
  if (gs->status != Initial) {
    copyField(&gi, gs);
  }

  gi.next = gs->next;
  gi.score = gs->score;
  gi.high_score = gs->high_score;
  gi.level = gs->level;
  gi.speed = gs->speed;
  gi.pause = gs->pause;

  if (gs->button == Start && gs->status == Initial) {
    initGameState();
    gs->status = Spawn;
    gs->is_play = true;
  }

  if (gs->status == Spawn) {
    gs->figure = gs->next;
    gs->next = generateFigure();
    gs->x = SPAWN_X;
    gs->y = SPAWN_Y;
    gs->is_play = true;
    gs->status = Moving;
  } else if (gs->status == Moving) {
    if (gs->button == Down && canMove(gs)) {
      gs->y++;
      gs->time.timeStart = get_time_ms(); // Сброс таймера после падения
    } else if (gs->button == Left || gs->button == Right || gs->button == Up || gs->button == ERR) {
      moveFigure(gs, &gi);
    }
  }
  if (gs->is_play) {
    updateField(&gi, gs);
  }

  if (gs->status == Attaching) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (gs->figure[i][j] == 1) {
          int fy = gs->y + i;
          int fx = gs->x + j;
          gs->field[fy][fx] = 1;
        }
      }
    }
    gs->status = Spawn;

    copyField(&gi, gs);
    mvprintw(9, 2, "Attacing\n");

    ////////////////////////////////
    FILE *out = fopen("field.txt", "w");
    if (out) {
      for (int row = 0; row < FIELD_ROWS; row++) {
        for (int col = 0; col < FIELD_COLS; col++) {
          fputc(gi.field[row][col] ? '*' : '.', out);
        }
        fputc('\n', out);
      }
      fclose(out);
    } else {
      mvprintw(10, 2, "ERROR: cannot open field.txt for writing");
    }

    // 4) Отладочное сообщение на экран
    mvprintw(9, 2, "Attaching -> wrote field.txt");
  }
  return gi;
}

void initGameState() {
  GameState_t *gs = getGs();
  gs->field = createMatrix(FIELD_ROWS, FIELD_COLS);
  gs->figure = generateFigure();
  gs->next = generateFigure();
  // gs->x = SPAWN_X;
  // gs->y = SPAWN_Y;
  gs->score = 0;
  gs->high_score = 10000;
  gs->level = 1;
  gs->speed = 700000;
  gs->pause = 0;
}

void moveFigure(GameState_t *gs, GameInfo_t *gi) {
  switch (gs->button) {
    case Left:
      if (canMove(gs)) {
        gs->x--;
        // updateField(gi, gs);
      }
      break;
    case Right:
      if (canMove(gs)) {
        gs->x++;
        // updateField(gi, gs);
      }
      break;
    case Down:
    case ERR:
      if (canMove(gs)) {
        gs->y++;
        // updateField(gi, gs);
      }
      break;
    case Up:
      break;
    default:
      break;
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
      return 1;  // остальные кнопки всегда ОК
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gs->figure[i][j] == 1) {
        int fy = gs->y + i + dy;
        int fx = gs->x + j + dx;

        // проверка границ
        if (fx < 0 || fx >= FIELD_COLS || fy < 0 || fy >= FIELD_ROWS) {
          if (dy == 1) gs->status = Attaching;  // если ударились снизу
          return 0;
        }

        // проверка на столкновение с другими фигурами
        if (gs->field[fy][fx]) {
          if (dy == 1) gs->status = Attaching;
          return 0;
        }
      }
    }
  }

  return 1;
}

void updateField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int y = gs->y + i;
      int x = gs->x + j;
      if (gs->figure[i][j] == 1) {
        if (x >= 0 && x < FIELD_COLS && y >= 0 && y < FIELD_ROWS) {
          gi->field[y][x] = gs->figure[i][j];
          mvprintw(8, 2, "field[%d][%d]", y, x);
        }
      }
    }
  }
}

int **generateFigure() {
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

void copyField(GameInfo_t *gi, GameState_t *gs) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      gi->field[i][j] = gs->field[i][j];
    }
  }
}

void freeGameInfo(GameInfo_t *gi) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    free(gi->field[i]);
  }
  free(gi->field);
}

// #endif