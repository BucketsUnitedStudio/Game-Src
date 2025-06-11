#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_WIDTH 200
#define INITIAL_HEIGHT 200

// Main window
SDL_Window* global_Window = NULL;

// Main Screen Surface
SDL_Surface* global_Screen_Surface = NULL;

// Main renderer and texture
SDL_Renderer* global_Renderer = NULL;
SDL_Texture* global_Texture = NULL;


int main() {
    //Initializing the SDL_Subsystems

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Was unable to initialize SDL: %s\n",
                SDL_GetError());
        exit(1);
    }

    // This function is kinda weird, but I swear this is how it's supposed to
    // work: https://wiki.libsdl.org/SDL2_image/IMG_Init
    if ( (IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) == 0) {
        fprintf(stderr, 
                "Was unable to initialize SLD_IMG extension: %s\n",
                IMG_GetError());
        exit(1);
    }

    global_Window = SDL_CreateWindow("The UnNam'd Gam'd",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN |
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (global_Window == NULL) {
        fprintf(stderr, "Window couldn't be initialized: %s\n",
                SDL_GetError());
        exit(1);
    }

//     global_Screen_Surface = SDL_GetWindowSurface(global_Window);

//     if (global_Screen_Surface == NULL) {
//         fprintf(stderr, 
//                 "Failed to retrieve main window surface: %s\n",
//                 SDL_GetError());
//     }

    // Defining and checking the global_Renderer variable
    global_Renderer = SDL_CreateRenderer(global_Window, -1, SDL_RENDERER_ACCELERATED);

    {
        char* homepage_path = "art/backgrounds/home-page.jpg";
        SDL_Surface* homepage_Surface =
            IMG_Load(homepage_path);

        if (homepage_Surface == NULL) {
            fprintf(stderr, 
                    "Couldn't load the specified image at %s: %s\n",
                    homepage_path, SDL_GetError());
            exit(1);

            // if (SDL_FillRect(global_Screen_Surface, NULL,
            //     SDL_MapRGBA(global_Screen_Surface->format,
            //         255, 255, 255, 255)) < 0) {
            //     fprintf(stderr, "Couldn't fill Screen with color: %s\n",
            //     SDL_GetError());
                
            // }
        // } else {
            // global_Screen_Surface = SDL_ConvertSurface(homepage_Surface,
            //         global_Screen_Surface->format, 0);
        } else {
            global_Texture =
                SDL_CreateTextureFromSurface(global_Renderer,
                        homepage_Surface);

            if (global_Texture == NULL) {
                fprintf(stderr, "Couldn't create hardware accelerated texture: %s\n", SDL_GetError());
                exit(1);
            }
        }
        SDL_FreeSurface(homepage_Surface);
    }


    // SDL_UpdateWindowSurface(global_Window);

    

    int quit = 0;
    SDL_Event currentEvent;
    while (quit != 1) {
        while( SDL_PollEvent(&currentEvent) != 0 ) {
            if (currentEvent.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Basically clearing then re-doing each frame 
        SDL_RenderClear(global_Renderer);
        SDL_RenderCopy(global_Renderer, global_Texture, NULL, NULL);
        SDL_RenderPresent(global_Renderer);
    }

    printf("Goodbye!\n");

    SDL_FreeSurface(global_Screen_Surface);
    global_Screen_Surface = NULL;
    SDL_DestroyWindow(global_Window);
    global_Window = NULL;
}
