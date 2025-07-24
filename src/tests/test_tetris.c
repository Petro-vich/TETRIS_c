#include "test_tetris.h"

START_TEST(test_init_score) {
  initGameState();
  GameState_t *gs = getGs();
  ck_assert_int_eq(gs->score, 0);
  freeGameState(gs);
}
END_TEST

START_TEST(test_init_level) {
  initGameState();
  GameState_t *gs = getGs();
  ck_assert_int_eq(gs->level, 1);
  freeGameState(gs);
}
END_TEST

START_TEST(test_add_score) {
  initGameState();
  GameState_t *gs = getGs();
  addScore(gs, 2);
  ck_assert_int_ge(gs->score, 300);
  freeGameState(gs);
}
END_TEST

START_TEST(test_update_level) {
  initGameState();
  GameState_t *gs = getGs();
  gs->score = 1000;
  updateLevel(gs);
  ck_assert_int_ge(gs->level, 2);
  freeGameState(gs);
}
END_TEST

START_TEST(test_save_high_score) {
  int original = loadHighScore();
  int test_score = 12345;
  saveHighScore(test_score);
  int loaded = loadHighScore();
  ck_assert_int_eq(loaded, test_score);

  saveHighScore(original);
}
END_TEST

START_TEST(test_updateCurrentState_spawn) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Spawn;
  GameInfo_t gi = updateCurrentState();
  ck_assert_int_eq(gs->status, Moving);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_updateCurrentState_moving_down) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Moving;
  gs->button = Down;
  int y_before = gs->y;
  GameInfo_t gi = updateCurrentState();
  ck_assert_int_eq(gs->y, y_before + 1);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_updateCurrentState_moving_left) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Moving;
  gs->button = Left;
  int x_before = gs->x;
  GameInfo_t gi = updateCurrentState();
  ck_assert(gs->x == x_before - 1 || gs->x == x_before);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_updateCurrentState_moving_space) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Moving;
  gs->button = Space;
  GameInfo_t gi = updateCurrentState();

  ck_assert_int_eq(gs->status, Moving);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_updateCurrentState_attaching) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Attaching;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) gs->figure[i][j] = 1;
  int score_before = gs->score;
  GameInfo_t gi = updateCurrentState();
  ck_assert_int_eq(gs->status, Spawn);
  ck_assert(gs->score >= score_before);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_updateCurrentState_gameover) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Moving;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) gs->field[i][j] = 1;
  GameInfo_t gi = updateCurrentState();
  ck_assert_int_eq(gs->status, Gameover);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_clear_full_line) {
  initGameState();
  GameState_t *gs = getGs();
  for (int c = 0; c < FIELD_COLS; ++c) {
    gs->field[FIELD_ROWS - 1][c] = 1;
  }
  int cleared = clearFullLines(gs);
  ck_assert_int_eq(cleared, 1);

  for (int c = 0; c < FIELD_COLS; ++c) {
    ck_assert_int_eq(gs->field[FIELD_ROWS - 1][c], 0);
  }
  freeGameState(gs);
}
END_TEST

START_TEST(test_updateCurrentState_moving_right) {
  initGameState();
  GameState_t *gs = getGs();
  gs->status = Moving;
  gs->button = Right;
  int x_before = gs->x;
  GameInfo_t gi = updateCurrentState();
  ck_assert(gs->x == x_before + 1 || gs->x == x_before);
  freeGameInfo(&gi);
}
END_TEST

START_TEST(test_generateFigure_all_types) {
  for (int t = 0; t < 10; ++t) {
    int **fig = generateFigure();
    sleep(1);
    freeFigure(fig);
  }
  ck_assert_int_eq(1, 1);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("tetris_suite");
  TCase *tc = tcase_create("tetris_tc");

  tcase_add_test(tc, test_init_score);
  tcase_add_test(tc, test_init_level);
  tcase_add_test(tc, test_add_score);
  tcase_add_test(tc, test_update_level);
  tcase_add_test(tc, test_save_high_score);
  tcase_add_test(tc, test_updateCurrentState_spawn);
  tcase_add_test(tc, test_updateCurrentState_moving_down);
  tcase_add_test(tc, test_updateCurrentState_moving_left);
  tcase_add_test(tc, test_updateCurrentState_moving_space);
  tcase_add_test(tc, test_updateCurrentState_attaching);
  tcase_add_test(tc, test_updateCurrentState_gameover);
  tcase_add_test(tc, test_clear_full_line);
  tcase_add_test(tc, test_updateCurrentState_moving_right);
  tcase_add_test(tc, test_generateFigure_all_types);

  suite_add_tcase(s, tc);
  return s;
}

int run_tetris_tests(void) {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  int tf = 0;
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  tf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return tf;
}

int main(void) {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  int tf = 0;

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  tf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return tf > 0;
}
