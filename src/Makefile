CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lm -lncurses

SRC_DIR = .
GUI_DIR = gui/cli
GAME_DIR = brick_game/tetris

SRC_FILES = $(GUI_DIR)/frontend.c $(GAME_DIR)/backend.c
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = $(GUI_DIR)/frontend

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cppcheck:
	cppcheck --check-level=exhaustive --enable=all --suppress=missingIncludeSystem $(GUI_DIR)/*.c $(GAME_DIR)/*.c

clang:
	clang-format -n $(GUI_DIR)/*.c $(GAME_DIR)/*.c $(GUI_DIR)/*.h $(GAME_DIR)/*.h

clean:
	rm -f $(OBJ_FILES) $(TARGET) 
