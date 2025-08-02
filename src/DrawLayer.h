#ifndef DRAW_LAYER_H
#define DRAW_LAYER_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
} DrawLayer;

DrawLayer* draw_create(SDL_Window *window, SDL_Renderer *renderer);

void fill_rectangle(
  DrawLayer *draw, 
  float x, 
  float y, 
  float width, 
  float height,
  SDL_Color color
);

#endif
