#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_WIDTH 200
#define INITIAL_HEIGHT 200

// Main window
SDL_Window* global_Window = NULL;

// Main Screen Surface
SDL_Surface* global_Screen_Surface = NULL;
SDL_Texture* global_Texture = NULL;


int main() {
    SDL_Surface* initial_Surface = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Was unable to initialize SDL: %s\n",
                SDL_GetError());
        exit(1);
    }

    global_Window = SDL_CreateWindow("The UnNamed (game)",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN);

    if (global_Window == NULL) {
        fprintf(stderr, "Window couldn't be initialized: %s\n",
                SDL_GetError());
        exit(1);
    }

    global_Screen_Surface = SDL_GetWindowSurface(global_Window);

    if (global_Screen_Surface == NULL) {
        fprintf(stderr, 
                "Failed to retrieve main window surface: %s\n",
                SDL_GetError());
    }

    if (SDL_FillRect(global_Screen_Surface, NULL,
                SDL_MapRGBA(global_Screen_Surface->format,
                    255, 255, 255, 255)) < 0) {
        fprintf(stderr, "Couldn't fill Screen with color: %s\n",
                SDL_GetError());
    }

    SDL_UpdateWindowSurface(global_Window);

    int quit = 0;
    SDL_Event currentEvent;
    while (quit != 1) {
        while( SDL_PollEvent(&currentEvent) != 0 ) {
            if (currentEvent.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    printf("Goodbye!\n");

    SDL_FreeSurface(global_Screen_Surface);
    global_Screen_Surface = NULL;
    SDL_DestroyWindow(global_Window);
    global_Window = NULL;
}
