//
// Main.c 
//

#include "SDL_rect.h"
#include "SDL_render.h"
#include "game.h"
#include "string.h"
#include <stdio.h>



int main(int argc, char** argv) {
  init();

  struct Texture_Info Loading_Mesage = {};
  struct Texture_Info Loading_BUS_Logo = {};

  // Render_Image_From_Path(LOGO_PATH, &Loading_BUS_Logo.Texture);
  Render_Image_From_Array(global_Renderer, LOGO_ICON_RAW, LOGO_ICON_RAW_LEN,
      &Loading_BUS_Logo);
  Loading_BUS_Logo.Rect.h = 200;
  Loading_BUS_Logo.Rect.w = 200;
  center_Rect(&Loading_BUS_Logo.Rect);
    
  Render_Text("Press any key to start", global_Font,
      White, &Loading_Mesage);


  struct Texture_Info game_Title;
  Render_Text("Recollection", global_Font_Title, White,
      &game_Title);

  struct Texture_Info game_confirmation_question;
  // struct Texture_Info game_confirmation_answers[2];
  Render_Text("Are you sure you want to do this?", global_Font, White,
      &game_confirmation_question);

  struct Texture_Info game_info_bar;
    Render_Text("Exploration", global_Font, White,
        &game_info_bar);

  struct Menu confirm_Menu = {};
  Menu_init(&confirm_Menu, 2);
  confirm_Menu.text_for_options[0] = "No";
  confirm_Menu.text_for_options[1] = "Yes";
  Menu_initTextures(&confirm_Menu, global_Font);

  struct Menu main_Menu = {};
  Menu_init(&main_Menu, 3);
  main_Menu.text_for_options[0] = "Start";
  main_Menu.text_for_options[1] = "Amnesia";
  main_Menu.text_for_options[2] = "Settings";
  Menu_initTextures(&main_Menu, global_Font_Title);

  // Import image here
  struct Texture_Info test_background = {};
  Render_Image_From_Array(global_Renderer, TEST_BACKGROUND, TEST_BACKGROUND_LEN,
      &test_background);
  SDL_Rect test_player_View = {};
  SDL_GetWindowSize(global_Window.Window, &(test_player_View.w), &(test_player_View.h));

  int fps_limit = FPS_LIMIT;
  SDL_Thread* frame_cap_thread;

  int quit = 0;
  SDL_Event currentEvent;

  user_inputs = NONE;

  SDL_bool key_pressed = SDL_FALSE;

  struct Sprite Player = {};
  Player.name = "Bozo";
  Player.info.Rect = (SDL_Rect) {.x= 0,.y= 0};

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

    center_Rect(&main_Menu.textures[0].Rect);
    Menu_align(&main_Menu, &main_Menu.textures[0].Rect, 40);
    Menu_highlightItem(&main_Menu, main_Menu.selected_index, 10, 10);
    Menu_renderItemTextures(&main_Menu, global_Renderer);
  
    switch (user_inputs) {
    case UP:
      main_Menu.selected_index = (main_Menu.selected_index +
          main_Menu.option_count - 1) % main_Menu.option_count;
      user_inputs = NONE;
      break;
    case DOWN:
      main_Menu.selected_index = (main_Menu.selected_index + 1) %
        main_Menu.option_count;
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
      switch (main_Menu.selected_index) {
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

    goto displayFrame;
    
Dialogue:
    goto displayFrame;
    
Exploration:

    switch (user_inputs) {
    case UP:
      test_player_View.y = (test_player_View.y - 10 >= 0) ?
        test_player_View.y - 10: 0;
      break;
    case DOWN:
      test_player_View.y = 
        (test_player_View.y + global_Window.Rect.h + 10 <test_background.Rect.h)
        ? test_player_View.y + 10:
        test_background.Rect.h - global_Window.Rect.h;
      break;
    case LEFT:
      test_player_View.x = (test_player_View.x - 10 >= 0) ?
        test_player_View.x - 10: 0;
      break;

    case RIGHT:
      test_player_View.x = 
        (test_player_View.x+global_Window.Rect.w + 10 <= test_background.Rect.w) 
        ? test_player_View.x + 10:
        test_background.Rect.w - global_Window.Rect.w;
      break;

    case SELECT:
    case NONE: 
      break;
    }

    SDL_RenderCopy(global_Renderer, test_background.Texture, &test_player_View, NULL);
    SDL_SetRenderDrawColor(global_Renderer, Black.r, Black.g,
        Black.b, Black.a);

    // center_Rect(&game_info_bar.Rect);
    //
    // SDL_RenderCopy(global_Renderer, game_info_bar.Texture, NULL,
    //     &game_info_bar.Rect);
    goto displayFrame;

Settings:
    SDL_RenderClear(global_Renderer);
    goto displayFrame;

Confirmation:
    SDL_RenderClear(global_Renderer);
    
    center_Rect(&game_confirmation_question.Rect);
    game_confirmation_question.Rect.y -= 40;
    SDL_RenderCopy(global_Renderer, game_confirmation_question.Texture, NULL,
        &game_confirmation_question.Rect);

    center_Rect(&confirm_Menu.textures[0].Rect);
    Menu_align(&confirm_Menu, &confirm_Menu.textures->Rect, 20);
    Menu_highlightItem(&confirm_Menu, confirm_Menu.selected_index,
        DEFAULT_BORDER_AND_PADDING, DEFAULT_BORDER_AND_PADDING);

    switch (user_inputs) {
    case RIGHT:
    case LEFT:
    case UP:
    case DOWN:
      confirm_Menu.selected_index = (confirm_Menu.selected_index + 1) %
        confirm_Menu.option_count;
      user_inputs = NONE;
      break;
    case SELECT:
      if (confirm_Menu.selected_index == 0) {
        user_inputs = NONE;
        SDL_RenderClear(global_Renderer);
        global_Game_Mode = START_MENU;
        goto StartMenu;
      }
      quit = 1;
    default:
      break;
    }

    Menu_renderItemTextures(&confirm_Menu, global_Renderer);

    goto displayFrame;

Battle:
    goto displayFrame;
    
displayFrame:  
    user_inputs = NONE;
    SDL_RenderPresent(global_Renderer);

    SDL_WaitThread(frame_cap_thread, NULL);
  }

  TTF_CloseFont(global_Font);
  SDL_DestroyRenderer(global_Renderer);
  global_Renderer = NULL;
  SDL_DestroyWindow(global_Window.Window);
  global_Window.Window = NULL;
}





