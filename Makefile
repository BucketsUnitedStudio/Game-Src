PRE-COMPILE =  mkdir -p build;
CC = $(PRE-COMPILE) bear -- clang

SRC_FILE = ./src/main.c ./src/game.c
SRC_EXEC = ./build/game
FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -pedantic -std=c23 -Wno-gnu-flexible-array-initializer -O1

DBG_FLAGS = -ggdb

WIN_FLAGS = $(shell pkg-config --cflags --libs sdl2_ttf sdl2_image)

PKG_CONFIG_FLAGS = $(shell pkg-config --libs --cflags sdl2 SDL2_image SDL2_ttf)

all: 
	$(CC) $(FLAGS) $(SRC_FILE) -o $(SRC_EXEC) $(FLAGS) $(PKG_CONFIG_FLAGS)

debug:
	$(CC) $(FLAGS) $(SRC_FILE) -o $(SRC_EXEC) $(FLAGS) $(DBG_FLAGS) $(PKG_CONFIG_FLAGS)
	
compilation-database:
	# @echo "$(FLAGS) $(PKG_CONFIG_FLAGS)" | tr " " "\n" > compile_commands.txt
	bear -- make

cmake:
	cmake -B ./build

debug-cmake:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	make -C build

run:
	# make -C build
	./$(SRC_EXEC)

crosscomp:
	x86_64-w64-mingw32-gcc $(SRC_FILE) -o $(SRC_EXEC).win.exe -mwindows -static -static-libgcc -L/usr/x86_64-w64-mingw32/static/lib/ $(shell x86_64-w64-mingw32-pkg-config --cflags --libs --static sdl2 SDL2_image SDL2_ttf) 
	
