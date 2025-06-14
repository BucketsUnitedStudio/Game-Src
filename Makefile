CC = clang

SRC_FILE = ./src/main.c 
SRC_EXEC = ./build/game
FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

WIN_FLAGS = $(shell pkg-config --cflags --libs sdl2_ttf sdl2_image)

all:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)

run:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)
	./$(SRC_EXEC)

windows:
	gcc $(SRC_FILE) -o $(SRC_EXEC) $(WIN_FLAGS)
