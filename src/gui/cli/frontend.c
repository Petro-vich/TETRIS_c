#include "../../brick_game/brickgame.h"
#include "../../brick_game/tetris/tetris.h"

int main(void) {
  initGameState();

  GameState_t *gs = getGs();
  GameInfo_t gi = updateCurrentState(gs);
}