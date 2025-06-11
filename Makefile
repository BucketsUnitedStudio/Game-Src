CC = clang

SRC_FILE = ./src/main.c
SRC_EXEC = ./build/game
FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)
