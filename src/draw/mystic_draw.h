#ifndef MYSTIC_DRAW_H
#define MYSTIC_DRAW_H

#include "SDL_ttf.h"
#include "../mystic_color.h"
#include "../mystic.h"

/**
 * Draw text with a given font and color at a given position.
 */
void MysticDrawText(SDL_Renderer *renderer,
                    TTF_Font *font,
                    SDL_Color color,
                    const char *text,
                    MysticPosition position);

#endif //MYSTIC_DRAW_H
