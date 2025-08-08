#include <SDL.h>
#include <SDL_ttf.h>
#include "gapbuffer/gapbuffer.h"

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Color textColor = {0, 0, 0, 255}; // Black text

typedef struct
{
  char character;
  int x;
  int y;
  int width;
  int height;
} CharacterInfo;

void DrawCharInfo(CharacterInfo *ci)
{
  char char_str[2] = {ci->character, '\0'};
  SDL_Surface *charSurface = TTF_RenderText_Blended(font, char_str, textColor);
  SDL_Texture *charTexture = SDL_CreateTextureFromSurface(renderer, charSurface);

  // Set rendering position
  SDL_Rect charRect = {
      ci->x,
      ci->y,
      ci->width,
      ci->height};

  // Render the character
  SDL_RenderCopy(renderer, charTexture, NULL, &charRect);

  // Clean up
  SDL_DestroyTexture(charTexture);
  SDL_FreeSurface(charSurface);
}

/*
 * Calculate character positions for text within given bounds
 * Returns array of CharacterInfo (caller must free)
 * count will be set to number of characters that fit
 */
CharacterInfo *calculate_char_positions(const char *text, SDL_Rect bounds, int *count)
{
  *count = 0;
  if (!text || !*text)
  {
    return NULL;
  }

  // Get fixed character metrics once
  int char_height = TTF_FontHeight(font);
  int space_width;
  TTF_GlyphMetrics(font, ' ', NULL, NULL, NULL, NULL, &space_width);

  int text_len = strlen(text);
  CharacterInfo *chars = malloc(text_len * sizeof(CharacterInfo));
  if (!chars)
  {
    return NULL;
  }

  int x = bounds.x;
  int y = bounds.y;
  int max_x = bounds.x + bounds.w;
  int max_y = bounds.y + bounds.h;

  int i;
  for (i = 0; i < text_len; i++)
  {
    char c = text[i];
    int w;

    // Measure character width (use space width for non-printable chars)
    if (c == ' ' || c == '\t' || c == '\n')
    {
      w = space_width;
    }
    else
    {
      TTF_GlyphMetrics(font, c, NULL, NULL, NULL, NULL, &w);
    }

    // Check if character fits horizontally
    if (x + w > max_x)
    {
      // Move to next line
      x = bounds.x;
      y += char_height + 2; // Add small line spacing

      // Check if we have vertical space for new line
      if (y + char_height > max_y)
      {
        break; // Stop if we exceed vertical bounds
      }
    }

    // Store character info
    chars[*count].character = c;
    chars[*count].x = x;
    chars[*count].y = y;
    chars[*count].width = w;
    chars[*count].height = char_height;
    (*count)++;

    // Advance x position
    x += w;
  }

  return chars;
}

int main()
{
  // Initialize video subsystem.
  SDL_Init(SDL_INIT_VIDEO);

  // Create window.
  window = SDL_CreateWindow(
      "Mysticbook",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

  // Create renderer.
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Initialize TTF
  TTF_Init();
  font = TTF_OpenFont("../Open_Sans/static/OpenSans-Regular.ttf", 24);

  // Run the app.
  int running = 1;
  SDL_Event event;
  int window_width = 800;
  int window_height = 600;

  GapBuffer *gap_buffer = gap_buffer_new(64);
  gap_buffer_insert_string(gap_buffer, "Hello, world! My name is Andy. Hello, world! My name is Andy. Hello, world! My name is Andy.");

  while (running)
  {
    // Handle events
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = 0;
      }
      else if (event.type == SDL_WINDOWEVENT)
      {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
          window_width = event.window.data1;
          window_height = event.window.data2;
          break;
        }
      }
    }

    // Clear screen with white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Create text surface and texture
    const char *text = gap_buffer_get_string(gap_buffer);

    // Calculate character positions with updated window dimensions
    SDL_Rect bounds = {10, 10, window_width - 20, window_height - 20}; // Window bounds with 10px padding
    int char_count = 0;
    CharacterInfo *one_line = calculate_char_positions(text, bounds, &char_count);

    // Render each character individually
    int i;
    for (i = 0; i < char_count; i++)
    {
      DrawCharInfo(one_line + i);
    }
    free(one_line);

    // Present the rendered frame
    SDL_RenderPresent(renderer);
  }

  if (font)
  {
    TTF_CloseFont(font);
  }
}
