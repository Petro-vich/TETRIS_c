#include <ncurses.h>
#include <unistd.h>

#define GAME_HEIGHT 20
#define GAME_WIDTH 10
#define INFO_WIDTH 15
#define BORDER_WIDTH 2

int main(void) {
    // 1. Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color(); // Включаем поддержку цветов
    
    // Инициализация цветов
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Обычный текст
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Счёт
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Уровень
    
    // 2. Параметры основного окна
    int total_height = GAME_HEIGHT + BORDER_WIDTH;
    int total_width = GAME_WIDTH + INFO_WIDTH + BORDER_WIDTH + BORDER_WIDTH; //Два окна по 2 символа
    int start_y = LINES / 2 - total_height / 2;
    int start_x = COLS / 2 - total_width / 2;

    // 3. Создание основного окна
    WINDOW *main_win = newwin(total_height, total_width, start_y, start_x);
    box(main_win, 0, 0);
    
    // 4. Создание игрового поля
    WINDOW *game_win = derwin(main_win, GAME_HEIGHT, GAME_WIDTH + BORDER_WIDTH, 1, 1);
    box(game_win, 0, 0);
    
    // 5. Создание информационного окна
    WINDOW *info_win = derwin(main_win, GAME_HEIGHT, INFO_WIDTH, 1, GAME_WIDTH + 3);
    box(info_win, 0, 0);
    
    // 6. Создание окна следующей фигуры
    WINDOW *newtFigure = derwin(info_win, 6, 6, 2 , 4);
    box(newtFigure, 0, 0);

    // 6. Отрисовка информационного окна
    wattron(info_win, COLOR_PAIR(1));
    mvwprintw(info_win, 1, 5, "Next:");
    
    wattron(info_win, COLOR_PAIR(2));
    mvwprintw(info_win, 8, 4, "Score: 0");
    mvwprintw(info_win, 10, 2, "Max Score: 0");
    
    wattron(info_win, COLOR_PAIR(3));
    mvwprintw(info_win, 12, 4, "lvl: 1");
    
    // 7. Обновление окон
    wrefresh(main_win);
    wrefresh(game_win);
    wrefresh(info_win);
    
    // 8. Ожидание нажатия клавиши
    usleep(40000000);
    
    // 9. Освобождение ресурсов
    delwin(info_win);
    delwin(game_win);
    delwin(main_win);
    endwin();
    
    return 0;
}