#include "./mystic_color.h"

const SDL_Color MYSTIC_BLACK = {0, 0, 0, 255};
const SDL_Color MYSTIC_WHITE = {255, 255, 255, 255};

void MysticSetDrawColor(SDL_Renderer *renderer, SDL_Color color)
{
  SDL_SetRenderDrawColor(renderer,
    color.r,
    color.g,
    color.b,
    color.a);
}
