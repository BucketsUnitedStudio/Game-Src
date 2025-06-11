# Game-Src
Game source files and documentation

# Tools we are using
- The C language with the SDL2 library.
- Cython (to convert python to C)

# General structure of files
- ./README.md For general messages as to how to compile/use the source.
- ./Makefile 
- ./build To put executables so they are not uploaded into the git repo
- ./src For C source files
- ./doc Any documentation that we want to share

# Notes
\*\* 
Make sure that you use SDL\_Renderer and SDL\_Texture instead of SDL\_Surface.
The Renderer and Textures use the GPU (hardware acceleration), and the
Surfaces use the CPU (for modifying images and such, but not very fast).
\*\*
