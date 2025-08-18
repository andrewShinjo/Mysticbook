#include <stdlib.h>
#include <string.h>
#include "mystic_editor_layout.h"

int MysticEditorLineInit(MysticEditorLine *self, int capacity)
{
  if (capacity <= 0)
  {
    return 1;
  }

  self->x_start = -1;
  self->y_start = -1;
  self->height = 0;

  self->chars = malloc(capacity * sizeof(char));
  self->x_positions = malloc(capacity * sizeof(uint16_t));

  self->length = 0;
  self->capacity = capacity;

  self->dirty_from = -1;

  return 0;
}

bool MysticEditorLineInsertAt(MysticEditorLine *self, uint16_t index,
                             const char *s, uint16_t n)
{
  if (!self || !s || index >= self->capacity ||
    self->length + n > self->capacity)
  {
    return false;
  }

  if (index < self->length)
  {
    uint16_t bytes_to_move = self->length - index;
    memmove(self->chars + index + n, self->chars + index, bytes_to_move);
  }

  memcpy(self->chars + index, s, n);
  self->length += n;

  return true;
}

bool MysticEditorLineReserve(MysticEditorLine *self, int new_capacity)
{
  // If the new capacity is less than or equal to the original capacity,
  // then do nothing and return false.
  if (!self || new_capacity <= self->capacity || new_capacity < self->length)
  {
    return false;
  }

  // Create arrays with the new capacity.
  char *new_chars = malloc(new_capacity * sizeof(char));
  uint16_t *new_x_positions = malloc(new_capacity * sizeof(uint16_t));

  // Copy the contents of the original array to the new one and free the
  // original array.
  memcpy(new_chars, self->chars, self->length);
  free(self->chars);
  self->chars = new_chars;

  memcpy(new_x_positions, self->x_positions, self->length);
  free(self->x_positions);
  self->x_positions = new_x_positions;

  // Set new capacity.
  self->capacity = new_capacity;

  return true;
}
