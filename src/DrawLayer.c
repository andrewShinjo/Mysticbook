#include <stdlib.h>
#include "./DrawLayer.h"

DrawLayer* draw_create(SDL_Window *window, SDL_Renderer *renderer)
{
  DrawLayer *draw = malloc(sizeof(DrawLayer));
  draw->window = window;
  draw->renderer = renderer;
  return draw;
}

void fill_rectangle(
  DrawLayer *draw,
  float x,
  float y,
  float width,  
  float height,
  SDL_Color color)
{
  SDL_SetRenderDrawColor(
    draw->renderer, 
    color.r, 
    color.g, 
    color.b, 
    color.a); 
  SDL_FRect rect = { x, y, width, height };
  SDL_RenderFillRect(draw->renderer, &rect);
}
