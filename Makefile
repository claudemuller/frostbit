UNAME_S := $(shell uname -s)
CC=gcc
CFLAGS=-Wall -Wfatal-errors -Wextra -pedantic -Wmissing-declarations -std=c11
ASANFLAGS=-fsanitize=address -fno-common -fno-omit-frame-pointer
INCS=-I./libs/
LIBS=
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -lglib-2.0 -lintl
SRC=./src/*.c \
	./src/systems/*.c
DETECT_LEAKS=ASAN_OPTIONS=detect_leaks=1
BIN=gameengine

# Mac Stuff
ifeq ($(UNAME_S),Darwin)
	INCS+= -I/opt/homebrew/include -I/opt/homebrew/Cellar/sdl2/2.26.3/include/SDL2/ -I/opt/homebrew/Cellar/glib/2.76.1/include/glib-2.0 -I/opt/homebrew/Cellar/glib/2.76.1/lib/glib-2.0/include -I/opt/homebrew/opt/gettext/include -I/opt/homebrew/Cellar/pcre2/10.42/include
	LIBS+= -L/opt/homebrew/lib -L/opt/homebrew/lib/ -L/opt/homebrew/Cellar/glib/2.76.1/lib -L/opt/homebrew/opt/gettext/lib
	DETECT_LEAKS=
endif

all: clean build run

build:
	$(CC) $(SRC) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -o $(BIN)

build_small:
	$(CC) -Os -fdata-sections -ffunction-sections $(SRC) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -Wl,-dead_strip -o $(BIN)


debug:
	@$(CC) -g $(SRC) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -o debug

memcheck:
	@$(CC) -g $(SRC) $(ASANFLAGS) $(CFLAGS) $(INCS) $(LIBS) $(LFLAGS) -o memcheck.out
	@$(DETECT_LEAKS) ./memcheck.out
	@echo "Memory check passed"

run:
	./$(BIN)

clean:
	rm -rf $(BIN) debug*
