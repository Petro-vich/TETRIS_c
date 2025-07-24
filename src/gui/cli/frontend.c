#include "frontend.h"

int main(void) {
  WIN_INIT(700);
  gameLoop();
  return 0;
}

void gameLoop() {
  GameState_t *gs = getGs();
  GameInfo_t gi;
  GameWindows_t window;

  while (gs->button != Terminate) {
    initWindows(&window);

    getUserInput(gs);

    gi = updateCurrentState();
    draw(&gi, &window);
    freeGameInfo(&gi);
  }

  freeGameState(gs);

  endwin();
}

void getUserInput(GameState_t *gs) {
  int button = getch();

  switch (button) {
    case KEY_ENTER:
    case '\n':
      gs->button = Start;
      break;
    case 'p':
      gs->button = Pause;
      gs->pause = true;
      break;
    case 'q':
      gs->button = Terminate;
      break;
    case KEY_LEFT:
      gs->button = Left;
      break;
    case KEY_RIGHT:
      gs->button = Right;
      break;
    case KEY_UP:
      gs->button = Up;
      break;
    case KEY_DOWN:
      gs->button = Down;
      break;
    case 'a':
      gs->button = Action;
      break;
    case ' ':
      gs->button = Space;
      break;
    default:
      gs->button = ERR;
      break;
  }
}

void draw(GameInfo_t *gi, GameWindows_t *window) {
  GameState_t *gs = getGs();

  if (gs->status == Initial) {
    box(window->waitEnter, 0, 0);
    wattron(window->waitEnter, COLOR_PAIR(3));
    mvwprintw(window->waitEnter, 3, 7, "PREESS ENTER");
    wnoutrefresh(window->waitEnter);
  }

  if (gs->is_play) {
    clearWinGame(window);
    renderWinGame(gi, window);
    renderInfoWin(gi, gs, window);
    wnoutrefresh(window->main);
    wnoutrefresh(window->game);
    wnoutrefresh(window->info);
    wnoutrefresh(window->newtFigure);
    // renderHelpInfo(gs);
  }

  if (gs->status == Gameover) {
    erase();
    int row, col;
    getmaxyx(stdscr, row, col);
    mvwprintw(stdscr, row / 2, (col - 38) / 2,
              "GameOver, press ENTER to try again");
    gs->status = Initial;
  }

  doupdate();
}

void clearWinGame(GameWindows_t *window) {
  int y, x;
  getmaxyx(window->game, y, x);

  for (int i = 1; i < y - 1; i++) {
    for (int j = 1; j < x - 1; j++) {
      mvwaddch(window->game, i, j, ' ');
    }
  }
}

void renderWinGame(GameInfo_t const *gi, GameWindows_t *window) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    for (int j = 0; j < FIELD_COLS; j++) {
      wattron(window->game, COLOR_PAIR(10));
      if (gi->field[i][j] == 1)
        mvwprintw(window->game, i + 1, (j * 2) + 1, "  ");
    }
  }
}

void renderInfoWin(GameInfo_t *gi, GameState_t const *gs,
                   GameWindows_t *window) {
  wattron(window->info, COLOR_PAIR(1));
  mvwprintw(window->info, 1, 2, "Next:");

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      wattron(window->newtFigure, COLOR_PAIR(10));
      if (gs->next[i][j] == 1)
        mvwprintw(window->newtFigure, i + 1, (j * 2) + 2, "  ");
    }
  }

  wattron(window->info, COLOR_PAIR(2));
  mvwprintw(window->info, 8, 1, "Score: %d", gi->score);
  mvwprintw(window->info, 10, 1, "Max Score:");
  mvwprintw(window->info, 11, 1, "%d", gi->highScore);

  wattron(window->info, COLOR_PAIR(3));
  mvwprintw(window->info, 13, 1, "Level: %d", gi->level);
  // mvwprintw(window->info, 14, 1, "status: %d", gs->status);
  // mvwprintw(window->info, 16, 1, "button: %d", gs->button);
  // mvwprintw(window->info, 18, 1, "speed: %d", gs->speed / 1000);
}

// void renderHelpInfo(GameState_t *gs) {
//   mvprintw(4, 2, "figure positin y: %d", gs->y);
//   mvprintw(6, 2, "figure positin x: %d", gs->x);
// }

void initWindows(GameWindows_t *window) {
  // 2. Параметры основного окна
  int total_height = GAME_HEIGHT + BORDER_HEIGHT + BORDER_HEIGHT;
  int info_width = 15;
  int total_width = (GAME_WIDTH * 2) + info_width + BORDER_WIDTH + BORDER_WIDTH;
  int start_y = LINES / 2 - total_height / 2;
  int start_x = COLS / 2 - total_width / 2;

  // 3. Создание основного окна
  window->main = newwin(total_height, total_width, start_y, start_x);
  box(window->main, 0, 0);

  window->waitEnter = newwin(7, 27, start_y, start_x);
  box(window->waitEnter, 0, 0);

  // 4. Создание игрового поля
  window->game = derwin(window->main, GAME_HEIGHT + BORDER_HEIGHT,
                        (GAME_WIDTH * 2) + BORDER_WIDTH, 1, 1);
  box(window->game, 0, 0);

  // 5. Создание информационного окна (увеличенная ширина)
  window->info = derwin(window->main, GAME_HEIGHT + BORDER_HEIGHT, info_width,
                        1, (GAME_WIDTH * 2) + 3);
  box(window->info, 0, 0);

  // 6. Создание окна следующей фигуры (ширина x2)
  window->newtFigure = derwin(window->info, 4, 12, 2, 2);
  box(window->newtFigure, 0, 0);
}
