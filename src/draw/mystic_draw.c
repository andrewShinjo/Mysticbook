#include "SDL.h"
#include "mystic_draw.h"

// TODO(andy): Cache texture that we use frequently.
// For example, if we draw "Hello" multiple times,
// then save the "Hello" texture so that we don't have to keep creating it.
void MysticDrawText(SDL_Renderer *renderer,
                    TTF_Font *font,
                    SDL_Color color,
                    const char *text,
                    MysticPosition position)
{
  // Create an image of the text in RAM.
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
  // Create a copy of the surface in the GPU.
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  // Position image at the (x, y) position. If the width/height of the
  // rectangle aren't equal to the surface width/height, then the text
  // will shrink/stretch to fit/fill the rectangle.
  const SDL_Rect rect = { position.x, position.y, surface->w, surface->h };
  // Delete image in RAM.
  SDL_FreeSurface(surface);
  // Draw the image on the window.
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  // Delete image in GPU.
  SDL_DestroyTexture(texture);
}