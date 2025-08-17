#include "SDL.h"
#include "SDL_ttf.h"
#include "mystic.h"
#include "./mystic_color.h"
#include "draw/mystic_draw.h"

SDL_Window *window;
SDL_Renderer *renderer;

int main()
{
  // Initialize SDL.
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL couldn't initialize. Error: %s", SDL_GetError());
    return 1;
  }

  // Initialize SDL_ttf.
  if(TTF_Init() != 0)
  {
    SDL_Log("SDL_ttf couldn't initialize. Error: %s", TTF_GetError());
    SDL_Quit();
    return 1;
  }

  // Create a window.
  window = SDL_CreateWindow("Mysticbook", 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
      SDL_WINDOW_SHOWN);

  if(window == NULL)
  {
    SDL_Log("Window couldn't be created. Error: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_RaiseWindow(window);

  // Create a renderer for the window.
  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED);

  if(renderer == NULL)
  {
    SDL_Log("Renderer couldn't be created. Error: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Load a font.
  const char *font_path = RESOURCE_PATH "/OpenSans-Regular.ttf";
  TTF_Font *font = TTF_OpenFont(
    font_path,
    32);

  if (!font)
  {
    SDL_Log("Font couldn't be loaded. Error: %s", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 1;
  }

  // Main loop that runs the app.
  SDL_Event e;
  int quit = 0;

  while (!quit)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = 1;
      }
    }

    // Draw a white background.
    MysticSetDrawColor(renderer, MYSTIC_WHITE);
    SDL_RenderClear(renderer);

    // Draw a black rectangle.
    SDL_Rect rect = {100, 100, 100, 100};
    MysticSetDrawColor(renderer, MYSTIC_BLACK);
    SDL_RenderFillRect(renderer, &rect);

    // Draw text.
    MysticDrawText(
      renderer,
      font,
      MYSTIC_BLACK,
      "Hello, world!",
      (MysticPosition) {500, 500});

    // Draw.
    SDL_RenderPresent(renderer);
  }

  // Clean up resources and quit SDL.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
