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
