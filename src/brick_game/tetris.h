#ifndef TETRIC_H
#define TETIC_H

#define FIELD_WIGHT 10
#define FIELD_HEIGHT 20
#define SPAWN_X 4
#define SPAWN_Y 0

#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

typedef struct
{
  char* status;
  int** field;
  int** figure;
  int score;
  int cd_figure_x;
  int cd_figure_y
} GameState_t;

void init_game();
void check_status();
void print_matrix();
void generate_figure();
void move_figure();
void move_figure();

#endif