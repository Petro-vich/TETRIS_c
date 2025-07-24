#include "includes/lvl.h"

#define HIGH_SCORE_FILE "highscore.txt"

void addScore(GameState_t *gs, int lines_cleared) {
  if (lines_cleared > 0) {
    const int score_table[5] = {0, 100, 300, 700, 1500};
    if (lines_cleared < 5) {
      gs->score += score_table[lines_cleared];
    } else {
      gs->score += score_table[4];
    }
    updateHighScore(gs);
    updateLevel(gs);
  }
}

void updateLevel(GameState_t *gs) {
  int new_level = (gs->score / 600) + 1;
  if (new_level > 10) new_level = 10;

  if (new_level > gs->level) {
    gs->level = new_level;
    gs->speed = 700000 - (gs->level - 1) * 50000;
    if (gs->speed < 200000) gs->speed = 200000;
  }
}

int loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  int high_score = 0;
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

void saveHighScore(int score) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", score);
    fclose(file);
  }
}

void updateHighScore(GameState_t *gs) {
  if (gs->score > gs->highScore) {
    gs->highScore = gs->score;
    saveHighScore(gs->highScore);
  }
}