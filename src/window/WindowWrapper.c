#include <SDL3/SDL_video.h>
#include <stdlib.h>
#include "./WindowWrapper.h"

WindowWrapper* ww_create_window(const char *title, int width, int height)
{
  WindowWrapper *ww = malloc(sizeof(WindowWrapper));
  ww->window = SDL_CreateWindow(
    title, 
    width, 
    height, 
    SDL_WINDOW_RESIZABLE);
  SDL_RaiseWindow(ww->window);
  return ww;
}
