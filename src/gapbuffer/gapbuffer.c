#include "gapbuffer.h"
#include <stdlib.h>
#include <string.h>

GapBuffer *gap_buffer_new(int initial_capacity)
{
  GapBuffer *gb = malloc(sizeof(GapBuffer));
  if (!gb)
    return NULL;

  gb->buffer = malloc(initial_capacity);
  if (!gb->buffer)
  {
    free(gb);
    return NULL;
  }

  gb->size = initial_capacity;
  gb->gap_start = 0;
  gb->gap_end = initial_capacity;
  gb->cursor_pos = 0;
  return gb;
}

void gap_buffer_insert_string(GapBuffer *gb, const char *str)
{
  if (!gb || !str)
  {
    return;
  }

  // Insert each character in sequence
  const char *c;
  for (c = str; *c; c++)
  {
    gap_buffer_insert_char(gb, *c);
  }
}

void gap_buffer_insert_char(GapBuffer *gb, char c)
{
  // If gap is empty, we need to expand the buffer
  if (gb->gap_start == gb->gap_end)
  {
    int new_size = gb->size * 2;
    char *new_buffer = realloc(gb->buffer, new_size);
    if (!new_buffer)
      return;

    // Move content after gap to the end of new buffer
    int gap_size = new_size - gb->size;
    memmove(new_buffer + gb->gap_end + gap_size,
            new_buffer + gb->gap_end,
            gb->size - gb->gap_end);

    gb->buffer = new_buffer;
    gb->gap_end += gap_size;
    gb->size = new_size;
  }

  // Insert character and move gap
  gb->buffer[gb->gap_start++] = c;
  gb->cursor_pos++;
}

void gap_buffer_delete_char(GapBuffer *gb)
{
  if (gb->cursor_pos == 0)
    return; // Nothing to delete at start

  // Move gap to cursor position if not already there
  if (gb->cursor_pos != gb->gap_start)
  {
    gap_buffer_move_gap(gb, gb->cursor_pos);
  }

  // Delete character by expanding gap backwards
  if (gb->gap_start > 0)
  {
    gb->gap_start--;
    gb->cursor_pos--;
  }
}

void gap_buffer_move_gap(GapBuffer *gb, int new_position)
{
  if (new_position < 0 || new_position > gb->size - (gb->gap_end - gb->gap_start))
  {
    return; // Invalid position
  }

  if (new_position < gb->gap_start)
  {
    // Move gap left - move characters from [new_position, gap_start) to [gap_end - (gap_start - new_position), gap_end)
    int distance = gb->gap_start - new_position;
    memmove(gb->buffer + gb->gap_end - distance,
            gb->buffer + new_position,
            distance);
  }
  else if (new_position > gb->gap_start)
  {
    // Move gap right - move characters from [gap_end, gap_end + (new_position - gap_start)) to [gap_start, new_position)
    int distance = new_position - gb->gap_start;
    memmove(gb->buffer + gb->gap_start,
            gb->buffer + gb->gap_end,
            distance);
  }

  // Update gap and cursor positions
  gb->gap_end += (new_position - gb->gap_start);
  gb->gap_start = new_position;
  gb->cursor_pos = new_position;
}

char *gap_buffer_get_string(GapBuffer *gb)
{
  if (!gb)
    return NULL;

  // Calculate total length (before gap + after gap)
  int length = gb->gap_start + (gb->size - gb->gap_end);
  char *result = malloc(length + 1); // +1 for null terminator
  if (!result)
    return NULL;

  // Copy content before gap
  memcpy(result, gb->buffer, gb->gap_start);

  // Copy content after gap
  memcpy(result + gb->gap_start,
         gb->buffer + gb->gap_end,
         gb->size - gb->gap_end);

  result[length] = '\0'; // Null-terminate
  return result;
}
