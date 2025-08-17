#ifndef MYSTIC_COLOR_H
#define MYSTIC_COLOR_H

#include <SDL.h>

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} MysticColor;

// MysticColor constants are defined in mystic_color.c
extern const SDL_Color MYSTIC_BLACK;
extern const SDL_Color MYSTIC_WHITE;

/**
 * A helper function to easily set the SDL renderer's color
 * using a MysticColor struct.
 */
void MysticSetDrawColor(SDL_Renderer *renderer, SDL_Color color);

#endif // MYSTIC_COLOR_H
