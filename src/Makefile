CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
LDFLAGS = -lncurses
PREFIX = /usr/local
INSTALL_DIR = $(PREFIX)/bin

SRC_DIR = .
GUI_DIR = gui/cli
GAME_DIR = brick_game/tetris

SRC_FILES = $(GUI_DIR)/frontend.c $(GAME_DIR)/backend.c $(GAME_DIR)/field.c $(GAME_DIR)/figure.c $(GAME_DIR)/lvl.c
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = brickgame

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Unit tests for tetris logic
TEST_SRC = tests/test_tetris.c brick_game/tetris/backend.c brick_game/tetris/field.c brick_game/tetris/figure.c brick_game/tetris/lvl.c
TEST_BIN = test_tetris
CHECK_FLAGS = -lcheck -lm -lpthread -lrt -lsubunit

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

C_FILES = $(shell find . -name "*.c" -type f)
H_FILES = $(shell find . -name "*.h" -type f)
ALL_SOURCE_FILES = $(C_FILES) $(H_FILES)

clean:
	rm -f $(OBJ_FILES) $(TARGET)
	rm -f tests/*.gcda tests/*.gcno tests/*.gcov
	rm -f dvi/*.dvi dvi/*.log dvi/*.aux dvi/*.toc dvi/*.out
	rm -f brickgame-*.tar.gz
	rm -rf coverage_out
	rm -f *~ *.tmp
	rm -f tests/test_tetris


	@echo "Clean complete!"

install: $(TARGET)
	@echo "Installing brickgame to $(INSTALL_DIR)..."
	@sudo mkdir -p $(INSTALL_DIR)
	@sudo cp $(TARGET) $(INSTALL_DIR)/
	@sudo chmod +x $(INSTALL_DIR)/$(TARGET)
	@echo "Installation complete!"

uninstall:
	@echo "Uninstalling brickgame from $(INSTALL_DIR)..."
	@sudo rm -f $(INSTALL_DIR)/$(TARGET)
	@echo "Uninstallation complete!"

dist:
	tar --exclude='*.o' --exclude='*.gc*' --exclude='*.tar.gz' --exclude='brickgame' \
	    --exclude='test_tetris' --exclude='*.dvi' --exclude='*.pdf' \
	    -czf brickgame-`date +%Y%m%d`.tar.gz \
	    Makefile brick_game gui tests dvi .clang-format

dvi:
	@latex -output-directory=dvi doc.tex

gcov_report: clean test
	@mkdir -p coverage_out
	@gcovr --html --html-details -o coverage_out/coverage_report.html
	@cp coverage_report.css coverage_out/ 2>/dev/null || true
	@google-chrome coverage_out/coverage_report.html || chromium-browser coverage_out/coverage_report.html || firefox coverage_out/coverage_report.html || echo "Please open coverage_out/coverage_report.html manually"

valgrind: $(TEST_SRC)
	$(CC) $(CFLAGS) -o tests/$(TEST_BIN) $(TEST_SRC) $(CHECK_FLAGS) $(LDFLAGS)

	$(CC) $(CFLAGS) -g -o $(TARGET) $(SRC_FILES) $(LDFLAGS)

	@valgrind --tool=memcheck --leak-check=yes tests/./$(TEST_BIN)


clang-format:
	clang-format -n $(ALL_SOURCE_FILES)

clang-format-fix:
	clang-format -i $(ALL_SOURCE_FILES)

cppcheck:
	cppcheck --check-level=exhaustive --enable=all --suppress=missingIncludeSystem $(GUI_DIR)/*.c $(GAME_DIR)/*.c
	
test: $(TEST_SRC)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o tests/$(TEST_BIN) $(TEST_SRC) $(CHECK_FLAGS) $(LDFLAGS)
	tests/./$(TEST_BIN)

.PHONY: all clean install uninstall test dist dvi gcov_report leak_check asan_check clang-format-check clang-format-fix clang-format-show clang format-help
