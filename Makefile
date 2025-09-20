CC = tcc
DBG_BIN = lldb
CFLAGS = #-D_GNU_SOURCE
CFLAGS += -std=c11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
# CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -I./libs/
ASANFLAGS=-fsanitize=address -fno-common -fno-omit-frame-pointer
CFLAGS += $(shell pkg-config --cflags sdl3 sdl3-image)
LDFLAGS = $(shell pkg-config --libs sdl3 sdl3-image)
LIBS =
SRC = ./src/*.c
BIN_DIR = ./bin
BIN = $(BIN_DIR)/lemmings

build: bin-dir
	$(CC) $(CFLAGS) $(LIBS) $(SRC) -o $(BIN) $(LDFLAGS)

bin-dir:
	mkdir -p $(BIN_DIR)

debug: debug-build
	$(DBG_BIN) $(BIN) $(ARGS)

debug-build: bin-dir
	$(CC) $(CFLAGS) -g $(LIBS) $(SRC) -o $(BIN) $(LDFLAGS)

run: build
	@$(BIN) $(ARGS)

memcheck:
	@$(CC) $(ASANFLAGS) $(CFLAGS) -g $(LIBS) $(SRC) $(LDFLAGS) -o $(BIN_DIR)/memcheck.out
	@$(BIN_DIR)/memcheck.out
	@echo "Memory check passed"

valgrid:
	$(CC) $(CFLAGS) -g $(LIBS) $(SRC) -o $(BIN_DIR)/memcheck.out $(LDFLAGS)
	valgrind --leak-check=yes $(BIN_DIR)/memcheck.out

leakscheck:
	leaks -atExit -- $(BIN)

clean:
	rm -rf $(BIN_DIR)/* $(TEST_DIR)/tests*

gen-compilation-db:
	bear -- make build
