CC = clang

SRC_FILE = ../src/main.c ../src/game.c
SRC_EXEC = ../build/game
WARNING_FLAGS = -pedantic -Wno-gnu-flexible-array-initializer -Wmissing-variable-declarations -Wunused-variable -Wswitch-enum -Wunused-parameter -Wsign-compare -Wunreachable-code-break -Wconditional-uninitialized
FLAGS = -Wpedantic -lSDL2 -lSDL2_image -lSDL2_ttf -std=c23  -O1 

DBG_FLAGS = -ggdb

WIN_FLAGS = $(shell pkg-config --cflags --libs sdl2_ttf sdl2_image)

all:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)

error:
	$(CC) $(SRC_FILE) $(WARNING_FLAGS) $(FLAGS) -o $(SRC_EXEC)

Weverything:
	$(CC) $(SRC_FILE) $(FLAGS) -Weverything -Wno-c++98-compat -Wno-pre-c23-compat -o $(SRC_EXEC) 

debug:
	$(CC) $(SRC_FILE) $(FLAGS) $(DBG_FLAGS) -o $(SRC_EXEC)

run:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)
	./$(SRC_EXEC)

windows:
	gcc $(SRC_FILE) -o $(SRC_EXEC) $(WIN_FLAGS)

crosscomp:
	x86_64-w64-mingw32-gcc $(SRC_FILE) -o $(SRC_EXEC).win.exe -mwindows -static -static-libgcc -L/usr/x86_64-w64-mingw32/static/lib/ $(shell x86_64-w64-mingw32-pkg-config --cflags --libs --static sdl2 SDL2_image SDL2_ttf) 
