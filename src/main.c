#include "game.h"



int main(int argc, char** argv) {
  init();

  struct Texture_Info Loading_Mesage = {};
  struct Texture_Info Loading_BUS_Logo = {};
  Loading_BUS_Logo.Rect.h = 200;
  Loading_BUS_Logo.Rect.w = 200;

  Render_Image_From_Path(LOGO_PATH, &Loading_BUS_Logo.Texture);
  Render_Image_From_Array(global_Renderer, LOGO_ICON_RAW, LOGO_ICON_RAW_LEN,
      &Loading_BUS_Logo.Texture);
  center_Rect(&Loading_BUS_Logo.Rect);
    
  Render_Text("Press any key to start", global_Font,
      White, &Loading_Mesage);


  struct Texture_Info game_Title;
  Render_Text("Recollection", global_Font_Title, White,
      &game_Title);

  struct Texture_Info starting_menus[3];
  Render_Text("Start", global_Font_Title, White, &starting_menus[0]);
  Render_Text("Amnesia", global_Font_Title, White, &starting_menus[1]);
  Render_Text("Settings", global_Font_Title, White, &starting_menus[2]);

  struct Texture_Info game_confirmation_question;
  struct Texture_Info game_confirmation_answers[2];
  Render_Text("Are you sure you want to do this?", global_Font, White,
      &game_confirmation_question);
  Render_Text("No.", global_Font, White, &game_confirmation_answers[0]);
  Render_Text("Yes.", global_Font, White, &game_confirmation_answers[1]);

  struct Texture_Info game_info_bar;
    Render_Text("Exploration", global_Font, White,
        &game_info_bar);

  struct Texture_Info text_highlight;
  int text_highlight_index = 0;

  struct Menu testing_menu = {};
  Menu_init(&testing_menu, 3);
  testing_menu.text_for_options[0] = "Opt 1";
  testing_menu.text_for_options[1] = "Opt 2";
  testing_menu.text_for_options[2] = "Opt 3";
  Menu_initTextures(&testing_menu, NULL);
  Menu_align(&testing_menu, &global_Window.Rect, 20);

  int fps_limit = FPS_LIMIT;
  SDL_Thread* frame_cap_thread;

  int quit = 0;
  SDL_Event currentEvent;

  int selected_menu = 0;
  user_inputs = NONE;

  SDL_bool key_pressed = SDL_FALSE;

  while (quit != 1) {
    frame_cap_thread =
      SDL_CreateThread(&length_Of_Frame,"frame_cap_thread",
          &fps_limit);

    while( SDL_PollEvent(&currentEvent) != 0 ) {
      if (currentEvent.type == SDL_QUIT) {
        quit = 1;
      }
      if (currentEvent.type == SDL_KEYDOWN) {
        user_inputs = NONE;
        key_pressed = SDL_TRUE;
        for (int i=0; i<(SELECT); i++) {
          if (currentEvent.key.keysym.sym == Game_Settings.Keybinds.inputs[i].keycode){
            user_inputs = (enum directions) i+1;
            printf("%s\n", Game_Settings.Keybinds.inputs[i].key_name);
          }
        }
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
    case START_MENU:
      goto StartMenu;
    case SETTINGS:
      goto Settings;
    case CONFIRMATION:
      goto Confirmation;
    case DIALOGUE:
      goto Dialogue;
    case EXPLORATION:
      goto Exploration;
    case BATTLE:
      goto Battle;
    default:
      break;
    }
    
LoadingScreen:
    if (key_pressed == SDL_TRUE) {
      global_Game_Mode = START_MENU;
      user_inputs = NONE;
      goto StartMenu;
    }
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

    center_Rect(&starting_menus[0].Rect);
    starting_menus[1].Rect = starting_menus[0].Rect;
    starting_menus[1].Rect.y += global_Window.Rect.h/6;
    starting_menus[2].Rect = starting_menus[0].Rect;
    starting_menus[2].Rect.y += global_Window.Rect.h/3;
    for (int i = 0; i<=2; i++) {
      SDL_RenderCopy(global_Renderer, starting_menus[i].Texture, NULL, &starting_menus[i].Rect);
    }
  
    switch (user_inputs) {
    case UP:
      selected_menu = (selected_menu + 3 - 1) % 3;
      user_inputs = NONE;
      break;
    case DOWN:
      selected_menu = (selected_menu + 1) % 3;
      user_inputs = NONE;
      break;
    case LEFT:
      global_Game_Mode = LOADING_SCREEN;
      user_inputs = NONE;
      break;

    case RIGHT:
    case SELECT:
      // Select that game menu
      user_inputs = NONE;
      switch (selected_menu) {
        case (0):
          //Start game from where you left off
          global_Game_Mode = EXPLORATION;
          break;
        case (1):
          // Restart game from beginning
          // quit = 1;
          global_Game_Mode = CONFIRMATION;
          // Delete file with save data
          break;
        case (2):
          // Go to setting page
          global_Game_Mode = SETTINGS;
          break;
        default:
          //IDK, error
          break;
      }
    default:
      break;
    }

    SDL_SetRenderDrawColor(global_Renderer, White.r, White.g,
        White.b, White.a);
    SDL_RenderDrawRect(global_Renderer,
        &starting_menus[selected_menu].Rect);
    SDL_SetRenderDrawColor(global_Renderer, Black.r, Black.g,
        Black.b, Black.a);
    goto displayFrame;
    
Dialogue:
    goto displayFrame;
    
Exploration:
    SDL_SetRenderDrawColor(global_Renderer, Black.r, Black.g,
        Black.b, Black.a);
    center_Rect(&game_info_bar.Rect);

    SDL_RenderCopy(global_Renderer, game_info_bar.Texture, NULL,
        &game_info_bar.Rect);
    goto displayFrame;

Settings:
    //
    goto displayFrame;

Confirmation:
    SDL_RenderClear(global_Renderer);
    
    center_Rect(&game_confirmation_question.Rect);
    SDL_RenderCopy(global_Renderer,
        game_confirmation_question.Texture, NULL,
        &game_confirmation_question.Rect);
    game_confirmation_answers[0].Rect.x = game_confirmation_question.Rect.x;
    game_confirmation_answers[0].Rect.y = game_confirmation_question.Rect.y + 100;

    game_confirmation_answers[1].Rect.x = game_confirmation_question.Rect.x +
      game_confirmation_question.Rect.w - game_confirmation_answers[1].Rect.w;
    game_confirmation_answers[1].Rect.y = game_confirmation_question.Rect.y + 100;

    if ((user_inputs == RIGHT) || (user_inputs == LEFT)) {
      text_highlight_index = (text_highlight_index+1) % 2;
      user_inputs = NONE;
    }

    // Here
    createHighlightFromTexture(&game_confirmation_answers[text_highlight_index],
        &text_highlight, 4, 4);
    center_Rect_Relative(&game_confirmation_answers[text_highlight_index].Rect,
        &text_highlight.Rect);

    SDL_RenderCopy(global_Renderer, text_highlight.Texture,
        NULL, &text_highlight.Rect);

    SDL_RenderCopy(global_Renderer,
        game_confirmation_answers[0].Texture, NULL,
        &game_confirmation_answers[0].Rect);

    SDL_RenderCopy(global_Renderer,
        game_confirmation_answers[1].Texture, NULL,
        &game_confirmation_answers[1].Rect);

    if (user_inputs == SELECT) {
      if (text_highlight_index == 0) {
        user_inputs = NONE;
        SDL_RenderClear(global_Renderer);
        global_Game_Mode = START_MENU;
        goto StartMenu;
      }
      quit = 1;
    }

    goto displayFrame;

Battle:
    goto displayFrame;
    
displayFrame:  
    SDL_RenderPresent(global_Renderer);

    SDL_WaitThread(frame_cap_thread, NULL);
  }

  TTF_CloseFont(global_Font);
  SDL_DestroyRenderer(global_Renderer);
  global_Renderer = NULL;
  SDL_DestroyWindow(global_Window.Window);
  global_Window.Window = NULL;
}





