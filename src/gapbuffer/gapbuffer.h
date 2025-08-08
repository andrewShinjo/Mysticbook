#ifndef GAPBUFFER_H
#define GAPBUFFER_H

typedef struct
{
  char *buffer;   // The buffer containing the text
  int size;       // Total size of the buffer
  int gap_start;  // Start index of the gap
  int gap_end;    // End index of the gap
  int cursor_pos; // Current cursor position (logical position)
} GapBuffer;

// Creates a new gap buffer with initial capacity
GapBuffer *gap_buffer_new(int initial_capacity);

// Inserts a character at the current cursor position
void gap_buffer_insert_char(GapBuffer *gb, char c);

// Inserts a string at the current cursor position
void gap_buffer_insert_string(GapBuffer *gb, const char *str);

// Deletes the character before the cursor position
void gap_buffer_delete_char(GapBuffer *gb);

// Moves the gap to the specified position
void gap_buffer_move_gap(GapBuffer *gb, int new_position);

// Returns the complete string from the gap buffer
char *gap_buffer_get_string(GapBuffer *gb);

#endif // GAPBUFFER_H
