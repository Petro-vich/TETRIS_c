#include "../../brick_game/tetris/backend.h"
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
  initWindows(&window);
  

  
  while (gs->button != Terminate) {
    getUserInput(gs);
    gi = updateCurrentState();
    Draw(&gi, &window);
    freeGameInfo(&gi);
    gs->button = ERR;
  }
  // cleanupWindows();
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
    default:
      gs->button = ERR;
      break;
  }
}

// void getUserInput(GameState_t *gs) {
//   int button = ERR;

//   // 1) Открываем файл
//   FILE *fp = fopen("getch.txt", "r");
//   if (fp) {
//       // 2) Считываем один символ
//       int c = fgetc(fp);
//       if (c != EOF) {
//           button = c;
//       }
//   }

//   switch (button) {
//     case 's':
//     case '\n':
//       gs->button = Start; 
//       break;
//     case 'p':
//       gs->button = Pause;
//       break;
//     case 'q':
//       gs->button = Terminate;
//       break;
//     case KEY_LEFT:
//       gs->button = Left;
//       break;
//     case KEY_RIGHT:
//       gs->button = Right;
//       break;
//     case KEY_UP:  
//       gs->button = Up;
//       break;
//     case 'd':
//       gs->button = Down;
//       break;
//     case 'a':
//       gs->button = Action;
//     break;
//     default:
//       gs->button = ERR;
//       break;
//   }
// }


// typedef enum {
//     Start,
//     Pause,
//     Terminate,
//     Left,
//     Right,
//     Up,
//     Down,
//     Action
// } UserAction_t;