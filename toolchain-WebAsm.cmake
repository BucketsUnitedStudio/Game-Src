# Cross-compiling for WebAssembly using Emscripten

set(CMAKE_SYSTEM_NAME Emscripten)
set(CMAKE_C_COMPILER emcc)
set(CMAKE_CXX_COMPILER em++)
set(CMAKE_AR emar)
set(CMAKE_RANLIB emranlib)

# Enable WebAssembly output
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Output folder for WebAssembly artifacts
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build-wasm)

# Extra settings for Emscripten
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s EXPORTED_FUNCTIONS='[\"_main\"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'")

