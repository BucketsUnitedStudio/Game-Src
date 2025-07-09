CC = clang

SRC_FILE = ./src/main.c 
SRC_EXEC = ./build/game
FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -pedantic -Wc23-extensions -O1

DBG_FLAGS = -ggdb

WIN_FLAGS = $(shell pkg-config --cflags --libs sdl2_ttf sdl2_image)

all:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)

debug:
	$(CC) $(SRC_FILE) $(FLAGS) $(DBG_FLAGS) -o $(SRC_EXEC)

run:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)
	./$(SRC_EXEC)

windows:
	gcc $(SRC_FILE) -o $(SRC_EXEC) $(WIN_FLAGS)

crosscomp:
	x86_64-w64-mingw32-gcc $(SRC_FILE) -o $(SRC_EXEC).win.exe -mwindows -static -static-libgcc -L/usr/x86_64-w64-mingw32/static/lib/ $(shell x86_64-w64-mingw32-pkg-config --cflags --libs --static sdl2 SDL2_image SDL2_ttf) 
