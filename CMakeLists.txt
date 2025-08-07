cmake_minimum_required(VERSION 3.10)



# Define source and executable
set(SRC_FILE src/main.c src/game.c)
set(SRC_EXEC game)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message("Cross comp for Windows")
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain-windows.cmake)
  target_link_options(${SRC_EXEC} PRIVATE
      -static
      -static-libgcc
      -static-libstdc++
      -mwindows
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "WebAsm")
  message("Cross comp for WebAsm")
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain-WebAsm.cmake)
else()
  message("Compilation natively for Linux")
  # Set the compiler to clang
  set(CMAKE_C_COMPILER clang)
endif()

project(GameProject LANGUAGES C)
set (CMAKE_C_STANDARD 23)
set (CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
set(PKG_CONFIG_USE_STATIC_LIBS ON)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET GLOBAL sdl2)
pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET GLOBAL SDL2_image)
pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET GLOBAL SDL2_ttf)

add_executable(${SRC_EXEC} ${SRC_FILE})

target_link_libraries(${SRC_EXEC} PRIVATE PkgConfig::SDL2 PkgConfig::SDL2_image
  PkgConfig::SDL2_ttf)

message("C compiler is ${CMAKE_C_COMPILER}")
message("SDL2 libs at ${PKG_CONFIG_EXECUTABLE}")

get_target_property(SDL2_INCLUDE_DIRS PkgConfig::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_COMPILE_OPTIONS PkgConfig::SDL2 INTERFACE_COMPILE_OPTIONS)
get_target_property(SDL2_LINK_LIBRARIES PkgConfig::SDL2 INTERFACE_LINK_LIBRARIES)
get_target_property(SDL2_LINK_OPTIONS PkgConfig::SDL2 INTERFACE_LINK_OPTIONS)

message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 compile options: ${SDL2_COMPILE_OPTIONS}")
message(STATUS "SDL2 link libraries: ${SDL2_LINK_LIBRARIES}")
message(STATUS "SDL2 link options: ${SDL2_LINK_OPTIONS}")
