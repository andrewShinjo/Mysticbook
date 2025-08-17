#include <stdlib.h>
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
