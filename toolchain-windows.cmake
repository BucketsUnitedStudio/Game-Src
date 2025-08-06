# Cross-compiling for Windows with MinGW

set(CMAKE_SYSTEM_NAME Windows)
# set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)


set(PKG_CONFIG_EXECUTABLE "/usr/bin/x86_64-w64-mingw32-pkg-config" CACHE
  FILEPATH "pkg-config executable")

set(ENV{PKG_CONFIG_LIBDIR}
  "/usr/x86_64-w64-mingw32/lib/pkgconfig:/usr/x86_64-w64-mingw32/static/lib/pkgconfig")

# set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
# set(BUILD_SHARED_LIBS OFF)


# Disable search for native system libraries
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
