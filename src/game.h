//
// game.h 
//

#ifndef GAME_H
#define GAME_H

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
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_messagebox.h>
#include <SDL2/SDL_rwops.h>

#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

#define WINDOW_NAME "Recollection"
#define SAVE_FILE "save_file.cfg"

constexpr int INITIAL_WIDTH = 800;
constexpr int INITIAL_HEIGHT = 800;
constexpr int FONT_PT = 20;
constexpr int FPS_LIMIT = 60;

constexpr int DEFAULT_BORDER_AND_PADDING = 5;

extern const char LOGO_ICON_RAW[];
extern const int LOGO_ICON_RAW_LEN;

extern const char TEST_BACKGROUND[];
extern const int TEST_BACKGROUND_LEN;



enum gameMode {
  LOADING_SCREEN,
  START_MENU,
  SETTINGS,
  CONFIRMATION,
  EXPLORATION,
  DIALOGUE,
  BATTLE
};

enum directions {
  UP=1,
  DOWN,
  LEFT,
  RIGHT,
  SELECT,

  // Must remain the LAST direction in the enum
  // referred to by #define DIRECTION_COUNT
  NONE
};

constexpr int DIRECTION_COUNT = NONE - UP + 1;

struct key_input_t {
  char* key_name;
  SDL_KeyCode keycode;
  char* search_query;
};

struct Window_Info {SDL_Window* Window; SDL_Rect Rect; };
struct Texture_Info { SDL_Texture* Texture; SDL_Rect Rect; };

struct array_and_len{
  char** array;
  int len;
};

struct Game_Settings_t {
  struct {
    struct key_input_t inputs[DIRECTION_COUNT];
    struct key_input_t end_buffer;
  } Keybinds;
  struct {
    char* Option_name;
    SDL_bool Option_enabled;
  } Misc_Settings[];
};

struct settings_file_t {
  enum {
    NO_MODE = -1,
    KEYBINDS,
    MISC,

    ITEM_AMOUNT
  } mode;

  // Make sure to allocate enough char* pointers
  char* strings[ITEM_AMOUNT];
  char* strings_end;
};

struct Menu {
  int option_count;
  
  // The index of the selected menu item
  int selected_index;

  struct Texture_Info highlight;
  struct Texture_Info* textures;
  char** text_for_options;
};

void Menu_init(struct Menu* empty_Menu, int option_count);
void Menu_align(struct Menu* to_align, SDL_Rect* reference_point, const int
    spacing);
void Menu_initTextures(struct Menu* menu, TTF_Font* font);
void Menu_renderItemTextures(struct Menu* menu, SDL_Renderer* global_Renderer);
void Menu_highlightItem(struct Menu* menu, Uint32 item_index, int border_width,
    int padding);
void Menu_destroy(struct Menu* dead_menu);

struct Sprite_stats { 
  int Attack;
  int Defense;
  int Speed;
};

struct Sprite_inventory {
  void* weapon;
  void* armour;
  void* Item_Slots[5];
};

enum Sprite_status_effect {
  SPEED,
  INVINC
};

struct Sprite {
  char* name;
  enum Sprite_status_effect status;
  struct Sprite_stats stats;
  struct Texture_Info info;
  void* inventory[5];
};

extern void Sprite_create(struct Sprite* buffer_Sprite, char* img_array, char*
    name);

extern struct Window_Info global_Window;
extern SDL_Renderer* global_Renderer;
extern enum gameMode global_Game_Mode;
extern TTF_Font* global_Font;
extern TTF_Font* global_Font_Title;
extern struct Game_Settings_t Game_Settings;
extern struct settings_file_t settings_file;
extern enum directions user_inputs;

// Color constants
extern const SDL_Color White;
extern const SDL_Color Grey;
extern const SDL_Color Black;
extern const SDL_Color Red;


extern void handle_Error(SDL_Window* window, char* msg, const char* error_func,
    SDL_bool fatal );
extern void center_Rect(SDL_Rect* to_Be_Centered);
extern void center_Rect_Relative(SDL_Rect* anchor, SDL_Rect* to_Be_Centered);
extern int findIndexOfChar(char* str, char character, int occurrence_num); 
extern int frequencyOfChar(char* str, char character);
extern struct array_and_len* parse_list(char* str, char delimiter);
extern void Render_Text(const char* text, TTF_Font* font, SDL_Color text_color,
    struct Texture_Info* Texture);
extern void Render_Image_From_Path(const char* path, SDL_Texture** Texture);
extern void Render_Image_From_Array(SDL_Renderer* Renderer, const void* src_ptr,
    const int buff_len, struct Texture_Info* Texture);
extern void createHighlightFromTexture (struct Texture_Info* src, struct
    Texture_Info* render_target, Sint32 border_width, Sint32 padding);
extern int length_Of_Frame(void * fps);



// Just Initializing a lot of stuff, and error checking for the most part
extern void init(void);
#endif
