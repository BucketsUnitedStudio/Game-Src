cmake_minimum_required(VERSION 3.10)

project(GameProject)

# Set the compiler to clang
set(CMAKE_C_COMPILER clang)

set (CMAKE_C_STANDARD 23)

# Define source and executable
set(SRC_FILE ./src/main.c)
set(SRC_EXEC ./build/game)

# Find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Create the executable
add_executable(game ${SRC_FILE})

# Link libraries
target_link_libraries(game SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

# Custom target to run the game
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMAND ./build/game
    DEPENDS game
)
