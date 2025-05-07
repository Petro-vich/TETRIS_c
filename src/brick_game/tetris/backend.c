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
  }else if (gs->status == Moving) {
    if (gs->button == Left || gs->button == Right ||
        gs->button == Down || gs->button == Up || gs->button == ERR) {
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
          fputc( gi.field[row][col] ? '*' : '.', out );
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
    default: break;
  }

}

// void updateField(GameInfo_t *gi, GameState_t *gs) {  //Хз почему core dumped
//   for (int i = 0; i < 4; i++) {
//     for (int j = 0; j < 4; j++) {
//       int y = gs->y + i;
//       int x = gs->x + j;
//       if (gs->figure[i][j] == 1) {
//         gi->field[y][x] = gs->figure[i][j];
//         mvprintw(8, 2, "field[%d][%d]", gs->y + i, gs->x +j);
//       }
//     }
//   }
// }

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

int canMove(GameState_t *gs) {
  int dx = 0, dy = 0;

  switch (gs->button) {
    case Left:  dx = -1; break;
    case Right: dx = 1;  break;
    case Down: 
    case ERR:   dy = 1;  break;
    default: return 1;  // остальные кнопки всегда ОК
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



void initWindows(GameWindows_t *window) {
  // 2. Параметры основного окна
  int total_height = GAME_HEIGHT + BORDER_HEIGHT + BORDER_HEIGHT;
  int total_width = GAME_WIDTH + INFO_WIDTH + BORDER_WIDTH + BORDER_WIDTH; //Два окна по 2 символа
  int start_y = LINES / 2 - total_height / 2;
  int start_x = COLS / 2 - total_width / 2;
  
  // 3. Создание основного окна
  window->main = newwin(total_height, total_width, start_y, start_x);
  box(window->main, 0, 0);

  window->waitEnter = newwin(7, 27, start_y, start_x);
  box(window->waitEnter, 0, 0);

  // 4. Создание игрового поля
  window->game = derwin(window->main, GAME_HEIGHT + BORDER_HEIGHT, GAME_WIDTH + BORDER_WIDTH, 1, 1);
  box(window->game, 0, 0);

  // 5. Создание информационного окна
  window->info = derwin(window->main, GAME_HEIGHT + BORDER_HEIGHT, INFO_WIDTH, 1, GAME_WIDTH + 3);
  box(window->info, 0, 0);

  // 6. Создание окна следующей фигуры
  window->newtFigure = derwin(window->info, 4, 6, 2 , 4);
  box(window->newtFigure, 0, 0);

  // 6. Creating window wait input ENTER
 

}

void renderInfoWin(GameInfo_t *gi, GameState_t *gs, GameWindows_t *window) {
  wattron(window->info, COLOR_PAIR(1));
  mvwprintw(window->info, 1, 5, "Next:");

  wattron(window->info, COLOR_PAIR(2));
  mvwprintw(window->info, 8, 4, "Score: %d", gi->score);
  mvwprintw(window->info, 10, 2, "Max Score: 0");

  wattron(window->info, COLOR_PAIR(3));
  mvwprintw(window->info, 12, 4, "lvl: %d", gi->level);
  mvwprintw(window->info, 14, 1, "status: %d", gs->status);
  mvwprintw(window->info, 16, 1, "button: %d", gs->button);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gs->next[i][j] == 1) {
        mvwprintw(window->newtFigure, i + 1, j + 1, "*");
      }
    }
  }  
  
  

}

void clearWinGame(GameInfo_t *gi, GameWindows_t *window) {
  int y, x;
  getmaxyx(window->game, y, x);

  for (int i = 1; i < y-1; i++) {
    for (int j = 1; j < x-1; j++) {
      mvwaddch(window->game, i, j, ' ');
    }
  }
}

void renderHelpInfo(GameState_t *gs) {
  mvprintw(4, 2, "figure positin y: %d", gs->y);
  mvprintw(6, 2, "figure positin x: %d", gs->x);

}

void renderWinGame(GameInfo_t *gi, GameWindows_t *window) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      if (gi->field[i][j] == 1) {
        mvwaddch(window->game, i+1, j+1, '*');
      }
    }
  }
}


void Draw(GameInfo_t *gi, GameWindows_t *window) {
  GameState_t *gs = getGs();
  
  if (gs->status == Initial) {
    box(window->waitEnter, 0, 0);
    wattron(window->waitEnter, COLOR_PAIR(3));
    mvwprintw(window->waitEnter, 3, 7, "PREESS ENTER");
    wnoutrefresh(window->waitEnter);
  } 

  if (gs->is_play){
    clearWinGame(gi, window);
    renderWinGame(gi, window);
    renderInfoWin(gi, gs, window);
    wnoutrefresh(window->main);
    wnoutrefresh(window->game);
    wnoutrefresh(window->info);
    wnoutrefresh(window->newtFigure);
  }

  renderHelpInfo(gs);

  doupdate();
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
  gs->score = 0;
  gs->high_score = 10000;
  gs->level = 1;
  gs->speed = 500000;
  gs->pause = 0;
  
  
}




// #endif