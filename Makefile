CC = clang

SRC_FILE = ./src/main.c
SRC_EXEC = ./build/game
FLAGS = -w -lSDL2

all:
	$(CC) $(SRC_FILE) $(FLAGS) -o $(SRC_EXEC)
