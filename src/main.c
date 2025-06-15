#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <time.h>
#define SDL_MAIN_HANDLED

// #include <sys/ucontext.h>
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
#include <unistd.h>
#include <pthread.h>

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 800
#define WINDOW_NAME "Recollection"
#define FONT_PATH "fonts/BlockMonoFont/BlockMono-Bold.ttf"
#define FONT_PT 20
#define LOGO_PATH "art/sprites/LOGO.jpg"

enum gameMode {
    LOADING_SCREEN,
    START_MENU,
    EXPLORATION,
    DIALOGUE,
    BATTLE
};

struct Window_Info {SDL_Window* Window; SDL_Rect Rect; };
struct Texture_Info { SDL_Texture* Texture; SDL_Rect Rect; };

// Main window
struct Window_Info global_Window = {.Window = NULL, .Rect = {0, 0,
    INITIAL_WIDTH, INITIAL_HEIGHT} };

// Main renderer and texture
SDL_Renderer* global_Renderer = NULL;

TTF_Font* global_Font = NULL;
TTF_Font* global_Font_Title = NULL;

// Shows what 'part' of the game we are in
enum gameMode global_Game_Mode = LOADING_SCREEN;

// Color constants
const SDL_Color White = {255, 255, 255};
const SDL_Color Black = {0, 0, 0, 255};

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

void* length_Of_Frame(void* fps_wanted) {
    // This is 1000 milliseconds divided by 60 frames, so it should give the
    // (truncated/ rounded) amount of time per frame for 60fps

    int frames_per_second = 1000 / 60;
    SDL_Delay(frames_per_second);

    return NULL;
}

void* increment_var(int* var) {
    *var +=1;
    return NULL;
}

int main(int argc, char** argv) {
    init();

// 
// The LoadingScreen variables and initialization 
// 

    struct Texture_Info Loading_Mesage;
    Loading_Mesage.Texture = NULL;

    struct Texture_Info Loading_BUS_Logo;
    Loading_BUS_Logo.Texture = NULL;

    Render_Image(LOGO_PATH, &Loading_BUS_Logo.Texture);
    Loading_BUS_Logo.Rect.h = 200;
    Loading_BUS_Logo.Rect.w = 200;
    center_Rect(&Loading_BUS_Logo.Rect);
        
    Render_Text("Press any key to start", global_Font, White, &Loading_Mesage);


    // For the Game-Start screen 
    //
    //

    struct Texture_Info game_Title;
    Render_Text("Recollection", global_Font_Title, White, &game_Title);
    //

    int animation_stage = 0;
    pthread_t frame_cap_thread;

    int quit = 0;
    SDL_Event currentEvent;
    while (quit != 1) {

        pthread_create(&frame_cap_thread, NULL, &length_Of_Frame, NULL);

        while( SDL_PollEvent(&currentEvent) != 0 ) {
            if (currentEvent.type == SDL_QUIT) {
                quit = 1;
            }
            if ((currentEvent.type == SDL_KEYDOWN) && (global_Game_Mode == LOADING_SCREEN)) {
                global_Game_Mode = START_MENU;
                // SDL_AddTimer(2000, SDL_TimerCallback callback, void *param);
            }
        }
        SDL_GetWindowSize(global_Window.Window, &(global_Window.Rect.w), &(global_Window.Rect.h));

        // Basically clearing then re-doing each frame 
        SDL_RenderClear(global_Renderer);
        
        // Finding out what game mode we are in, and there is a goto statement
        // for each
        switch (global_Game_Mode) {
            case LOADING_SCREEN:
                goto LoadingScreen;
                break;
            case START_MENU:
                goto StartMenu;
                break;
            case DIALOGUE:
                goto Dialogue;
                break;
            case EXPLORATION:
                goto Exploration;
                break;
            case BATTLE:
                goto Battle;
                break;
        }
        
LoadingScreen:
        center_Rect(&Loading_BUS_Logo.Rect);
        center_Rect(&Loading_Mesage.Rect);
        Loading_Mesage.Rect.y += (global_Window.Rect.h / 4);

        // Background image
        SDL_RenderCopy(global_Renderer, Loading_BUS_Logo.Texture, NULL, &Loading_BUS_Logo.Rect);
        SDL_RenderCopy(global_Renderer, Loading_Mesage.Texture, NULL, &Loading_Mesage.Rect);
        goto displayFrame;

StartMenu:
        center_Rect(&game_Title.Rect);
        game_Title.Rect.y -= global_Window.Rect.h / 4;
        
        SDL_RenderCopy(global_Renderer, game_Title.Texture, NULL, &game_Title.Rect);
        goto displayFrame;
        
Dialogue:
        goto displayFrame;
        
Exploration:
        goto displayFrame;
        
Battle:
        goto displayFrame;
        
displayFrame:    
        SDL_RenderPresent(global_Renderer);
    }

    TTF_CloseFont(global_Font);
    SDL_DestroyRenderer(global_Renderer);
    global_Renderer = NULL;
    SDL_DestroyWindow(global_Window.Window);
    global_Window.Window = NULL;

    pthread_join(frame_cap_thread, NULL);
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

    // Loading the default fonts for the game (font can be changed, this one is
    // just a placeholder)
    {
        int temp = TTF_Init();
        if (temp < 0) {
            fprintf(stderr, "Error initializing TTF Library: %s\n", TTF_GetError());
            exit(1);
        }

        global_Font = TTF_OpenFont(FONT_PATH, FONT_PT);
        global_Font_Title = TTF_OpenFont(FONT_PATH, FONT_PT+20);

        if ((global_Font == NULL) || (global_Font_Title == NULL)) {
            fprintf(stderr, "Unable to open font file %s: %s\n",
                    FONT_PATH, TTF_GetError());
        }
    }
}
