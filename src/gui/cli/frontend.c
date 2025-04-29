#include "../../brick_game/tetris/backend.h"
#include "frontend.h"

int main(void) {
  gameLoop();
  return 0;
}   

void gameLoop() {
  GameState_t *gs = getGs();
  GameInfo_t gi;
  

  while (gs->status != Terminate) {
    gi = updateCurrentState(gs);
    printField(gi.field);
    handleUserInput(gs);
    freeGameInfo(&gi);
  }
}

void handleUserInput(GameState_t *gs) {
  int key = getchar();
  while (getchar() != '\n');
  
  switch (key) {
    case 'q':
      gs->status = Terminate;
      break;
    case 'p':
      gs->status = Pause;
      break;
    case ENTER:
      gs->status = Initial; 
      break;
    case '2':
      gs->status = Pause;
      break;
    case LEFT:
      gs->status = Left;
      break;
    case RIGHT:
      gs->status = Right;
      break;
    case DOWN:
      gs->status = Down;
      break;
    case '6':

      
  }
}