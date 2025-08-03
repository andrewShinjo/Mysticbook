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
  ww->renderer = SDL_CreateRenderer(ww->window, NULL);
  SDL_RaiseWindow(ww->window);
  return ww;
}

void ww_clear_window(WindowWrapper *self)
{
  SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
  SDL_RenderClear(self->renderer);
  SDL_RenderPresent(self->renderer);
}
