#include "includes/field.h"
#include "includes/figure.h"
#include "includes/lvl.h"

#define HIGH_SCORE_FILE "highscore.txt"

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
  gi.highScore = gs->highScore;
  gi.level = gs->level;
  gi.speed = gs->speed;
  gi.pause = gs->pause;

  if (gs->pause) {
    handlePause(gs, &gi);
  }

  if (isShifting(gs) && gs->status != Initial) {
    gs->button = Down;
  }

  if (gs->button == Start && gs->status == Initial) {
    initGameState();
    gs->status = Spawn;
    gs->is_play = true;
  }

  if (gs->status == Spawn) {
    handleSpawn(gs);
  } else if (checkGameOver(gs)) {
    gs->status = Gameover;
  } else if (gs->status == Moving) {
    if (gs->button == Down && canMove(gs)) {
      gs->y++;
      gs->time.timeStart = get_time_ms();
    } else if (gs->button == Space) {
      rotateFigure(gs);
    } else if (gs->button == Left || gs->button == Right || gs->button == ERR) {
      moveFigure(gs);
    }
  } else if (gs->button == Terminate) {
    gs->is_play = false;
  }
  if (gs->is_play) {
    updateField(&gi, gs);
  }

  if (gs->status == Attaching) {
    handleAttaching(gs, &gi);
  }
  return gi;
}

void initGameState() {
  GameState_t *gs = getGs();
  if (gs->field != NULL) {
    for (int i = 0; i < FIELD_ROWS; i++) {
      free(gs->field[i]);
    }
    free(gs->field);
  }
  if (gs->figure != NULL) {
    freeFigure(gs->figure);
  }
  if (gs->next != NULL) {
    freeFigure(gs->next);
  }

  gs->field = createMatrix(FIELD_ROWS, FIELD_COLS);
  gs->figure = generateFigure();
  gs->next = generateFigure();
  gs->score = 0;
  gs->highScore = loadHighScore();
  gs->level = 1;
  gs->speed = 700000;
  gs->pause = 0;
}

void freeGameInfo(GameInfo_t *gi) {
  for (int i = 0; i < FIELD_ROWS; i++) {
    free(gi->field[i]);
  }
  free(gi->field);
}

void freeGameState(GameState_t *gs) {
  if (gs->field != NULL) {
    for (int i = 0; i < FIELD_ROWS; i++) {
      free(gs->field[i]);
    }
    free(gs->field);
    gs->field = NULL;
  }

  if (gs->figure != NULL) {
    for (int i = 0; i < 4; i++) {
      free(gs->figure[i]);
    }
    free(gs->figure);
    gs->figure = NULL;
  }

  if (gs->next != NULL) {
    for (int i = 0; i < 4; i++) {
      free(gs->next[i]);
    }
    free(gs->next);
    gs->next = NULL;
  }
}
