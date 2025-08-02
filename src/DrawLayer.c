#include <stdlib.h>
#include "./DrawLayer.h"

DrawLayer* draw_create(SDL_Window *window, SDL_Renderer *renderer)
{
  DrawLayer *draw = malloc(sizeof(DrawLayer));
  draw->window = window;
  draw->renderer = renderer;
  return draw;
}
