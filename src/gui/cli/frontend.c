#include "../../brick_game/tetris/backend.h"
#include "frontend.h"


int main(void) {
  WIN_INIT(50);

  gameLoop();
  return 0;
}   

void gameLoop() {
  GameState_t *gs = getGs();
  GameInfo_t gi;
  
  while (gs->status != Terminate) {
    gi = updateCurrentState(gs);
    Draw(&gi);
    handleUserInput(gs);
    freeGameInfo(&gi);
  }
}


void handleUserInput(GameState_t *gs) {
  int key = getch();
  
  switch (key) {
    case 'q':
      gs->status = Terminate;
      break;
    case 'p':
      gs->status = Pause;
      break;
    case KEY_ENTER:
      gs->status = Initial; 
      break;
    case KEY_LEFT:
      gs->status = Left;
      break;
    case KEY_RIGHT:
      gs->status = Right;
      break;
    case KEY_DOWN:
      gs->status = Down;
      break;
    case KEY_UP:
    //Dont; use
      break;
  }
}
