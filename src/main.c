#define SDL_MAIN_HANDLED

#include <sys/ucontext.h>
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
#include <limits.h>
#include <string.h>

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 800
#define WINDOW_NAME "Recollection"
#define FONT_PATH "fonts/BlockMonoFont/BlockMono-Bold.ttf"
#define FONT_PT 100
#define LOGO_PATH "art/sprites/LOGO.jpg"

struct Window_Info {SDL_Window* Window; SDL_Rect Rect; };
struct Texture_Info { SDL_Texture* Texture; SDL_Rect Rect; };

// Main window
struct Window_Info global_Window = {.Window = NULL, .Rect = {0, 0,
    INITIAL_WIDTH, INITIAL_HEIGHT} };

// Main renderer and texture
SDL_Renderer* global_Renderer = NULL;


TTF_Font* global_Font = NULL;

// Color constants
const SDL_Color White = {255, 255, 255};
const SDL_Color Black = {255, 0, 0, 255};

void center_Rect(SDL_Rect* to_Be_Centered) {
    to_Be_Centered->x = (global_Window.Rect.w / 2) - (to_Be_Centered->w / 2);
    to_Be_Centered->y = (global_Window.Rect.h / 2) - (to_Be_Centered->h / 2);
}

// Just Initializing a lot of stuff, and error checking for the most part
void init();

void Render_Text(const char* text, TTF_Font* font, SDL_Color text_color, struct Texture_Info* Texture) {
    int text_len = strlen(text);
    SDL_Surface* temp_surface = TTF_RenderText_Solid(font , text , text_color);

    SDL_DestroyTexture(Texture->Texture);

    Texture->Texture = (temp_surface != NULL) ? SDL_CreateTextureFromSurface(global_Renderer, temp_surface): NULL;

    if (Texture->Texture == NULL || (TTF_SizeText(font, text, &Texture->Rect.w, &Texture->Rect.h) < 0) )  {
        printf("Error creating text texture with the text %s: %s\n", text, TTF_GetError());
        exit(1);
    }

    if (TTF_SizeText(font, text, &Texture->Rect.w, &Texture->Rect.h) < 0) {
        printf("Error creating text texture with the text %s: %s\n", text, TTF_GetError());
        exit(1);
    }

    SDL_FreeSurface(temp_surface);
}

// Loads an image from a path, and creates a texture from it
void Render_Image(const char* path, SDL_Texture** Texture) {
    SDL_Surface* temp_surface = IMG_Load(path);

    if (temp_surface == NULL) {
        fprintf(stderr, "Unable to load the image at %s: %s\n", path, IMG_GetError(  ));
        exit(1);
    } 

    SDL_DestroyTexture(*Texture);
    *Texture = SDL_CreateTextureFromSurface(global_Renderer, temp_surface);

    if (Texture == NULL) {
        fprintf(stderr, "Could not create a Texture from %s: %s\n", path, SDL_GetError()); 
        exit(1);
    }

    SDL_FreeSurface(temp_surface);
}

int main() {
    init();

    // The main background texture
    struct Texture_Info global_Background_Texture = {.Texture=NULL};
    global_Background_Texture.Texture =
        SDL_CreateTexture(global_Renderer,
                SDL_PIXELFORMAT_RGB888,
                SDL_TEXTUREACCESS_TARGET, global_Window.Rect.w,
                global_Window.Rect.h);
    if (global_Background_Texture.Texture == NULL) {
        fprintf(stderr, "Unable to create background texture: %s\n", SDL_GetError()); 
        exit(1);
    }

    // SDL_Texture* Message_Texture = NULL;
    struct Texture_Info Loading_Mesage;
    Loading_Mesage.Texture = NULL;

    struct Texture_Info Loading_BUS_Logo;
    Loading_BUS_Logo.Texture = NULL;

    Render_Image(LOGO_PATH, &Loading_BUS_Logo.Texture);
    Loading_BUS_Logo.Rect.h = 200;
    Loading_BUS_Logo.Rect.w = 200;
    center_Rect(&Loading_BUS_Logo.Rect);
        
    Render_Text("Loading...", global_Font, White, &Loading_Mesage);
    

    if ( SDL_SetTextureColorMod(global_Background_Texture.Texture, Black.r, Black.g, Black.b) < 0) {
        fprintf(stderr, "Error setting Background color: %s\n",
                SDL_GetError());
        exit(1);
    }

    int quit = 0;
    SDL_Event currentEvent;
    while (quit != 1) {
        while( SDL_PollEvent(&currentEvent) != 0 ) {
            if (currentEvent.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_MaximizeWindow(global_Window.Window);

        SDL_GetWindowSize(global_Window.Window, &(global_Window.Rect.w), &(global_Window.Rect.h));

        center_Rect(&Loading_Mesage.Rect);
        Loading_Mesage.Rect.y += (global_Window.Rect.h / 4);

        center_Rect(&Loading_BUS_Logo.Rect);

        // Basically clearing then re-doing each frame 
        SDL_RenderClear(global_Renderer);

        // Background image
        SDL_RenderCopy(global_Renderer, Loading_BUS_Logo.Texture, NULL, &Loading_BUS_Logo.Rect);

        SDL_RenderCopy(global_Renderer, Loading_Mesage.Texture, NULL, &Loading_Mesage.Rect);

        SDL_RenderPresent(global_Renderer);
    }

    printf("Goodbye!\n");

    TTF_CloseFont(global_Font);
    SDL_DestroyTexture(global_Background_Texture.Texture);
    global_Background_Texture.Texture = NULL;
    SDL_DestroyRenderer(global_Renderer);
    global_Renderer = NULL;
    SDL_DestroyWindow(global_Window.Window);
    global_Window.Window = NULL;
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

    global_Window.Window = SDL_CreateWindow(WINDOW_NAME,
            0, 0,
            INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN |
            SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);

    if (global_Window.Window == NULL) {
        fprintf(stderr, "Window couldn't be initialized: %s\n",
                SDL_GetError());
        exit(1);
    }

    SDL_MaximizeWindow(global_Window.Window);

    // Defining and checking the global_Renderer variable
    global_Renderer = SDL_CreateRenderer(global_Window.Window, -1, SDL_RENDERER_ACCELERATED);

    // Loading the default font for the game (font can be changed, this one is
    // just a placeholder)
    {
        int temp = TTF_Init();
        if (temp < 0) {
            fprintf(stderr, "Error initializing TTF Library: %s\n", TTF_GetError());
            exit(1);
        }

        char* global_Font_Path = FONT_PATH;
        global_Font = TTF_OpenFont(global_Font_Path, FONT_PT);

        if (global_Font == NULL) {
            fprintf(stderr, "Unable to open font file %s: %s\n",
                    global_Font_Path, TTF_GetError());
        }
    }
}
