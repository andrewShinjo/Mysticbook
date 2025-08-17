#ifndef MYSTIC_EDITOR_LAYOUT_H
#define MYSTIC_EDITOR_LAYOUT_H
#include <stdbool.h>
#include <stdint.h>

// TODO(andy): change int -> int16_t if possible to save space.
typedef struct
{
  // Layout & metrics:
  int x_start;      // x-position of the left side of the line
  int y_start;      // y-position of the top of the line
  uint16_t height;  // line height

  // Text & positions:
  char *chars;            // the characters
  uint16_t *x_positions;  // the x-position of each character

  // Sizes:
  uint16_t length;    // the number of characters in the line
  uint16_t capacity;  // current max size of the buffer

  // Incremental layout:
  int dirty_from;   // -1 = clean, else recompute suffix from here before use
} MysticEditorLine;

/**
 * Initializes a line with a starting capacity (greater than zero).
 * Sets dirty_from to -1.
 * Returns 0 on success, nonzero on failure.
 */
int MysticEditorLineInit(MysticEditorLine *self, int capacity);

/**
 * Insert `n` characters from `s` at position `index`
 */
bool MysticEditorLineInsertAt(
  MysticEditorLine *self,
  uint16_t index,
  const char *s,
  uint16_t n);

/**
 * Allocates a new capacity for the MysticEditorLine.
 */
bool MysticEditorLineReserve(MysticEditorLine *self, int new_capacity);

#endif //MYSTIC_EDITOR_LAYOUT_H
