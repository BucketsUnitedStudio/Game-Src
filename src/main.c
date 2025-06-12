#include <sys/ucontext.h>
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
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 800


// Main window
SDL_Window* global_Window = NULL;

// Main Screen Surface
SDL_Surface* global_Screen_Surface = NULL;

// Main renderer and texture
SDL_Renderer* global_Renderer = NULL;
SDL_Texture* global_Texture = NULL;

TTF_Font* global_Font = NULL;

SDL_Rect global_Window_Rect = {0, 0, INITIAL_WIDTH, INITIAL_HEIGHT};

SDL_Rect game_Message_Rect = {320, 300, 200, 50};
SDL_Color White = {255, 255, 255};

void init();

int main() {
    init();

    SDL_Texture* Message_Texture = NULL;
        
    // Using global_Screen_Surface as a buffer of sorts
    global_Screen_Surface = TTF_RenderText_Solid(global_Font,
        "Loading...", White);

    Message_Texture = SDL_CreateTextureFromSurface(global_Renderer,  global_Screen_Surface);


    SDL_FreeSurface(global_Screen_Surface);
    global_Screen_Surface = NULL;
    
    SDL_Rect loading_Screen_Rect; 
    loading_Screen_Rect.h = 200; 
    loading_Screen_Rect.w = 200; 
    loading_Screen_Rect.x = (global_Window_Rect.w / 2) - (loading_Screen_Rect.w / 2); 
    loading_Screen_Rect.y = (global_Window_Rect.h / 2) - (loading_Screen_Rect.h / 2);
    

    int quit = 0;
    SDL_Event currentEvent;
    while (quit != 1) {
        while( SDL_PollEvent(&currentEvent) != 0 ) {
            if (currentEvent.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_GetWindowSize(global_Window, &(global_Window_Rect.w), &(global_Window_Rect.h));
        game_Message_Rect.x = (global_Window_Rect.w / 2) - (game_Message_Rect.w / 2);
        game_Message_Rect.y = (global_Window_Rect.h / 2) - (game_Message_Rect.h / 2) + (global_Window_Rect.h / 4);

        loading_Screen_Rect.x = (global_Window_Rect.w / 2) - (loading_Screen_Rect.w / 2);
        loading_Screen_Rect.y = (global_Window_Rect.h / 2) - (loading_Screen_Rect.h / 2);


        // Basically clearing then re-doing each frame 
        SDL_RenderClear(global_Renderer);

        // Background image
        SDL_RenderCopy(global_Renderer, global_Texture, NULL, &loading_Screen_Rect);

        SDL_RenderCopy(global_Renderer, Message_Texture, NULL, &game_Message_Rect);

        SDL_RenderPresent(global_Renderer);
    }

    printf("Goodbye!\n");

    TTF_CloseFont(global_Font);
    SDL_DestroyTexture(global_Texture);
    global_Texture = NULL;
    SDL_DestroyRenderer(global_Renderer);
    global_Renderer = NULL;
    SDL_FreeSurface(global_Screen_Surface);
    global_Screen_Surface = NULL;
    SDL_DestroyWindow(global_Window);
    global_Window = NULL;
}








void init() {
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
            0, 0,
            INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN |
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (global_Window == NULL) {
        fprintf(stderr, "Window couldn't be initialized: %s\n",
                SDL_GetError());
        exit(1);
    }

    // Defining and checking the global_Renderer variable
    global_Renderer = SDL_CreateRenderer(global_Window, -1, SDL_RENDERER_ACCELERATED);

    // Loading the default font for the game (font can be changed, this one is
    // just a placeholder)
    {
        int temp = TTF_Init();
        if (temp < 0) {
            fprintf(stderr, "Error initializing TTF Library: %s\n", TTF_GetError());
            exit(1);
        }

        char* global_Font_Path = "fonts/BlockMonoFont/BlockMono-Bold.ttf";
        global_Font = TTF_OpenFont(global_Font_Path, 100);

        if (global_Font == NULL) {
            fprintf(stderr, "Unable to open font file %s: %s\n",
                    global_Font_Path, TTF_GetError());
        }

    }

    {
        char* homepage_path = "art/backgrounds/home-page.jpg";
        SDL_Surface* homepage_Surface =
            IMG_Load(homepage_path);

        if (homepage_Surface == NULL) {
            fprintf(stderr, 
                    "Couldn't load the specified image at %s: %s\n",
                    homepage_path, SDL_GetError());
            exit(1);
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
}
