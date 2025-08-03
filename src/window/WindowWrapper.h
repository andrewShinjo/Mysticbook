#ifndef WINDOW_WRAPPER_H
#define WINDOW_WRAPPER_H

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
} WindowWrapper;

WindowWrapper* ww_create_window(const char *title, int width, int height);
void ww_clear_window(WindowWrapper *self);

#endif
