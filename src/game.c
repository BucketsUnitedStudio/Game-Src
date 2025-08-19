//
// Game.c 
//

#ifndef GAME_C
#define GAME_C
#include "game.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <string.h>

const char LOGO_ICON_RAW[] = {
#embed "../art/sprites/LOGO.jpg"
};

const int LOGO_ICON_RAW_LEN = sizeof(LOGO_ICON_RAW) / sizeof(LOGO_ICON_RAW[0]);

const char FONT_RAW[] = {
#embed "../fonts/BlockMonoFont/BlockMono-Bold.ttf" 
};
const int FONT_RAW_LEN = sizeof(FONT_RAW) / sizeof(FONT_RAW[0]);

// Color constants
const SDL_Color White = {255, 255, 255, 255};
const SDL_Color Grey = {128, 128, 128, 255};
const SDL_Color Black = {0, 0, 0, 255};
const SDL_Color Red = {255, 0, 0, 255};

// Main window
struct Window_Info global_Window = {.Window = NULL, .Rect = {0, 0,
  INITIAL_WIDTH, INITIAL_HEIGHT} };

// Main renderer and texture
SDL_Renderer* global_Renderer = NULL;

TTF_Font* global_Font = NULL;
TTF_Font* global_Font_Title = NULL;

// Shows what 'part' of the game we are in
enum gameMode global_Game_Mode = LOADING_SCREEN;

enum directions user_inputs;

struct Game_Settings_t Game_Settings = {
  .Keybinds = {
    .inputs = {
       [UP-1]={NULL, SDLK_w, "NORTH"},
      [DOWN-1]={NULL, SDLK_s, "SOUTH"},
      [LEFT-1]={NULL, SDLK_a, "WEST"},
      [RIGHT-1]={NULL, SDLK_d, "EAST"},
      [SELECT-1]={NULL, SDLK_SPACE, "SELECT"}
    },
    .end_buffer = {NULL, 0, NULL}
  },
  .Misc_Settings = {
    {"EasyMode", SDL_FALSE}, 

    // Buffer option
    {NULL, SDL_FALSE}
  }
};

// - the mode member helps divide the settings into categories e. g. MISC or
// KEYBINDS
// - the 'string' member is the array of mode strings (the strings that identify
// a mode)
// - the 'strings_end' is just a buffer to  be sure that strings don't
// accidentally overwrite the NULL byte
struct settings_file_t settings_file = {
  .mode=NONE,
  .strings= {
    "[KEYBINDS]",
    "[MISC]"
  },
  .strings_end=NULL
};

// Initializes a Menu struct so that there are the correct amounts of menu
// options/ submenus
//
// NOTE:
// Make sure to destroy menu if you are trying to re-init a Menu
void Menu_init(struct Menu *empty_Menu, int option_count) {
  memset(empty_Menu, '\0', sizeof(struct Menu));
  if (option_count <= 0) 
    handle_Error(global_Window.Window, "Error trying to create menu",
        "option_count is set to a non-real number (<=0)",
        SDL_FALSE);
  empty_Menu->option_count = option_count;
  memset(&empty_Menu->highlight, '\0', sizeof(struct Texture_Info));
  empty_Menu->textures = calloc(option_count, sizeof(struct Texture_Info));
  empty_Menu->text_for_options = calloc(option_count, sizeof(char*));
  empty_Menu->selected_index = 0;
}

// Aligns menu items (vertically center-oriented relative to reference_point)
void Menu_align(struct Menu* to_align, SDL_Rect* reference_point, const int
    spacing) {
  to_align->textures[0].Rect.x = reference_point->x;
  to_align->textures[0].Rect.y = reference_point->y;

  int center_x = to_align->textures[0].Rect.x + (to_align->textures[0].Rect.w/2);

  for (int i=1; i<to_align->option_count; i++) {
    to_align->textures[i].Rect.x = center_x - (to_align->textures[i].Rect.w/2);
    to_align->textures[i].Rect.y = to_align->textures[i-1].Rect.h +
      to_align->textures[i-1].Rect.y + spacing;
  }
}

// Creates the Textures for the menu items using the Menu->text_for_options
// array of strings
void Menu_initTextures(struct Menu* menu, TTF_Font* font) {
  if (font == NULL) font = global_Font;

  for (int i=0; i<menu->option_count; i++) {
    Render_Text(menu->text_for_options[i], font, White, &menu->textures[i]);
  }
}

// Actually RenderCopy the Menu textures onto the given renderer
void Menu_renderItemTextures(struct Menu* menu, SDL_Renderer* global_Renderer) {
  SDL_RenderCopy(global_Renderer, menu->highlight.Texture, NULL,
      &menu->highlight.Rect );
  for(int i=0; i<menu->option_count; i++) {
    SDL_RenderCopy(global_Renderer, menu->textures[i].Texture, NULL,
        &menu->textures[i].Rect);
  }
}

void Menu_highlightItem(struct Menu* menu, Uint32 item_index, int border_width,
    int padding) {
  if (item_index>= menu->option_count) {
    handle_Error(global_Window.Window, 
        "The index provided by caller is too large", 
        "Now using modulo operator", SDL_FALSE);
    item_index = item_index % menu->option_count;
  }
  createHighlightFromTexture(&menu->textures[item_index], &menu->highlight,
      border_width, padding);
  menu->highlight.Rect.x = menu->textures[item_index].Rect.x - padding - border_width;
  menu->highlight.Rect.y = menu->textures[item_index].Rect.y - padding - border_width;
}

// Frees all the textures in a 'dead' menu and then sets everything to a NULL
// byte
void Menu_destroy(struct Menu* dead_menu) {
  for (int i=0; i<dead_menu->option_count; i++) {
    SDL_DestroyTexture(dead_menu->textures[i].Texture);
  }
  memset(dead_menu, '\0', sizeof(struct Menu));
}

// Tries to handle errors so that they are easier to diagnose on systems without
// a stderr stream.
// -  Needs a SDL_Window to display SDL_ShowSimpleMessageBox 
// -  msg is the basic error reason provided by the caller
// -  the error_func is supposed to be used to show more info (SDL_GetError,
//    IMG_GetError, etc) but can also be used by the caller to have a more
//    descriptive error
// -  the fatal boolean decides whether the error is fatal to the program i.e.
//    whether or not to call exit(1) 
// 
void handle_Error(SDL_Window* window, char* msg, const char* error_func, SDL_bool fatal) {
  if ((msg == NULL) || (window == NULL)) 
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, 
        "Error in calling handle_Error()", "Needs BOTH a window and message",
        window);

  const char* error_msg = (!error_func) ? "" : error_func;
  
  int new_str_len = strlen(msg) + strlen(error_msg) + 1;
  char* new_Str = calloc(new_str_len, 1);
  strcpy(new_Str, msg);
  strcat(new_Str, error_msg);

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", new_Str, window);
  if (fatal) exit(1);
}

void center_Rect(SDL_Rect* to_Be_Centered) {
  to_Be_Centered->x = (global_Window.Rect.w / 2) - (to_Be_Centered->w / 2);
  to_Be_Centered->y = (global_Window.Rect.h / 2) - (to_Be_Centered->h / 2);
}

void center_Rect_Relative(SDL_Rect* anchor, SDL_Rect* to_Be_Centered) {
  to_Be_Centered->x = anchor->x + (anchor->w - to_Be_Centered->w) / 2;
  to_Be_Centered->y = anchor->y + (anchor->h - to_Be_Centered->h) / 2;
}

int findIndexOfChar(char* str, char character, int occurrence_num) {
  if (occurrence_num < 0)
    occurrence_num = 0;
  int count=0;
  for (int letter=0; str[letter]; letter++) {
    if (str[letter] == character) {
      if (occurrence_num == 0) {
        return letter;
      }
      occurrence_num--;
    }
    count = letter;
  }
  if (occurrence_num > 0) {return strlen(str)-1;}
  return -1;
}

int frequencyOfChar(char* str, char character) {
  int len = 0;
  for (int i=0; str[i]; i++) {
    if (str[i] == character)
      len++;
  }
  return len;
}

struct array_and_len* parse_list(char* str, char delimiter){
  struct array_and_len* buffer = malloc(sizeof(struct array_and_len));
  *buffer = (struct array_and_len) {NULL, 0};

  if ((!str) || (delimiter == '\0')) goto Error;

  char* str_tmp = strdup(str);

  buffer->len = frequencyOfChar(str_tmp, ',') + 1;
  if (buffer->len == 1) {
    buffer->array = &str_tmp;
    return buffer;
  }

  buffer->array = calloc((buffer->len)+1, sizeof(char*));
  buffer->array[buffer->len-1] = NULL;

  buffer->array[0] = str_tmp;
  for (int i=0, j=1; str_tmp[i]; i++) {
    if (str_tmp[i] == delimiter) {
      str_tmp[i] = '\0';
      buffer->array[j] = &str_tmp[i+1];
      j++;
    }
  }

  return buffer;

Error:;
  handle_Error(global_Window.Window, "Error parsing save file", NULL, SDL_FALSE);
  buffer->array = NULL;
  buffer->len = 0;
  return buffer;
}

void Render_Text(const char* text, TTF_Font* font, SDL_Color text_color, struct
    Texture_Info* Texture) {
  int text_len = strlen(text);
  char* error_msg = "Unable to create a texture for a string";

  SDL_Surface* temp_surface = TTF_RenderText_Solid(font , text , text_color);
  if (!temp_surface)
    handle_Error(global_Window.Window, error_msg, TTF_GetError(), 1);

  SDL_DestroyTexture(Texture->Texture);

  Texture->Texture = (temp_surface != NULL) ?
    SDL_CreateTextureFromSurface(global_Renderer, temp_surface): NULL;

  if (!Texture->Texture)
    handle_Error(global_Window.Window, error_msg, SDL_GetError(), 1);

  if (TTF_SizeText(font, text, &Texture->Rect.w, &Texture->Rect.h) < 0) 
    handle_Error(global_Window.Window, error_msg, TTF_GetError(), 1);

  SDL_FreeSurface(temp_surface);
}

// Loads an image from a path, and creates a texture from it
void Render_Image_From_Path(const char* path, SDL_Texture** Texture) {
  SDL_Surface* temp_surface = IMG_Load(path);

  if (temp_surface == NULL) {
    handle_Error(global_Window.Window, "Unable to load image", IMG_GetError(), SDL_TRUE);
  } 

  SDL_DestroyTexture(*Texture);
  *Texture = SDL_CreateTextureFromSurface(global_Renderer, temp_surface);

  if (Texture == NULL) {
    handle_Error(global_Window.Window, "Could not create a texture from path",
        SDL_GetError(), SDL_TRUE);
  }

  SDL_FreeSurface(temp_surface);
}

void Render_Image_From_Array(SDL_Renderer* Renderer, const void *src_ptr, const
    int buf_len, SDL_Texture **Texture){
  SDL_RWops* temp = SDL_RWFromConstMem(src_ptr, buf_len);
  if (temp == NULL) handle_Error(global_Window.Window, 
      "Error reading image from memory", SDL_GetError(), SDL_TRUE);

  *Texture = IMG_LoadTexture_RW(Renderer, temp, 1);
}

void createHighlightFromTexture (struct Texture_Info* src, struct Texture_Info*
    render_target, Sint32 border_width, Sint32 padding) {
  render_target->Rect.y = render_target->Rect.x = 0;
  border_width = (border_width == 0) ? 4 : border_width;
  padding = (padding == 0) ? 4 : padding;

  if (render_target->Texture != NULL)
  {SDL_DestroyTexture(render_target->Texture);
    render_target->Texture=NULL;}

  render_target->Rect.w = src->Rect.w + 2*(border_width + padding);
  render_target->Rect.h = src->Rect.h + 2*(border_width + padding);

  SDL_Rect middle_Rect = {
    border_width, border_width,
    render_target->Rect.w - 2 * (border_width),
    render_target->Rect.h - 2 * (border_width),
  };


  render_target->Texture = SDL_CreateTexture(global_Renderer,
      SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
      render_target->Rect.w, render_target->Rect.h);

  if (render_target->Texture == NULL) {
    handle_Error(global_Window.Window, "Couldn't create render target texture",
        SDL_GetError(), SDL_TRUE);
  }

  SDL_SetRenderTarget(global_Renderer, render_target->Texture);
  SDL_SetRenderDrawColor(global_Renderer, White.r, White.g, White.b, White.a);
  SDL_RenderFillRect(global_Renderer, NULL);

  SDL_SetRenderDrawColor(global_Renderer, Black.r, Black.g, Black.b, Black.a);
  SDL_RenderFillRect(global_Renderer,&middle_Rect);

  SDL_SetRenderTarget(global_Renderer, NULL);
  SDL_SetRenderDrawColor(global_Renderer, Black.r, Black.g, Black.b, Black.a);
}

int length_Of_Frame(void * fps) {
  // This is 1000 milliseconds divided by 60 frames, so it should give the
  // (truncated/ rounded) amount of time per frame for 60fps

  int frames_per_second = 1000 / 60;
  SDL_Delay(frames_per_second);

  return 0;
}

void init(void) {
  //Initializing the SDL_Subsystems
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "Was unable to initialize SDL: %s\n",
        SDL_GetError());
    exit(1);
  }
  

  // This function is kinda weird, but I swear this is how it's supposed to
  // work: https://wiki.libsdl.org/SDL2_image/IMG_Init
  if ( (IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) == 0) {
    fprintf(stderr, "Was unable to initialize SDL_IMG extension: %s\n",
        IMG_GetError()); 
    exit(1);
  }

  global_Window.Window = SDL_CreateWindow(WINDOW_NAME,
      0, 0,
      INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN |
      SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);

  if (global_Window.Window == NULL) { fprintf(stderr, "Window couldn't be initialized: %s\n",
        SDL_GetError());
    exit(1);
  }

  SDL_MaximizeWindow(global_Window.Window);

  // Defining and checking the global_Renderer variable
  global_Renderer = SDL_CreateRenderer(global_Window.Window, -1,
      SDL_RENDERER_ACCELERATED);

  // Loading the default fonts for the game (font can be changed, this one is
  // just a placeholder)
  {
    int temp = TTF_Init();
    if (temp < 0) {
      handle_Error(global_Window.Window, "Error initializing TTF Library:",
          TTF_GetError(), SDL_TRUE);
    }

    global_Font = TTF_OpenFont(FONT_PATH, FONT_PT);
    global_Font_Title = TTF_OpenFont(FONT_PATH, FONT_PT+20);

    if ((global_Font == NULL) || (global_Font_Title == NULL)) {
      handle_Error(global_Window.Window, "Unable to open font file",
          TTF_GetError(), SDL_TRUE);
    }
  }

  {
    // Loading save data from file
    FILE* save_file_fd = NULL;
    save_file_fd = fopen(SAVE_FILE, "a+");
    if (save_file_fd == NULL) 
      handle_Error(global_Window.Window, "Unable to open save file", NULL,
          SDL_TRUE);
    
  
    // Should initialize all values to zero
    char line_buffer[256] = { 0 };
    char temp_buffer[256] = { 0 };
    
    while(
        memset(line_buffer, 0, 256) &&
        memset(temp_buffer, 0, 256) &&
        fgets(line_buffer, 255, save_file_fd)) {
      
      for (int i=0, j=0; i < strlen(line_buffer); i++) {
        temp_buffer[j] = line_buffer[i];
        j = (line_buffer[i] != ' ') ? j + 1 : j;
      }
  
      strcpy(line_buffer, temp_buffer);
  
      // Setting the mode depending on what header (the [SOME_LABEL]) is
      {
        register int i=0;
        while (settings_file.strings[i]) {
          if (!strncmp(settings_file.strings[i], line_buffer,
                strlen(settings_file.strings[i]))) {
            settings_file.mode = i;
          } 
          i++;
        }
      }
  
  
      switch (settings_file.mode) {
      case KEYBINDS:;
      // Search for keybind phrases
        int len;
        int index;
        int found=-1;
      
        for (int i=0; Game_Settings.Keybinds.inputs[i].search_query; i++) {
          len = strlen(Game_Settings.Keybinds.inputs[i].search_query);
          index=-1;
          if (!strncmp(Game_Settings.Keybinds.inputs[i].search_query, line_buffer,
                len)) {
            index = findIndexOfChar(line_buffer, '=', 0) + 1;
            line_buffer[strlen(line_buffer)-2] = '\0';
            found=i;
            break;
          }
        }
        if (found == -1) {break;}

        struct array_and_len* line_array =
          parse_list(&line_buffer[index+1], ',');

        for (int i=0; i<line_array->len; i++) {
          Game_Settings.Keybinds.inputs[found].key_name = line_array->array[i];
          Game_Settings.Keybinds.inputs[found].keycode =
            SDL_GetKeyFromName(line_array->array[i]);
        }
        break;
      case MISC:
        //Search for miscellaneous phrases
        break;
      case NO_MODE:
      default:
        break;
      }
    }
  }
}

#endif
