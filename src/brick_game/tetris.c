#include <stdlib.h>
#include "tetris.h"
 
int** create_matrix(int rows, int cols){
  int **temp = malloc(sizeof(int*) * rows);
  for(int i = 0; i < rows; i++) {
    temp[i] = malloc(sizeof(int) * cols);
    for (int j = 0; j < cols; j++) {
      temp[i][j] = 0;
    }
  }
  return temp;
}

GameState_t* get_game_state() {
  static GameState_t gs;
  return &gs;
}

void init_game(){
  GameState_t *gs = get_game_state();
  gs->field = create_matrix(FIELD_HEIGHT, FIELD_WIGHT);
  gs->score = 0;
}

void print_matrix(int **temp) {
  GameState_t *gs = get_game_state();
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIGHT; j++) {
      printf("%d", temp[i][j]);
    }
    putchar('\n');
  }
}

void check_status(){
  GameState_t *gs = get_game_state();
  if (gs->status == "GameOver") {
    exit(1);
  }
}

void generate_figure() {
  GameState_t *gs = get_game_state();
  gs->cd_figure_x = SPAWN_X;
  gs->cd_figure_y = SPAWN_Y;
  gs->figure = create_matrix (4, 4);
  gs->figure[0][0] = 1;
  gs->figure[0][1] = 1;
  gs->figure[1][0] = 1;
  gs->figure[1][1] = 1;
}

void copy_matrix(int **other) {
  GameState_t *gs = get_game_state();
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIGHT; j++)
    other[i][j] = gs->field[i][j];
  }

}

void move_figure() {
  GameState_t *gs = get_game_state();
  
  char key = getchar();  // getch() из ncurses
  switch (key) {
    case 'a':   // Стрелка влево
      gs->cd_figure_x--;
      break;
    case 'd':  // Стрелка вправо
      gs->cd_figure_x++;
      break;
    default:
      break;
  } 



  int **temp = create_matrix(FIELD_HEIGHT, FIELD_WIGHT);
  copy_matrix(temp);


  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      if (temp[gs->cd_figure_y][gs->cd_figure_x+j] != 1){
         temp[gs->cd_figure_y][gs->cd_figure_x+j] = 1;
      }
    }
    gs->cd_figure_y++;
  }
  print_matrix(temp);
  putchar('\n');
  while(key = getchar() != '\n');

}

int main(void) {
  

  init_game();
  generate_figure();
  while (1)
  {  
    check_status();
    move_figure();

  }

  return 0;
}
