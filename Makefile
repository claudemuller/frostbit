CC = gcc
DBG_BIN = gdb
# CFLAGS = -D_GNU_SOURCE
CFLAGS += -std=gnu11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
# CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -I./libs/
ASANFLAGS=-fsanitize=address -fno-common -fno-omit-frame-pointer
CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_image glib-2.0)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image glib-2.0)
LIBS =
SRC_FILES = ./src/*.c ./src/systems/*.c
BIN_DIR = ./bin
BIN = $(BIN_DIR)/gameengine
TEST_DIR = ./tests
TEST_SRC = $(filter-out ./src/main.c, $(wildcard ./src/*.c)) $(TEST_DIR)/*.c

ifeq ($(shell uname -s),Darwin)
	DBG_BIN = lldb
endif

build: bin-dir
	$(CC) $(CFLAGS) $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

bin-dir:
	mkdir -p $(BIN_DIR)

build_small:
	$(CC) -Os -fdata-sections -ffunction-sections -Wl,-dead_strip $(CFLAGS) $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

debug: debug-build
	$(DBG_BIN) $(BIN) $(ARGS)

debug-build: bin-dir
	$(CC) $(CFLAGS) -g $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

run: build
	@$(BIN) $(ARGS)

test:
	$(CC) $(CFLAGS) $(LIBS) $(TEST_SRC) -o $(TEST_DIR)/tests $(LDFLAGS) && $(TEST_DIR)/tests

test-debug:
	$(CC) $(CFLAGS) -g $(LIBS) $(TEST_SRC) -o $(TEST_DIR)/tests $(LDFLAGS) && lldb $(TEST_DIR)/tests $(ARGS)

memcheck:
	@$(CC) -g $(SRC) $(ASANFLAGS) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -o memcheck.out
	@./memcheck.out
	@echo "Memory check passed"

clean:
	rm -rf $(BIN_DIR)/* $(TEST_DIR)/tests*

gen-compilation-db:
	bear -- make build

gen-compilation-db-make:
	make --always-make --dry-run \
	| grep -wE 'gcc|g\+\+' \
	| grep -w '\-c' \
	| jq -nR '[inputs|{directory:".", command:., file: match(" [^ ]+$").string[1:]}]' \
	> compile_commands.json
