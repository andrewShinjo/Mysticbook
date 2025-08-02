#include "./GapBuffer.h"

GapBuffer* gap_buffer_create(int initial_capacity)
{
  GapBuffer *gb = malloc(sizeof(GapBuffer));
  gb->buffer = malloc(initial_capacity * sizeof(char));
  gb->capacity = initial_capacity;
  gb->gap_start = 0;
  gb->gap_end = initial_capacity - 1;
  return gb;
}
